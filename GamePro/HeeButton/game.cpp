// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// game.cpp v1.2
// Last modified Feb-11-2013

#include "game.h"

// The primary class should inherit from Game class

//=============================================================================
// Constructor
//=============================================================================
Game::Game()
{
    input = new Input();        // initialize keyboard input immediately
    // additional initialization is handled in later call to input->initialize()
    paused = false;             // game is not paused
    graphics = NULL;
    audio = NULL;
    console = NULL;
    messageDialog = NULL;
    inputDialog = NULL;
    fps = 100;
    fpsOn = false;              // default to fps display off
    initialized = false;
}

//=============================================================================
// Destructor
//=============================================================================
Game::~Game()
{
    deleteAll();                // free all reserved memory
    ShowCursor(true);           // show cursor
}

//=============================================================================
// Window message handler
//=============================================================================
LRESULT Game::messageHandler( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    if(initialized)     // do not process messages if not initialized
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
            case WM_CHAR:                           // character entered
                input->keyIn(wParam);
                return 0;
            case WM_MOUSEMOVE:                      // mouse moved
                input->mouseIn(lParam);
                return 0;
            case WM_INPUT:                          // raw mouse data in
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
            case WM_MOUSEWHEEL:                     // mouse wheel move
                input->mouseWheelIn(wParam);
                return 0;
            case WM_DEVICECHANGE:                   // check for controller insert
                input->checkControllers();
                return 0;
        }
    }
    return DefWindowProc( hwnd, msg, wParam, lParam );    // let Windows handle it
}

//=============================================================================
// Initializes the game
// throws GameError on error
//=============================================================================
void Game::initialize(HWND hw)
{
    hwnd = hw;                                  // save window handle

    // initialize graphics
    graphics = new Graphics();
    // throws GameError
    graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

    // initialize input, do not capture mouse
    input->initialize(hwnd, false);             // throws GameError

    // initialize console
    console = new Console();
    console->initialize(graphics, input);       // prepare console
    console->print("---Console---");

    // initialize messageDialog
    messageDialog = new MessageDialog();
    messageDialog->initialize(graphics, input, hwnd);

    // initialize inputDialog
    inputDialog = new InputDialog();
    inputDialog->initialize(graphics, input, hwnd);

    // initialize DirectX font
    if(dxFont.initialize(graphics, gameNS::POINT_SIZE, false, false, gameNS::FONT) == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize DirectX font."));

    dxFont.setFontColor(gameNS::FONT_COLOR);

    // init sound system
    audio = new Audio();
    if (*WAVE_BANK != '\0' && *SOUND_BANK != '\0')  // if sound files defined
    {
        if( FAILED( hr = audio->initialize() ) )
        {
            if( hr == HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND ) )
                throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize sound system because media file not found."));
            else
                throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize sound system."));
        }
    }

    // attempt to set up high resolution timer
    if(QueryPerformanceFrequency(&timerFreq) == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));

    QueryPerformanceCounter(&timeStart);        // get starting time

    initialized = true;
}

//=============================================================================
// Render game items
//=============================================================================
void Game::renderGame()
{
    const int BUF_SIZE = 20;
    static char buffer[BUF_SIZE];

    //start rendering
    if (SUCCEEDED(graphics->beginScene()))
    {
        render();           // call render() in derived object

        graphics->spriteBegin();    // begin drawing sprites
        if(fpsOn)           // if fps display requested
        {
            // convert fps to string
            _snprintf_s(buffer, BUF_SIZE, "fps %d ", (int)fps);
            dxFont.print(buffer,GAME_WIDTH-100,GAME_HEIGHT-28);
        }
        graphics->spriteEnd();      // end drawing sprites

        console->draw();    // console is drawn here so it appears on top of game

        messageDialog->draw();  // dialog is drawn on top
        inputDialog->draw();    // dialog is drawn on top

        //stop rendering
        graphics->endScene();
    }
    handleLostGraphicsDevice();

    //display the back buffer on the screen
    graphics->showBackbuffer();
}

