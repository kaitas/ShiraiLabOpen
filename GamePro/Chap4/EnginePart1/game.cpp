// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 4 game.cpp v1.0
//////////////////////////////
//GameProg �ۑ�̂��߉��ς����R�[�h�B�^�C�g���o�[�Ɍ��݂�FPS��\��
//�ύX�_�́@206�s��Game::run���ɂ����Č�q��setWindowTitleFPS�Ăяo��
//          254~259�s��setWindowTitleFPS�̒ǉ� �^�C�g���o�[��FPS��\������


#include "game.h"

// The primary class should inherit from Game class

//=============================================================================
// Constructor �R���X�g���N�^
//=============================================================================
Game::Game()
{
    input = new Input();        // initialize keyboard input immediately
								//�L�[�{�[�h�C���v�b�g���܂�������
    // additional initialization is handled in later call to input->initialize()
	// ���̂ق��̏�������input->initialize()���Ăяo���ď���
    paused = false;             // game is not paused �Q�[���͈ꎞ��~����Ă��Ȃ����
    graphics = NULL;
    initialized = false;
}

//=============================================================================
// Destructor �f�X�g���N�^
//=============================================================================
Game::~Game()
{
    deleteAll();                // free all reserved memory ���ׂĂ̗\�񂳂ꂽ�����������
    ShowCursor(true);           // show cursor �J�[�\����\��
}

//=============================================================================
// Window message handler �E�C���h�E���b�Z�[�W�n���h��
//=============================================================================
LRESULT Game::messageHandler( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    if(initialized)     // do not process messages if not initialized
		// ����������Ă��Ȃ��ꍇ�̓��b�Z�[�W���������Ȃ�
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
            case WM_CHAR:                           // character entered ���������͂��ꂽ
                input->keyIn(wParam);
                return 0;
            case WM_MOUSEMOVE:                      // mouse moved
                input->mouseIn(lParam);
                return 0;
            case WM_INPUT:                          // raw mouse data in �}�E�X����̐��̃f�[�^����
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
				//�R���g���[���[���}�����ꂽ���ǂ����`�F�b�N
                input->checkControllers();
                return 0;
        }
    }
    return DefWindowProc( hwnd, msg, wParam, lParam );    // let Windows handle it
}

//=============================================================================
// Initializes the game  �Q�[����������
// throws GameError on error �G���[����GameError�𓊂���(�G���[��throw����)
//=============================================================================
void Game::initialize(HWND hw)
{
    hwnd = hw;          // save window handle �E�C���h�E�n���h����ۑ�

    // initialize graphics  �O���t�B�b�N�X��������
    graphics = new Graphics();
    // throws GameError GameError�𓊂���
    graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

    // initialize input, do not capture mouse
	// ���͂��������A�}�E�X�̓L���v�`�����Ȃ�
    input->initialize(hwnd, false);             // throws GameError �G���[�𓊂���

    // attempt to set up high resolution timer ������\(�����x)�^�C�}�[�̃Z�b�g������
    if(QueryPerformanceFrequency(&timerFreq) == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));

    QueryPerformanceCounter(&timeStart);        // get starting time �J�n���Ԃ��擾

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
// Handle lost graphics device ���������O���t�B�b�N�X�f�o�C�X������
//=============================================================================
void Game::handleLostGraphicsDevice()
{
	// �f�o�C�X�̏������e�X�g���A����ɉ����ď��������s
    // test for and handle lost device
    hr = graphics->getDeviceState();
	// �O���t�B�b�N�X�e�o�C�X���L���ȏ�ԂłȂ��ꍇ
    if(FAILED(hr))                  // if graphics device is not in a valid state
    {
		// �f�o�C�X���������Ă���A���Z�b�g�ł����ԂɂȂ��ꍇ
        // if the device is lost and not available for reset
        if(hr == D3DERR_DEVICELOST)
        {
            Sleep(100);             // yield cpu time (100 mili-seconds) CPU���Ԃ𖾂��n��(100�~���b)
            return;
        } 
		// �f�o�C�X���������Ă��邪�A���Z�b�g�ł����Ԃɂ���ꍇ
        // the device was lost but is now available for reset 
        else if(hr == D3DERR_DEVICENOTRESET)
        {
            releaseAll();
			// �O���t�C�c�N�X�f�o�C�X�̃��Z�b�g�����݂�
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
// WinMain�̂Ȃ��ŌJ��Ԃ��Ă΂�郁�b�Z�[�W���[�v
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

	// �ȓd�̓R�[�h�Awinmm.lib���K�v
    // Power saving code, requires winmm.lib
	// ��]����t���[�����[�g�ɑ΂��Čo�ߎ��Ԃ��Z���ꍇ�i�܂�\���ɑ����Ƃ��j
    // if not enough time has elapsed for desired frame rate
    if (frameTime < MIN_FRAME_TIME) 
    {
        sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime)*1000);
        timeBeginPeriod(1);         // Request 1mS resolution for windows timer 1�~���b�̕���\��Windows�̃^�C�}�[�ɗv��
        Sleep(sleepTime);           // release cpu for sleepTime CPU��sleepTime�̂������������(sleep)������
        timeEndPeriod(1);           // End 1mS timer resolution 1�~���b�̓��쐸�x�I���
        return;
    }

    if (frameTime > 0.0)
        fps = (fps*0.99f) + (0.01f/frameTime);  // average fps ����FPS

    if (frameTime > MAX_FRAME_TIME) // if frame rate is very slow �t���[�����[�g���ƂĂ��x���Ƃ�
        frameTime = MAX_FRAME_TIME; // limit maximum frameTime �ő�t���[�����[�g�ɐ�������

	/////GameProg �ۑ�T���v���@�ύX�ӏ� �E�B���h�E�^�C�g����FPS����������
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
/////////GameProg �ۑ�T���v���@�ύX�ӏ�/////////
void Game::setWindowTitleFPS(HWND hwnd,float frameTime)
{	
	TCHAR strFPS[ 10 ];
	sprintf(strFPS, _T("%.2f"), 1.0f/frameTime);      //1�t���[���̎��Ԃ���FPS���v�Z��������ɕϊ�
	SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)strFPS); //���b�Z�[�W�̑��M(�^�C�g���̃Z�b�g)
}
//////////////////////////////////////////////