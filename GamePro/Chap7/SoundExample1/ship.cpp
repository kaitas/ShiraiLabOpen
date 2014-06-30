// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 7 ship.cpp v1.0

#include "ship.h"

//=============================================================================
// default constructor
//=============================================================================
Ship::Ship() : Entity()
{
    spriteData.width = shipNS::WIDTH;           // size of Ship1
    spriteData.height = shipNS::HEIGHT;
    spriteData.x = shipNS::X;                   // location on screen
    spriteData.y = shipNS::Y;
    spriteData.rect.bottom = shipNS::HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right = shipNS::WIDTH;
    velocity.x = 0;                             // velocity X
    velocity.y = 0;                             // velocity Y
    frameDelay = shipNS::SHIP_ANIMATION_DELAY;
    startFrame = shipNS::SHIP1_START_FRAME;     // first frame of ship animation
    endFrame     = shipNS::SHIP1_END_FRAME;     // last frame of ship animation
    currentFrame = startFrame;
    radius = shipNS::WIDTH/2.0;
    collisionType = entityNS::CIRCLE;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Ship::update(float frameTime)
{
    Entity::update(frameTime);
    spriteData.angle += frameTime * shipNS::ROTATION_RATE;  // rotate the ship
    spriteData.x += frameTime * velocity.x;     // move ship along X 
    spriteData.y += frameTime * velocity.y;     // move ship along Y

    // Bounce off walls
    // if hit right screen edge
    if (spriteData.x > GAME_WIDTH-shipNS::WIDTH*getScale())
    {
        // position at right screen edge
        spriteData.x = GAME_WIDTH-shipNS::WIDTH*getScale();
        velocity.x = -velocity.x;               // reverse X direction
        audio->playCue(BEEP1);                  // play sound
    } 
    else if (spriteData.x < 0)                  // else if hit left screen edge
    {
        spriteData.x = 0;                       // position at left screen edge
        velocity.x = -velocity.x;               // reverse X direction
        audio->playCue(BEEP2);                  // play sound
    }
    // if hit bottom screen edge
    if (spriteData.y > GAME_HEIGHT-shipNS::HEIGHT*getScale())
    {
        // position at bottom screen edge
        spriteData.y = GAME_HEIGHT-shipNS::HEIGHT*getScale();
        velocity.y = -velocity.y;               // reverse Y direction
        audio->playCue(BEEP3);                  // play sound
    }
    else if (spriteData.y < 0)                  // else if hit top screen edge
    {
        spriteData.y = 0;                       // position at top screen edge
        velocity.y = -velocity.y;               // reverse Y direction
//        audio->playCue(BEEP4);                  // play sound
        audio->playCue(PIYO);                  // play sound
    }
}
