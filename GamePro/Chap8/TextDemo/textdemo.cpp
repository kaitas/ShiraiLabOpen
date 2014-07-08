// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 8 textdemo.cpp v1.0

#include "textdemo.h"
using namespace textNS;

//=============================================================================
// Constructor
//=============================================================================
TextDemo::TextDemo()
{
    alignmentTimer = 0;
    fontCK      = new Text();   // sprite based font
    dxFont14    = new TextDX(); // DirectX fonts
    dxFont24    = new TextDX();
    dxFont48    = new TextDX();
    dxFont48B   = new TextDX(); // bold
    dxFont96    = new TextDX();
    dxFont24F   = new TextDX();
    spriteText  = false;        // true for sprite text, false for DirectX text
    red=0, green=0, blue=0;
    fallingY = FALLING_Y;
    angle = 0;
    pointSize = 6;
    chTimer = 0;
    ch = 0;
    st[1] = '\0';
}

//=============================================================================
// Destructor
//=============================================================================
TextDemo::~TextDemo()
{
    releaseAll();           // call deviceLost() for every graphics item
    SAFE_DELETE(fontCK);
    SAFE_DELETE(dxFont14);
    SAFE_DELETE(dxFont24);
    SAFE_DELETE(dxFont48);
    SAFE_DELETE(dxFont48B);
    SAFE_DELETE(dxFont96);
    SAFE_DELETE(dxFont24F);
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void TextDemo::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError

    graphics->setBackColor(graphicsNS::LTGRAY);

    // init text
    if (!fontCK->initialize(graphics,FONT_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing CKfont"));

    // initialize DirectX fonts
    // 14 point Arial
    if(dxFont14->initialize(graphics, 14, false, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    // 24 point Arial
    if(dxFont24->initialize(graphics, 24, false, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    // 48 point Arial
    if(dxFont48->initialize(graphics, 48, false, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    // 48 point Arial Bold
    if(dxFont48B->initialize(graphics, 48, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    // 96 point Arial
    if(dxFont96->initialize(graphics, 96, false, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    // 24 point Fixed Pitch Courier New
    if(dxFont24F->initialize(graphics, 24, true, false, "Courier New") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    reset();                // reset all game variables
    fpsOn = true;           // display frames per second

    return;
}

//=============================================================================
// Reset the game to begin play and after a score
//=============================================================================
void TextDemo::reset()
{
    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void TextDemo::update()
{
    if(input->anyKeyPressed())
        spriteText = !spriteText;   // toggle between sprite and Direct3D text

    fallingY += frameTime * FALLING_SPEED;
    if(fallingY > GAME_HEIGHT)
        fallingY = FALLING_Y;
    angle += frameTime * 360;

    alignmentTimer += frameTime;

    pointSize += frameTime * POINT_SIZE_SPEED;  // change point size
    if(pointSize > MAX_POINT_SIZE)
        pointSize = 6;

    chTimer += frameTime;
    if(chTimer > CH_TIME)
    {
        ch++;
        chTimer = 0;
    }

    if(red < 255 && green < 255 && blue == 0)
        red++;
    else if(red == 255 && green < 255 && blue == 0)
        green++;
    else if(red > 0 && green == 255 && blue == 0)
        red--;
    else if(red == 0 && green == 255 && blue < 255)
        blue++;
    else if(red == 0 && green > 0 && blue == 255)
        green--;
    else if(red < 128 && green == 0 && blue > 128)
    {
        red++;
        blue--;
    }
    else if(red > 0 && green == 0 && blue > 0)
    {
        red--;
        blue--;
    }
    else
    {
        red = 0;
        blue = 0;
        green = 0;
    }
}

//=============================================================================
// render game items and do end of round logic
//=============================================================================
void TextDemo::render()
{
    // Alternate between sprite based text and Direct3D text
    if(spriteText)
    {
        //=============================================================================
        // Sprite Font Test
        //=============================================================================
        graphics->spriteBegin();
        
        fontCK->setFontHeight(48);
        fontCK->setBold(false);

        // falling solid text
        fontCK->setProportional(true);
        fontCK->setFontColor(graphicsNS::GRAY); // solid
        fontCK->print("----I am solid----",0,(int)fallingY);

        fontCK->setProportional(false);
        fontCK->setFontColor(graphicsNS::GRAY);
        fontCK->setBackColor(TRANSCOLOR);
        // create a box
        fontCK->print("_______\n",0,-40);                  // box top
        fontCK->print(  "\220     \240\n");                // left and right edge
        fontCK->print("\257\257\257\257\257\257\257\n");   // box bottom
        // print "COLORS" inside box
        fontCK->setFontColor(SETCOLOR_ARGB(255,red,green,blue));    // changing colors
        fontCK->setDegrees(45);
        fontCK->print("COLORS",16,10);
        
        // sprite text heading
        fontCK->setFontHeight(24);
        fontCK->setProportional(true);
        fontCK->setDegrees(0);
        fontCK->setFontColor(graphicsNS::GRAY);            // set font color
        fontCK->print("Sprite Text Output",10,76);         // display message
        fontCK->setFontColor(SETCOLOR_ARGB(255,64,green,blue)); // changing colors
        fontCK->print("  Press a key to change.");         // display message

        // semi transparent text
        fontCK->setFontHeight(36);
        fontCK->setFontColor(graphicsNS::GREEN & graphicsNS::ALPHA50);  // semi transparent lime
        fontCK->print("Semi Transparent\n",0,124);
        // alpha bold
        fontCK->setBold(true);
        fontCK->print("Semi Transparent with BOLD\n");
        // plain text
        fontCK->setBold(false);
        fontCK->setFontColor(graphicsNS::PURPLE);
        fontCK->print("Plain  ");
        // bold
        fontCK->setBold(true);
        fontCK->print("Bold  ");
        fontCK->setBold(false);
        // background color
        fontCK->setBackColor(graphicsNS::WHITE);
        fontCK->print("Back Color\n");
        fontCK->setBackColor(TRANSCOLOR);
        // underline
        fontCK->setUnderline(true);
        fontCK->print("Underline",0,256);
        // underline with semi transparent text
        fontCK->setFontColor(graphicsNS::BLUE & graphicsNS::ALPHA50);  // semi transparent blue
        fontCK->print("gjpqy\n");
        fontCK->setUnderline(false);
        // fixed pitch
        fontCK->setFontColor(graphicsNS::WHITE);
        fontCK->setFontHeight(24);
        fontCK->setProportional(false);
        fontCK->print("Proportional (false)\n",0,310);
        // proportional pitch
        fontCK->setProportional(true);
        fontCK->print("Proportional (true)\n");

        // demo font alignment
        fontCK->setProportional(false);
        fontCK->setFontHeight(24);
        fontCK->print("+",500,500);
        if(alignmentTimer <= 1.0) {
            fontCK->setFontColor(graphicsNS::MAROON & graphicsNS::ALPHA50);
            fontCK->print("Left..........\nWith\nMultiple\nLines",500,500,LEFT);
        } else if(alignmentTimer <= 2.0) {
            fontCK->setFontColor(graphicsNS::OLIVE & graphicsNS::ALPHA50);
            fontCK->print("Center........\nWith\nMultiple\nLines",500,500,CENTER);
        } else if(alignmentTimer <= 3.0) {
            fontCK->setFontColor(graphicsNS::BROWN & graphicsNS::ALPHA50);
            fontCK->print("Right.........\nWith\nMultiple\nLines",500,500,RIGHT);
        } else if(alignmentTimer <= 4.0) {
            fontCK->setFontColor(graphicsNS::CYAN & graphicsNS::ALPHA50);
            fontCK->print("LeftBottom....\nWith\nMultiple\nLines",500,500,LEFT_BOTTOM);
        } else if(alignmentTimer <= 5.0) {
            fontCK->setFontColor(graphicsNS::ORANGE & graphicsNS::ALPHA50);
            fontCK->print("RightBottom...\nWith\nMultiple\nLines",500,500,RIGHT_BOTTOM);
        } else if(alignmentTimer <= 6.0) {
            fontCK->setFontColor(graphicsNS::PURPLE & graphicsNS::ALPHA50);
            fontCK->print("CenterMiddle...\nWith\nMultiple\nLines",500,500,CENTER_MIDDLE);
        } else if(alignmentTimer <= 7.0) {
            fontCK->setFontColor(graphicsNS::TEAL & graphicsNS::ALPHA50);
            fontCK->print("CenterBottom...\nWith\nMultiple\nLines",500,500,CENTER_BOTTOM);
        } else
            alignmentTimer = 0;

        // rotating text
        fontCK->setProportional(true);
        fontCK->setFontColor(SETCOLOR_ARGB(255,0,128,255)); // light blue
//        fontCK->setFontColor(SETCOLOR_ARGB(2,128,128,255)); // light blue
        fontCK->setDegrees(angle);
//        fontCK->print("Any Angle  ",700,440);
        fontCK->print("Akihiko SHIRAI  ",700,440);
        fontCK->setFontColor(SETCOLOR_ARGB(255,128,0,0));
        fontCK->setDegrees(0);

        // shadow
        fontCK->setFontHeight(48*2);
        fontCK->setFontColor(SETCOLOR_ARGB(128,128,128,128));  // shadow grey
        fontCK->print("SHADOW",400,680);
        fontCK->setFontColor(SETCOLOR_ARGB(255,255,255,0));
        fontCK->print("SHADOW",390,660);
        fontCK->setFontHeight(14);
        fontCK->print("Shadow is made with two prints, not a font feature.\n",440,740);

        // every character
        fontCK->setFontHeight(48*2);
        st[0] = ch;
        fontCK->print(st,GAME_WIDTH - 96, 10);

        // zoom in text
        fontCK->setFontHeight((int)pointSize);
        fontCK->setFontColor(SETCOLOR_ARGB(128,128,128,0));
        fontCK->print("Point size ",0,440);

        graphics->spriteEnd();

    } else {
        //=============================================================================
        // DirectX Font Test
        //=============================================================================

        graphics->spriteBegin();

        // falling solid text
        dxFont48->setFontColor(graphicsNS::GRAY);               // solid
        dxFont48->print("----I am solid----",0,(int)fallingY);

        dxFont48->setFontColor(SETCOLOR_ARGB(255,128,128,128));
        dxFont48->print("_________\n",0,-40);
        dxFont48->print(  "\220                \220\n",0,0);
        dxFont48->print("\257\257\257\257\257\257\257\257\257\n",0,40);
        dxFont48->setFontColor(SETCOLOR_ARGB(255,red,green,blue));        // changing colors
        dxFont48->print("COLORS",20,2);

        // DirectX text heading
        dxFont24->setFontColor(SETCOLOR_ARGB(255,128,128,128)); // set font color
        dxFont24->print("DirectX Text Output",10,60);           // display message
        dxFont24->setFontColor(SETCOLOR_ARGB(255,64,green,blue));  // changing colors
        dxFont24->print("Press a key to change",200,60);       // display message

        // semi transparent text
        dxFont48->setFontColor(graphicsNS::GREEN & graphicsNS::ALPHA50);  // semi transparent lime
        dxFont48->print("Semi Transparent\n",0,96);
        // alpha bold
        dxFont48B->setFontColor(graphicsNS::GREEN & graphicsNS::ALPHA50); // semi transparent lime
        dxFont48B->print("Semi Transparent with BOLD\n",0,128);
        
        // plain text
        dxFont48->setFontColor(graphicsNS::PURPLE);
        dxFont48->print("Plain",0,164);
        // bold
        dxFont48B->setFontColor(graphicsNS::PURPLE);
        dxFont48B->print("Bold",110,164);
        // no underline
        dxFont48->print("No Underline",210,164);

        // fixed pitch
        dxFont24F->setFontColor(graphicsNS::WHITE);
        dxFont24F->print("Fixed Pitch Font\n",0,256);
        // proportional pitch
        dxFont24->setFontColor(graphicsNS::WHITE);
        dxFont24->print("Proportional Font\n",0,224);

        // demo font alignment
        RECT rect = {GAME_WIDTH/2-100,GAME_HEIGHT/2-100,GAME_WIDTH/2+100,GAME_HEIGHT/2+100};
        if(alignmentTimer <= 1.0) {
            dxFont24->setFontColor(graphicsNS::MAROON & graphicsNS::ALPHA50);
            dxFont24->print("Left..........\nWith\nMultiple\nLines",rect,DT_LEFT);
        } else if(alignmentTimer <= 2.0) {
            dxFont24->setFontColor(graphicsNS::PURPLE & graphicsNS::ALPHA50);
            dxFont24->print("Center...\nWith\nMultiple\nLines", rect, DT_CENTER);
        } else if(alignmentTimer <= 3.0) {
            dxFont24->setFontColor(graphicsNS::BROWN & graphicsNS::ALPHA50);
            dxFont24->print("Right.........\nWith\nMultiple\nLines",rect,DT_RIGHT);
        } else
            alignmentTimer = 0;

        // rotating text
        dxFont24->setFontColor(SETCOLOR_ARGB(255,0,128,255)); // light blue
        dxFont24->setDegrees(angle);
        dxFont24->print("Any Angle  ",700,440);
        dxFont24->setFontColor(SETCOLOR_ARGB(255,128,0,0));
        dxFont24->setDegrees(0);

        // shadow
        dxFont96->setFontColor(SETCOLOR_ARGB(128,128,128,128)); // shadow grey
        dxFont96->print("SHADOW",460,640);
        dxFont96->setFontColor(SETCOLOR_ARGB(255,255,255,0));
        dxFont96->print("SHADOW",450,630);
        dxFont14->print("Shadow is made with two prints, not a font feature.\n",500,740);

        graphics->spriteEnd();
    }
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void TextDemo::releaseAll()
{
    SAFE_ON_LOST_DEVICE(fontCK);
    SAFE_ON_LOST_DEVICE(dxFont14);
    SAFE_ON_LOST_DEVICE(dxFont24);
    SAFE_ON_LOST_DEVICE(dxFont48);
    SAFE_ON_LOST_DEVICE(dxFont48B);
    SAFE_ON_LOST_DEVICE(dxFont96);
    SAFE_ON_LOST_DEVICE(dxFont24F);

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces and reset all entities.
//=============================================================================
void TextDemo::resetAll()
{
    SAFE_ON_RESET_DEVICE(fontCK);
    SAFE_ON_RESET_DEVICE(dxFont14);
    SAFE_ON_RESET_DEVICE(dxFont24);
    SAFE_ON_RESET_DEVICE(dxFont48);
    SAFE_ON_RESET_DEVICE(dxFont48B);
    SAFE_ON_RESET_DEVICE(dxFont96);
    SAFE_ON_RESET_DEVICE(dxFont24F);

    Game::resetAll();
    return;
}
