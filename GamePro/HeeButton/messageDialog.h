// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// messageDialog.h v1.1

#ifndef _MESSAGEDIALOG_H        // prevent multiple definitions if this 
#define _MESSAGEDIALOG_H        // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

class MessageDialog;

#include <string>
#include "constants.h"
#include "textDX.h"
#include "graphics.h"
#include "input.h"

namespace messageDialogNS
{
    const UINT WIDTH = 400;             // default width of dialog
    const UINT HEIGHT = 100;            // default height
    const UINT BORDER = 5;
    const UINT MARGIN = 5;              // text margin from border
    const char FONT[] = "Arial";        // font
    const int FONT_HEIGHT = 18;         // font height
    const COLOR_ARGB FONT_COLOR = graphicsNS::WHITE;        // text color
    const COLOR_ARGB BORDER_COLOR = D3DCOLOR_ARGB(192,192,192,192); // border color
    const COLOR_ARGB BACK_COLOR = SETCOLOR_ARGB(255,100,100,192);   // backdrop color
    const UINT X = GAME_WIDTH/4;        // default location
    const UINT Y = GAME_HEIGHT/4;
    const UINT BUTTON_WIDTH = (UINT)(FONT_HEIGHT * 4.5);
    const UINT BUTTON_HEIGHT = FONT_HEIGHT + 4;
    const int MAX_TYPE = 2;
    const int OK_CANCEL = 0;            // OK Cancel button type
    const int YES_NO = 1;               // Yes No button type
    static const char* BUTTON1_TEXT[MAX_TYPE] = {"OK", "YES"};
    static const char* BUTTON2_TEXT[MAX_TYPE] = {"CANCEL", "NO"};
    const byte DIALOG_CLOSE_KEY = VK_RETURN;    // Enter key
    const COLOR_ARGB BUTTON_COLOR = graphicsNS::GRAY;       // button background
    const COLOR_ARGB BUTTON_FONT_COLOR = graphicsNS::WHITE; // button text color
}

// Message Dialog
class MessageDialog
{
protected:
    Graphics    *graphics;              // graphics system
    Input       *input;                 // input system
    TextDX      dxFont;                 // DirectX font
    float       x,y;                    // screen location
    UINT        height;                 // dialog height, calculated in print()
    UINT        width;                  // dialog width
    std::string text;                   // dialog text
    RECT        textRect;               // text rectangle
    RECT        buttonRect;             // button rectangle
    RECT        button2Rect;            // button2 rectangle
    COLOR_ARGB  fontColor;              // font color (a,r,g,b)
    COLOR_ARGB  borderColor;            // border color (a,r,g,b)
    COLOR_ARGB  backColor;              // background color (a,r,g,b)
    COLOR_ARGB  buttonColor;            // button color
    COLOR_ARGB  buttonFontColor;        // button font color
    VertexC vtx[4];                     // vertex data
    LP_VERTEXBUFFER dialogVerts;        // dialog vertex buffer
    LP_VERTEXBUFFER borderVerts;        // border vertex buffer
    LP_VERTEXBUFFER buttonVerts;        // button vertex buffer
    LP_VERTEXBUFFER button2Verts;       // button2 vertex buffer
    int buttonClicked;                  // which button was clicked (1 or 2)
    int buttonType;                     // 0 = OK/Cancel, 1 = Yes/No
    bool    initialized;                // true when initialized successfully
    bool    visible;                    // true to display
    HWND    hwnd;                       // handle to window
    float   screenRatioX, screenRatioY;

public:
    // Constructor
    MessageDialog();
    // Destructor
    virtual ~MessageDialog();

    // Initialize the MessageDialog.
    // Pre: *g points to Graphics object
    //      *in points to Input object
    //      hwnd = window handle
    bool initialize(Graphics *g, Input *in, HWND hwnd);

    // Prepare vertex buffers
    void prepareVerts();

    // Display the MessageDialog.
    const void draw();

    // Return button clicked
    // 0 = no button clicked
    // 1 is left button, 2 is right button
    int getButtonClicked()  {return buttonClicked;}

    // Return visible.
    bool getVisible() {return visible;}

    // Set font color
    void setFontColor(COLOR_ARGB fc)    {fontColor = fc;}

    // Set border color
    void setBorderColor(COLOR_ARGB bc)  {borderColor = bc;}

    // Set background color
    void setBackColor(COLOR_ARGB bc)    {backColor = bc;}

    // Set button color
    void setButtonColor(COLOR_ARGB bc)  {buttonColor = bc;}

    // Set button font color
    void setButtonFontColor(COLOR_ARGB bfc) {buttonFontColor = bfc;}

    // Set visible;
    void setVisible(bool v) {visible = v;}

    // Set button type 0 = OK/CANCEL, 1 = YES/NO
    void setButtonType(UINT t)
    {
        if(t < messageDialogNS::MAX_TYPE)
            buttonType = t;
    }

    // Display text str in MessageDialog
    void print(const std::string &str);

    // Checks for Close event
    void update();

    // Call when graphics device is lost.
    void onLostDevice();

    // Call when graphics device is reset.
    void onResetDevice();
};

#endif

