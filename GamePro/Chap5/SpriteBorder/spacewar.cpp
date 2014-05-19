// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Draw animated spaceship
// Chapter 5 spacewar.cpp v1.0
// This class is the core of the game

#include "spaceWar.h"

//=============================================================================
// Constructor
//=============================================================================
Spacewar::Spacewar()
{}

//=============================================================================
// Destructor
//=============================================================================
Spacewar::~Spacewar()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void Spacewar::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError
    graphics->setBackColor(graphicsNS::WHITE);

    // spaceship texture
    if (!shipTexture.initialize(graphics,SHIP_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship texture"));

    // ship1
    if (!ship1.initialize(graphics, SHIP_WIDTH, SHIP_HEIGHT, SHIP_COLS, &shipTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship"));
    ship1.setX(GAME_WIDTH/4);
    ship1.setY(GAME_HEIGHT/4);
    ship1.setFrames(SHIP_START_FRAME, SHIP_END_FRAME);   // animation frames
    ship1.setCurrentFrame(SHIP_START_FRAME);     // starting frame
    ship1.setFrameDelay(SHIP_ANIMATION_DELAY);

    // ship2
    if (!ship2.initialize(graphics, SHIP_WIDTH, SHIP_HEIGHT, SHIP_COLS, &shipTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship"));
    ship2.setX(GAME_WIDTH/4 + SHIP_WIDTH + 5);
    ship2.setY(GAME_HEIGHT/4);
    ship2.setFrames(SHIP_START_FRAME, SHIP_END_FRAME);   // animation frames
    ship2.setCurrentFrame(SHIP_START_FRAME);     // starting frame
    ship2.setFrameDelay(SHIP_ANIMATION_DELAY);
    ship2.setScale(8);      // 8X scale

    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void Spacewar::update()
{
    ship1.update(frameTime);
    ship2.update(frameTime);
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void Spacewar::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void Spacewar::collisions()
{}

//=============================================================================
// Render game items
//=============================================================================
void Spacewar::render()
{
    graphics->spriteBegin();                // begin drawing sprites

    ship1.draw();                           // add the spaceship to the scene
    ship2.draw();                           // add the spaceship to the scene

    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Spacewar::releaseAll()
{
    shipTexture.onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Spacewar::resetAll()
{
    shipTexture.onResetDevice();

    Game::resetAll();
    return;
}
