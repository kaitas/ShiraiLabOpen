// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// console.h v1.1

#ifndef _CONSOLE_H              // Prevent multiple definitions if this 
#define _CONSOLE_H              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

class Console;

#include <string>
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "graphics.h"
#include "input.h"

namespace consoleNS
{
    const UINT WIDTH = 500;             // width of console
    const UINT HEIGHT = 400;            // height of console
    const UINT X = 5;                   // console location
    const UINT Y = 5;
    const UINT MARGIN = 4;              // text margin from console edge
    const char FONT[] = "Courier New";  // console font
    const int FONT_HEIGHT = 14;         // height of the font in pixels
    const COLOR_ARGB FONT_COLOR = graphicsNS::WHITE;    // color of console text
    const COLOR_ARGB BACK_COLOR = SETCOLOR_ARGB(192,128,128,128);    // backdrop color
    const int MAX_LINES = 256;          // maximun number of lines in text buffer
}

// 
class Console
{
private:
    Graphics    *graphics;              // graphics system
    Input       *input;                 // input system
    TextDX      dxFont;                 // DirectX font
    float       x,y;                    // console location (dynamic)
    int         rows;                   // number of rows of text that will fit on console
    std::string commandStr;             // console command
    std::string inputStr;               // console text input
    std::deque<std::string> text;       // console text
    RECT        textRect;               // text rectangle
    COLOR_ARGB  fontColor;              // font color (a,r,g,b)
    COLOR_ARGB  backColor;              // background color (a,r,g,b)
    VertexC vtx[4];                     // vertex data for background
    LP_VERTEXBUFFER vertexBuffer;       // buffer to hold vertex data
    int         scrollAmount;           // number of lines to scroll the display up
    bool        initialized;            // true when initialized successfully
    bool        visible;                // true to display

public:
    // Constructor
    Console();

    // Destructor
    virtual ~Console();

    // Initialize the Console
    // Pre: *g points to Graphics
    //      *in points to Input
    bool initialize(Graphics *g, Input *in);

    // Display the Console.
    const void draw();

    // Show/Hide the Console.
    void showHide();

    // Return visible.
    bool getVisible() {return visible;}

    // Set visible = true;
    void show() {visible = true;}

    // Set visible = false;
    void hide() {visible = false;}

    // Add text str to Console display.
    // Only the first line of text in str will be displayed.
    void print(const std::string &str);

    // Return Console command
    std::string getCommand();

    // Return Console Input text
    std::string getInput() {return inputStr;}

    // Clear Input text
    void clearInput()   {inputStr = "";}

    // Call when graphics device is lost.
    void onLostDevice();

    // Call when graphics device is reset.
    void onResetDevice();
};

#endif

