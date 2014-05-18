// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 4 spacewar.h v1.0

#ifndef _SPACEWAR_H             // prevent multiple definitions if this 
#define _SPACEWAR_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"

//=============================================================================
// Create game class
//=============================================================================
class Spacewar : public Game
{
private:
    // variables

public:
    // Constructor
    Spacewar();

    // Destructor
    virtual ~Spacewar();

    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void releaseAll();
    void resetAll();
};

#endif
