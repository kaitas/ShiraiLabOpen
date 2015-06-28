// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// console.cpp v1.0

#include "console.h"

//=============================================================================
// Constructor
//=============================================================================
Console::Console()
{
    initialized = false;                // set true when successfully initialized
    graphics = NULL;
    visible = false;                    // not visible
    fontColor = consoleNS::FONT_COLOR;
    backColor = consoleNS::BACK_COLOR;

    x = consoleNS::X;                   // starting console position
    y = consoleNS::Y;

    textRect.bottom = consoleNS::Y + consoleNS::HEIGHT - consoleNS::MARGIN;
    textRect.left = consoleNS::X + consoleNS::MARGIN;
    textRect.right = consoleNS::X + consoleNS::WIDTH - consoleNS::MARGIN;
    textRect.top = consoleNS::Y + consoleNS::MARGIN;

    vertexBuffer = NULL;

    rows = 0;
    scrollAmount = 0;
}

//=============================================================================
// Destructor
//=============================================================================
Console::~Console()
{
    onLostDevice();            // call onLostDevice() for every graphics item
}

//=============================================================================
// Initialize the console
//=============================================================================
bool Console::initialize(Graphics *g, Input *in)
{
    try {
        graphics = g;                    // the graphics system
        input = in;

        // top left
        vtx[0].x = x;
        vtx[0].y = y;
        vtx[0].z = 0.0f;
        vtx[0].rhw = 1.0f;
        vtx[0].color = backColor;

        // top right
        vtx[1].x = x + consoleNS::WIDTH;
        vtx[1].y = y;
        vtx[1].z = 0.0f;
        vtx[1].rhw = 1.0f;
        vtx[1].color = backColor;

        // bottom right
        vtx[2].x = x + consoleNS::WIDTH;
        vtx[2].y = y + consoleNS::HEIGHT;
        vtx[2].z = 0.0f;
        vtx[2].rhw = 1.0f;
        vtx[2].color = backColor;

        // bottom left
        vtx[3].x = x;
        vtx[3].y = y + consoleNS::HEIGHT;
        vtx[3].z = 0.0f;
        vtx[3].rhw = 1.0f;
        vtx[3].color = backColor;

        graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);

        // initialize DirectX font
        if(dxFont.initialize(graphics, consoleNS::FONT_HEIGHT, false,
                             false, consoleNS::FONT) == false)
            return false;      // if failed
        dxFont.setFontColor(fontColor);

    } catch(...) {
        return false;
    }

    initialized = true;
    return true;
}

//=============================================================================
// draw console
// Pre: Inside BeginScene/EndScene
//=============================================================================
const void Console::draw()
{
    if (!visible || graphics == NULL || !initialized)
        return;

    graphics->drawQuad(vertexBuffer);       // draw backdrop
    if(text.size() == 0)
        return;

    graphics->spriteBegin();                // Begin drawing sprites

    // display text on console
    textRect.left = 0;
    textRect.top = 0;

    // sets textRect bottom to height of 1 row
    dxFont.print("|",textRect,DT_CALCRECT);     
    int rowHeight = textRect.bottom + 2;    // height of 1 row (+2 is row spacing)
    if(rowHeight <= 0)                      // this should never be true
        rowHeight = 20;                     // force a workable result

    // number of rows that will fit on console
    rows = (consoleNS::HEIGHT - 2*consoleNS::MARGIN) / rowHeight;
    rows -= 2;                              // room for input prompt at bottom
    if (rows <= 0)                          // this should never be true
        rows = 5;                           // force a workable result

    // set text display rect for one row
    textRect.left = (long)(x + consoleNS::MARGIN);
    textRect.right = (long)(textRect.right + consoleNS::WIDTH - consoleNS::MARGIN);
    // -2*rowHeight is room for input prompt
    textRect.bottom = (long)(y + consoleNS::HEIGHT - 2*consoleNS::MARGIN - 2*rowHeight);
    // for all rows (max text.size()) from bottom to top
    for(int r=scrollAmount; r<rows+scrollAmount && r<(int)(text.size()); r++)
    {
        // set text display rect top for this row
        textRect.top = textRect.bottom - rowHeight; 
        // display one row of text
        dxFont.print(text[r],textRect,DT_LEFT);     
        // adjust text display rect bottom for next row
        textRect.bottom -= rowHeight;               
    }

    // display command prompt and current command string
    // set text display rect for prompt
    textRect.bottom = (long)(y + consoleNS::HEIGHT - consoleNS::MARGIN);
    textRect.top = textRect.bottom - rowHeight;
    std::string prompt = ">";                   // build prompt string
    prompt += input->getTextIn();
    dxFont.print(prompt,textRect,DT_LEFT);      // display prompt and command

    graphics->spriteEnd();                      // End drawing sprites
}

//=============================================================================
// show/hide console
//=============================================================================
void Console::showHide() 
{
    if (!initialized)
        return;
    visible = !visible;
    input->clear(inputNS::KEYS_PRESSED|inputNS::TEXT_IN);    // erase old input
}            


//=============================================================================
// Add text to console
// Only the first line of text in str will be displayed.
//=============================================================================
void Console::print(const std::string &str)     // add text to console
{
    if (!initialized)
        return;
    text.push_front(str);                       // add str to deque of text
    if(text.size() > consoleNS::MAX_LINES)
        text.pop_back();                        // delete oldest line
}

//=============================================================================
// Return console command
// Handles console single key commands.
// Returns all other commands to game.
//=============================================================================
std::string Console::getCommand()
{
    // if console not initialized or not visible
    if (!initialized || !visible)               
        return "";

    //check for console key
    if (input->wasKeyPressed(CONSOLE_KEY))
        hide();                                 // turn off console

    //check for Esc key
    if (input->wasKeyPressed(ESC_KEY))
        return "";

    // check for scroll
    if (input->wasKeyPressed(VK_UP))            // if up arrow
        scrollAmount++;
    else if (input->wasKeyPressed(VK_DOWN))     // if down arrow
        scrollAmount--;
    else if (input->wasKeyPressed(VK_PRIOR))    // if page up
        scrollAmount += rows;
    else if (input->wasKeyPressed(VK_NEXT))     // if page down
        scrollAmount -= rows;
    if (scrollAmount < 0)
        scrollAmount = 0;
    if (scrollAmount > consoleNS::MAX_LINES-1)
        scrollAmount = consoleNS::MAX_LINES-1;
    if (scrollAmount > (int)(text.size())-1)
        scrollAmount = (int)(text.size())-1;

    commandStr = input->getTextIn();            // get user entered text
    // do not pass keys through to game
    input->clear(inputNS::KEYS_DOWN|inputNS::KEYS_PRESSED|inputNS::MOUSE);

    if (commandStr.length() == 0)               // if no command entered
        return "";
    if (commandStr.at(commandStr.length()-1) != '\r')   // if 'Enter' key not pressed
        return "";                              // return, can't be command

    commandStr.erase(commandStr.length()-1);    // erase '\r' from end of command string
    input->clearTextIn();                       // clear input line
    inputStr = commandStr;                      // save input text
    return commandStr;                          // return command
}


//=============================================================================
// called when graphics device is lost
//=============================================================================
void Console::onLostDevice()
{
    if (!initialized)
        return;
    dxFont.onLostDevice();
    safeRelease(vertexBuffer);
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void Console::onResetDevice()
{
    if (!initialized)
        return;
    graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);
    dxFont.onResetDevice();
}

