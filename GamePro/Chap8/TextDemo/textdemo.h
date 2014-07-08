// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 8 textdemo.h v1.0

#ifndef _textdemo_h             // Prevent multiple definitions if this 
#define _textdemo_h             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "image.h"
#include "text.h"
#include "textDX.h"
#include "constants.h"

const int BUF_SIZE = 20;
const float FALLING_SPEED = 100.0f;
const float FALLING_Y = -100.0f;
const float POINT_SIZE_SPEED = 200.0f;
const float MAX_POINT_SIZE = 480.0f;
const float CH_TIME = 0.1f;

class TextDemo : public Game
{
private:
    // game items    
    Text    *fontCK;                    // sprite based font
    TextDX  *dxFont14;                  // DirectX fonts
    TextDX  *dxFont24;
    TextDX  *dxFont48;
    TextDX  *dxFont48B;                 // bold
    TextDX  *dxFont96;
    TextDX  *dxFont24F;                 // 24 point fixed pitch

    // game variables
    double  alignmentTimer;             // used to change text alignment display
    bool    spriteText;                 // true for sprite text, false for DirectX text
    byte red, green, blue;
    char buffer[BUF_SIZE];
    VECTOR2 position;                   // used to position items on screen
    float fallingY;
    float angle;
    float pointSize;                    // for changing point size
    float chTimer;
    UCHAR ch;
    char st[2];


public:
    TextDemo();
    virtual ~TextDemo();
    void initialize(HWND hwnd);
    void reset();
    void update();
    void ai() {};
    void collisions() {};
    void render();
    void releaseAll();
    void resetAll();
};

#endif
