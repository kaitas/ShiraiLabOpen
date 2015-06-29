// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// inputDialog.h v1.1

#ifndef _INPUTDIALOG_H          // Prevent multiple definitions if this 
#define _INPUTDIALOG_H          // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

class InputDialog;

#include <string>
#include "constants.h"
#include "textDX.h"
#include "graphics.h"
#include "input.h"
#include "messageDialog.h"

namespace inputDialogNS
{
    const COLOR_ARGB TEXT_BACK_COLOR = graphicsNS::WHITE;   // input text background
    const COLOR_ARGB TEXT_COLOR = graphicsNS::BLACK;        // input text color
}

// Input Dialog, inherits from Message Dialog
class InputDialog : public MessageDialog
{
private:
    std::string inText;                 // input text
    RECT        inTextRect;
    RECT        tempRect;
    COLOR_ARGB  textBackColor;          // text area background color
    COLOR_ARGB  textFontColor;          // text area font color
    LP_VERTEXBUFFER inTextVerts;        // text area vertex buffer

public:
    // Constructor
    InputDialog();
    // Destructor
    virtual ~InputDialog();

    // Prepare vertex buffers
    void prepareVerts();

    // Display the InputDialog.
    const void draw();

    // Return input text.
    std::string getText()   
    {
        if(!visible)
            return inText;
        else
            return "";
    }

    // Set input text font color
    void setTextFontColor(COLOR_ARGB fc)  {textFontColor = fc;}

    // Set input text background color
    void setTextBackColor(COLOR_ARGB bc)  {textBackColor = bc;}

    // Display text str in InputDialog
    void print(const std::string &str);

    // Checks for Close event
    void update();

    // Call when graphics device is lost.
    void onLostDevice();
};

#endif

