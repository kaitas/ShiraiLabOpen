// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 8 game.h v1.0

#ifndef _GAME_H                 // Prevent multiple definitions if this 
#define _GAME_H                 // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <Mmsystem.h>
#include "graphics.h"
#include "input.h"
#include "audio.h"
#include "constants.h"
#include "textDX.h"
#include "console.h"
#include "gameError.h"


namespace gameNS
{
    const char FONT[] = "Courier New";  // font
    const int POINT_SIZE = 14;          // point size
    const COLOR_ARGB FONT_COLOR = SETCOLOR_ARGB(255,255,255,255);    // white
}

class Game
{
protected:
    // common game properties
    Graphics *graphics;             // pointer to Graphics
    Input   *input;                 // pointer to Input
    Audio   *audio;                 // pointer to Audio
    Console *console;               // pointer to Console
    HWND    hwnd;                   // window handle
    HRESULT hr;                     // standard return type
    LARGE_INTEGER timeStart;        // Performance Counter start value
    LARGE_INTEGER timeEnd;          // Performance Counter end value
    LARGE_INTEGER timerFreq;        // Performance Counter frequency
    float   frameTime;              // time required for last frame
    float   fps;                    // frames per second
    TextDX  dxFont;                 // DirectX font for fps
    bool    fpsOn;                  // true to display fps
    DWORD   sleepTime;              // number of milli-seconds to sleep between frames
    bool    paused;                 // true if game is paused
    bool    initialized;
    std::string  command;           // command from console

public:
    // Constructor
    Game();
    // Destructor
    virtual ~Game();

    // Member functions

    // Window message handler
    LRESULT messageHandler( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

    // Initialize the game
    // Pre: hwnd is handle to window
    virtual void initialize(HWND hwnd);

    // Call run repeatedly by the main message loop in WinMain
    virtual void run(HWND);

    // Call when the graphics device was lost.
    // Release all reserved video memory so graphics device may be reset.
    virtual void releaseAll();

    // Recreate all surfaces and reset all entities.
    virtual void resetAll();

    // Delete all reserved memory.
    virtual void deleteAll();

    // Process console commands.
    virtual void consoleCommand();

    // Render game items.
    virtual void renderGame();

    // Handle lost graphics device
    virtual void handleLostGraphicsDevice();

    // Set display mode (fullscreen, window or toggle)
    void setDisplayMode(graphicsNS::DISPLAY_MODE mode = graphicsNS::TOGGLE);

    // Return pointer to Graphics.
    Graphics* getGraphics() {return graphics;}

    // Return pointer to Input.
    Input* getInput()       {return input;}

    // Exit the game
    void exitGame()         {PostMessage(hwnd, WM_DESTROY, 0, 0);}

    // Return pointer to Audio.
    Audio* getAudio()       {return audio;}

    // Pure virtual function declarations
    // These functions MUST be written in any class that inherits from Game

    // Update game items.
    virtual void update() = 0;

    // Perform AI calculations.
    virtual void ai() = 0;

    // Check for collisions.
    virtual void collisions() = 0;

    // Render graphics.
    // Call graphics->spriteBegin();
    //   draw sprites
    // Call graphics->spriteEnd();
    //   draw non-sprites
    virtual void render() = 0;
};

#endif
