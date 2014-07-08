// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 8 threeCsDX.cpp v1.0

#include "threeCsDX.h"

//=============================================================================
// Constructor
//=============================================================================
ThreeCsDX::ThreeCsDX()
{
    dxFontSmall	= new TextDX();     // DirectX fonts
    dxFontMedium = new TextDX();
    dxFontLarge	= new TextDX();
}

//=============================================================================
// Destructor
//=============================================================================
ThreeCsDX::~ThreeCsDX()
{
    releaseAll();               // call deviceLost() for every graphics item
    SAFE_DELETE(dxFontSmall);
    SAFE_DELETE(dxFontMedium);
    SAFE_DELETE(dxFontLarge);
}

//=============================================================================
// initializes the game
// Throws GameError on error
//=============================================================================
void ThreeCsDX::initialize(HWND hwnd)
{
    Game::initialize(hwnd);
    graphics->setBackColor(graphicsNS::WHITE);

    // initialize DirectX fonts
    // 15 pixel high Arial
    if(dxFontSmall->initialize(graphics, 15, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    // 62 pixel high Arial
    if(dxFontMedium->initialize(graphics, 62, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    // 124 pixel high Arial
//    if(dxFontLarge->initialize(graphics, 124, true, false, "Arial") == false)
    if(dxFontLarge->initialize(graphics, 224, true, false, "Broadway") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    reset();            // reset all game variables
    fpsOn = true;       // display frames per second
    return;
}


//=============================================================================
// Reset the game to begin play and after a score
//=============================================================================
void ThreeCsDX::reset()
{
    return;
}

//=============================================================================
// move all game items
// frameTime is used to regulate the speed of movement
//=============================================================================
void ThreeCsDX::update()
{
}

//=============================================================================
// render game items
//=============================================================================
void ThreeCsDX::render()
{
    graphics->spriteBegin();

    dxFontSmall->setFontColor(graphicsNS::BLACK);
    dxFontMedium->setFontColor(graphicsNS::BLACK);
    dxFontLarge->setFontColor(graphicsNS::BLACK);
    dxFontLarge->print("A",20,100);
    dxFontMedium->print("B",114,148);
    dxFontSmall->print("C",164,184);

    graphics->spriteEnd();
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void ThreeCsDX::releaseAll()
{
    dxFontSmall->onLostDevice();
    dxFontMedium->onLostDevice();
    dxFontLarge->onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces and reset all entities.
//=============================================================================
void ThreeCsDX::resetAll()
{
    dxFontSmall->onResetDevice();
    dxFontMedium->onResetDevice();
    dxFontLarge->onResetDevice();
    Game::resetAll();
    return;
}