//=============================================================================
// Handle lost graphics device
//=============================================================================
void Game::handleLostGraphicsDevice()
{
    // test for and handle lost device
    hr = graphics->getDeviceState();
    if(FAILED(hr))                  // if graphics device is not in a valid state
    {
        // if the device is lost and not available for reset
        if(hr == D3DERR_DEVICELOST)
        {
            Sleep(100);             // yield cpu time (100 mili-seconds)
            return;
        } 
        // the device was lost but is now available for reset
        else if(hr == D3DERR_DEVICENOTRESET)
        {
            releaseAll();
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
// Toggle window or fullscreen mode
//=============================================================================
void Game::setDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
    releaseAll();                   // free all user created surfaces
    graphics->changeDisplayMode(mode);
    resetAll();                     // recreate surfaces
}

//=============================================================================
// Call repeatedly by the main message loop in WinMain
//=============================================================================
void Game::run(HWND hwnd)
{
    if(graphics == NULL)            // if graphics not initialized
        return;

    // calculate elapsed time of last frame, save in frameTime
    QueryPerformanceCounter(&timeEnd);
    frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart ) / (float)timerFreq.QuadPart;

    // Power saving code, requires winmm.lib
    // if not enough time has elapsed for desired frame rate
    if (frameTime < MIN_FRAME_TIME) 
    {
        sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime)*1000);
        timeBeginPeriod(1);         // Request 1mS resolution for windows timer
        Sleep(sleepTime);           // release cpu for sleepTime
        timeEndPeriod(1);           // End 1mS timer resolution
        return;
    }

    if (frameTime > 0.0)
        fps = (fps*0.99f) + (0.01f/frameTime);  // average fps
    if (frameTime > MAX_FRAME_TIME) // if frame rate is very slow
        frameTime = MAX_FRAME_TIME; // limit maximum frameTime
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

    //check for console key
    if (input->getCharIn() == CONSOLE_KEY)
    {
        input->clearCharIn();       // clear last char
        console->showHide();
        paused = console->getVisible(); // pause game when console is visible
    }
    consoleCommand();               // process user entered console command

    input->readControllers();       // read state of controllers

    messageDialog->update();
    inputDialog->update();

    audio->run();                   // perform periodic sound engine tasks

    // if Alt+Enter toggle fullscreen/window
    if (input->isKeyDown(ALT_KEY) && input->wasKeyPressed(ENTER_KEY))
        setDisplayMode(graphicsNS::TOGGLE); // toggle fullscreen/window

    // if Esc key, set window mode
    if (input->isKeyDown(ESC_KEY))
        setDisplayMode(graphicsNS::WINDOW); // set window mode

    // if Pause key
    if (input->wasKeyPressed(VK_PAUSE))
        paused = !paused;

    // Clear input keys pressed
    // Call this after all key checks are done
    input->clear(inputNS::KEYS_PRESSED);
}

//=============================================================================
// Process console commands
// Override this function in the derived class if new console commands are added.
//=============================================================================
void Game::consoleCommand()
{
    command = console->getCommand();    // get command from console
    if(command == "")                   // if no command
        return;

    if (command == "help")              // if "help" command
    {
        console->print("Console Commands:");
        console->print("fps - toggle display of frames per second");
        return;
    }

    if (command == "fps")
    {
        fpsOn = !fpsOn;                 // toggle display of fps
        if(fpsOn)
            console->print("fps On");
        else
            console->print("fps Off");
    }
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Game::releaseAll()
{
    safeOnLostDevice(inputDialog);
    safeOnLostDevice(messageDialog);
    safeOnLostDevice(console);
    dxFont.onLostDevice();
    return;
}

//=============================================================================
// Recreate all surfaces and reset all entities.
//=============================================================================
void Game::resetAll()
{
    dxFont.onResetDevice();
    safeOnResetDevice(console);
    safeOnResetDevice(messageDialog);
    safeOnResetDevice(inputDialog);
    return;
}

//=============================================================================
// Delete all reserved memory
//=============================================================================
void Game::deleteAll()
{
    releaseAll();               // call onLostDevice() for every graphics item
    safeDelete(audio);
    safeDelete(graphics);
    safeDelete(input);
    safeDelete(console);
    safeDelete(messageDialog);
    safeDelete(inputDialog);
    initialized = false;
}