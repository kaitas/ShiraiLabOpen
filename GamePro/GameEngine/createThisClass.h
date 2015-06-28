// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// createThisClass.h

#ifndef _CREATETHIS_H           // Prevent multiple definitions if this 
#define _CREATETHIS_H           // file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#define MAX_FRAME 12387

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
	//Add by Aki
	TextureManager shipTexture;     // ship texture
	Image   ship;                   // ship image
	TextureManager bossTexture;     
	Image   boss;                    
	TextureManager zakoTexture;     
	Image   zako;                    
	TextureManager bombTexture;     
	Image   bomb	;
	Image target[MAX_FRAME]; 


    TextDX  *dxFont;            // DirectX font
    std::string  message;
    float messageY;
	int counter,max_target;
	int oto[20000][4]; //‚ ‚Ü‚è’·‚¢‚Æ“ü‚è‚«‚è‚Ü‚¹‚ñ


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
