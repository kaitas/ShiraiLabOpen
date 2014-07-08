// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 8 threeCs.cpp v1.0

#include "threeCs.h"
using namespace textNS;

//=============================================================================
// Constructor
//=============================================================================
ThreeCs::ThreeCs()
{
    fontCK = new Text();        // sprite based font
}

//=============================================================================
// Destructor
//=============================================================================
ThreeCs::~ThreeCs()
{
    releaseAll();               // call deviceLost() for every graphics item
    SAFE_DELETE(fontCK);
}

//=============================================================================
// initializes the game
// Throws GameError on error
//=============================================================================
void ThreeCs::initialize(HWND hwnd)
{
    Game::initialize(hwnd);
    graphics->setBackColor(graphicsNS::WHITE);

    // init text
    if (!fontCK->initialize(graphics,FONT_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing CKfont"));

    reset();            // reset all game variables
    fpsOn = true;       // display frames per second

    return;
}

//=============================================================================
// Reset the game to begin play and after a score
//=============================================================================
void ThreeCs::reset()
{
    return;
}

//=============================================================================
// move all game items
// frameTime is used to regulate the speed of movement
//=============================================================================
void ThreeCs::update()
{
}

//=============================================================================
// render game items
//=============================================================================
void ThreeCs::render()
{
	//‚±‚±‚ªC‚ð3‚Â‘‚¢‚Ä‚¢‚é‚Æ‚±‚ë
    graphics->spriteBegin();
    fontCK->setProportional(false);
    fontCK->setFontColor(graphicsNS::BLACK);
    fontCK->setBackColor(TRANSCOLOR);
    fontCK->setFontHeight(FONT_HEIGHT*2);
    fontCK->print("A",20,100);
    fontCK->setFontHeight(FONT_HEIGHT);
    fontCK->print("B",114,148);
    fontCK->setFontHeight(FONT_HEIGHT/4);
    fontCK->print("C",164,184);
    graphics->spriteEnd();
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void ThreeCs::releaseAll()
{
    fontCK->onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces and reset all entities.
//=============================================================================
void ThreeCs::resetAll()
{
    fontCK->onResetDevice();
    Game::resetAll();
    return;
}
