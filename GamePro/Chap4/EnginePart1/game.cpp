// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 4 game.cpp v1.0
//////////////////////////////
//GameProg 課題のため改変したコード。タイトルバーに現在のFPSを表示
//変更点は　206行目Game::run内において後述のsetWindowTitleFPS呼び出し
//          254~259行目setWindowTitleFPSの追加 タイトルバーにFPSを表示する


#include "game.h"

// The primary class should inherit from Game class

//=============================================================================
// Constructor コンストラクタ
//=============================================================================
Game::Game()
{
    input = new Input();        // initialize keyboard input immediately
								//キーボードインプットをまず初期化
    // additional initialization is handled in later call to input->initialize()
	// そのほかの初期化はinput->initialize()を呼び出して処理
    paused = false;             // game is not paused ゲームは一時停止されていない状態
    graphics = NULL;
    initialized = false;
}

//=============================================================================
// Destructor デストラクタ
//=============================================================================
Game::~Game()
{
    deleteAll();                // free all reserved memory すべての予約されたメモリを解放
    ShowCursor(true);           // show cursor カーソルを表示
}

//=============================================================================
// Window message handler ウインドウメッセージハンドラ
//=============================================================================
LRESULT Game::messageHandler( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    if(initialized)     // do not process messages if not initialized
		// 初期化されていない場合はメッセージを処理しない
    {
        switch( msg )
        {
            case WM_DESTROY:
                PostQuitMessage(0);        //tell Windows to kill this program
                return 0;
            case WM_KEYDOWN: case WM_SYSKEYDOWN:    // key down
                input->keyDown(wParam);
                return 0;
            case WM_KEYUP: case WM_SYSKEYUP:        // key up
                input->keyUp(wParam);
                return 0;
            case WM_CHAR:                           // character entered 文字が入力された
                input->keyIn(wParam);
                return 0;
            case WM_MOUSEMOVE:                      // mouse moved
                input->mouseIn(lParam);
                return 0;
            case WM_INPUT:                          // raw mouse data in マウスからの生のデータ入力
                input->mouseRawIn(lParam);
                return 0;
            case WM_LBUTTONDOWN:                    // left mouse button down
                input->setMouseLButton(true);
                input->mouseIn(lParam);             // mouse position
                return 0;
            case WM_LBUTTONUP:                      // left mouse button up
                input->setMouseLButton(false);
                input->mouseIn(lParam);             // mouse position
                return 0;
            case WM_MBUTTONDOWN:                    // middle mouse button down
                input->setMouseMButton(true);
                input->mouseIn(lParam);             // mouse position
                return 0;
            case WM_MBUTTONUP:                      // middle mouse button up
                input->setMouseMButton(false);
                input->mouseIn(lParam);             // mouse position
                return 0;
            case WM_RBUTTONDOWN:                    // right mouse button down
                input->setMouseRButton(true);
                input->mouseIn(lParam);             // mouse position
                return 0;
            case WM_RBUTTONUP:                      // right mouse button up
                input->setMouseRButton(false);
                input->mouseIn(lParam);             // mouse position
                return 0;
            case WM_XBUTTONDOWN: case WM_XBUTTONUP: // mouse X button down/up
                input->setMouseXButton(wParam);
                input->mouseIn(lParam);             // mouse position
                return 0;
            case WM_DEVICECHANGE:                   // check for controller insert
				//コントローラーが挿入されたかどうかチェック
                input->checkControllers();
                return 0;
        }
    }
    return DefWindowProc( hwnd, msg, wParam, lParam );    // let Windows handle it
}

//=============================================================================
// Initializes the game  ゲームを初期化
// throws GameError on error エラー時にGameErrorを投げる(エラーをthrowする)
//=============================================================================
void Game::initialize(HWND hw)
{
    hwnd = hw;          // save window handle ウインドウハンドルを保存

    // initialize graphics  グラフィックスを初期化
    graphics = new Graphics();
    // throws GameError GameErrorを投げる
    graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

    // initialize input, do not capture mouse
	// 入力を初期化、マウスはキャプチャしない
    input->initialize(hwnd, false);             // throws GameError エラーを投げる

    // attempt to set up high resolution timer 高分解能(高精度)タイマーのセットを試す
    if(QueryPerformanceFrequency(&timerFreq) == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));

    QueryPerformanceCounter(&timeStart);        // get starting time 開始時間を取得

    initialized = true;
}

