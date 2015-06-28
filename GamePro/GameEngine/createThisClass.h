// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// createThisClass.h

#ifndef _CREATETHIS_H           // Prevent multiple definitions if this 
#define _CREATETHIS_H           // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <sstream>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "textDX.h"

//=============================================================================
// This class is the core of the game
//=============================================================================
class CreateThis : public Game
{
private:
    // game items
    TextureManager menuTexture; // textures
    Image   menu;               // menu image
    TextDX  *dxFont;            // DirectX font
    std::string  message;
    float messageY;

public:
    // Constructor
    CreateThis();
    // Destructor
    virtual ~CreateThis();
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