//=============================================================================
// Render game items
//=============================================================================
void Game::renderGame()
{
    //start rendering
    if (SUCCEEDED(graphics->beginScene()))
    {
        // render is a pure virtual function that must be provided in the
        // inheriting class.
        render();               // call render in derived class

        //stop rendering
        graphics->endScene();
    }
    handleLostGraphicsDevice();

    //display the back buffer on the screen
    graphics->showBackbuffer();
}

//=============================================================================
// Handle lost graphics device 消失したグラフィックスデバイスを処理
//=============================================================================
void Game::handleLostGraphicsDevice()
{
	// デバイスの消失をテストし、それに応じて処理を実行
    // test for and handle lost device
    hr = graphics->getDeviceState();
	// グラフィックステバイスが有効な状態でない場合
    if(FAILED(hr))                  // if graphics device is not in a valid state
    {
		// デバイスが消失しており、リセットできる状態にない場合
        // if the device is lost and not available for reset
        if(hr == D3DERR_DEVICELOST)
        {
            Sleep(100);             // yield cpu time (100 mili-seconds) CPU時間を明け渡す(100ミリ秒)
            return;
        } 
		// デバイスが消失しているが、リセットできる状態にある場合
        // the device was lost but is now available for reset 
        else if(hr == D3DERR_DEVICENOTRESET)
        {
            releaseAll();
			// グラフイツクスデバイスのリセットを試みる
            hr = graphics->reset(); // attempt to reset graphics device
            if(FAILED(hr))          // if reset failed
                return;
            resetAll();
        }
        else
            return;                 // other device error
    }
}

//=============================================================================
// WinMainのなかで繰り返し呼ばれるメッセージループ
// Call repeatedly by the main message loop in WinMain
//=============================================================================
void Game::run(HWND hwnd)
{
    if(graphics == NULL)            // if graphics not initialized
        return;

    // calculate elapsed time of last frame, save in frameTime
    QueryPerformanceCounter(&timeEnd);
    frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart ) / 
                (float)timerFreq.QuadPart;

	// 省電力コード、winmm.libが必要
    // Power saving code, requires winmm.lib
	// 希望するフレームレートに対して経過時間が短い場合（つまり十分に速いとき）
    // if not enough time has elapsed for desired frame rate
    if (frameTime < MIN_FRAME_TIME) 
    {
        sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime)*1000);
        timeBeginPeriod(1);         // Request 1mS resolution for windows timer 1ミリ秒の分解能をWindowsのタイマーに要求
        Sleep(sleepTime);           // release cpu for sleepTime CPUをsleepTimeのあいだだけ解放(sleep)させる
        timeEndPeriod(1);           // End 1mS timer resolution 1ミリ秒の動作精度終わり
        return;
    }

    if (frameTime > 0.0)
        fps = (fps*0.99f) + (0.01f/frameTime);  // average fps 平均FPS

    if (frameTime > MAX_FRAME_TIME) // if frame rate is very slow フレームレートがとても遅いとき
        frameTime = MAX_FRAME_TIME; // limit maximum frameTime 最大フレームレートに制限する

	/////GameProg 課題サンプル　変更箇所 ウィンドウタイトルにFPSを書き込む
	setWindowTitleFPS(hwnd,frameTime);

    timeStart = timeEnd;

    // update(), ai(), and collisions() are pure virtual functions.
    // These functions must be provided in the class that inherits from Game.
    if (!paused)                    // if not paused
    {
        update();                   // update all game items
        ai();                       // artificial intelligence
        collisions();               // handle collisions
        input->vibrateControllers(frameTime); // handle controller vibration
    }
    renderGame();                   // draw all game items
    input->readControllers();       // read state of controllers


    // Clear input
    // Call this after all key checks are done
    input->clear(inputNS::KEYS_PRESSED);
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Game::releaseAll()
{}

//=============================================================================
// Recreate all surfaces and reset all entities.
//=============================================================================
void Game::resetAll()
{}

//=============================================================================
// Delete all reserved memory
//=============================================================================
void Game::deleteAll()
{
    releaseAll();               // call onLostDevice() for every graphics item
    SAFE_DELETE(graphics);
    SAFE_DELETE(input);
    initialized = false;
}
/////////GameProg 課題サンプル　変更箇所/////////
void Game::setWindowTitleFPS(HWND hwnd,float frameTime)
{	
	TCHAR strFPS[ 10 ];
	sprintf(strFPS, _T("%.2f"), 1.0f/frameTime);      //1フレームの時間からFPSを計算し文字列に変換
	SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)strFPS); //メッセージの送信(タイトルのセット)
}
//////////////////////////////////////////////