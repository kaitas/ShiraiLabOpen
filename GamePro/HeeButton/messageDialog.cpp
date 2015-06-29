// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// messageDialog.cpp v1.2

#include "messageDialog.h"

//=============================================================================
// Constructor
//=============================================================================
MessageDialog::MessageDialog()
{
    initialized = false;                // set true when successfully initialized
    graphics = NULL;
    visible = false;                    // not visible
    fontColor = messageDialogNS::FONT_COLOR;
    borderColor = messageDialogNS::BORDER_COLOR;
    backColor = messageDialogNS::BACK_COLOR;
    buttonColor = messageDialogNS::BUTTON_COLOR;
    buttonFontColor = messageDialogNS::BUTTON_FONT_COLOR;
    x = static_cast<float>(messageDialogNS::X); // starting position
    y = static_cast<float>(messageDialogNS::Y);
    height = messageDialogNS::HEIGHT;
    width = messageDialogNS::WIDTH;
    textRect.bottom = messageDialogNS::Y + messageDialogNS::HEIGHT - messageDialogNS::MARGIN;
    textRect.left = messageDialogNS::X + messageDialogNS::MARGIN;
    textRect.right = messageDialogNS::X + messageDialogNS::WIDTH - messageDialogNS::MARGIN;
    textRect.top = messageDialogNS::Y + messageDialogNS::MARGIN;
    dialogVerts = NULL;
    borderVerts = NULL;
    buttonVerts = NULL;
    button2Verts = NULL;
    buttonType = 0;     // OK/Cancel
}

//=============================================================================
// Destructor
//=============================================================================
MessageDialog::~MessageDialog()
{
    onLostDevice();            // call onLostDevice() for every graphics item
}

//=============================================================================
// Initialize the MessageDialog
//=============================================================================
bool MessageDialog::initialize(Graphics *g, Input *in, HWND h)
{
    try {
        graphics = g;                   // the graphics object
        input = in;                     // the input object
        hwnd = h;

        // initialize DirectX font
        if(dxFont.initialize(graphics, messageDialogNS::FONT_HEIGHT, false, 
                             false, messageDialogNS::FONT) == false)
            return false;               // if failed
        dxFont.setFontColor(fontColor);
    } catch(...) {
        return false;
    }

    initialized = true;
    return true;
}

//=============================================================================
// Prepare the vertex buffers for drawing dialog background and buttons
//=============================================================================
void MessageDialog::prepareVerts()
{
    safeRelease(dialogVerts);
    safeRelease(borderVerts);
    safeRelease(buttonVerts);
    safeRelease(button2Verts);

    // border top left
    vtx[0].x = x;
    vtx[0].y = y;
    vtx[0].z = 0.0f;
    vtx[0].rhw = 1.0f;
    vtx[0].color = borderColor;

    // border top right
    vtx[1].x = x + width;
    vtx[1].y = y;
    vtx[1].z = 0.0f;
    vtx[1].rhw = 1.0f;
    vtx[1].color = borderColor;

    // border bottom right
    vtx[2].x = x + width;
    vtx[2].y = y + height;
    vtx[2].z = 0.0f;
    vtx[2].rhw = 1.0f;
    vtx[2].color = borderColor;

    // border bottom left
    vtx[3].x = x;
    vtx[3].y = y + height;
    vtx[3].z = 0.0f;
    vtx[3].rhw = 1.0f;
    vtx[3].color = borderColor;

    graphics->createVertexBuffer(vtx, sizeof vtx, borderVerts);

    // background top left
    vtx[0].x = x + messageDialogNS::BORDER;
    vtx[0].y = y + messageDialogNS::BORDER;
    vtx[0].z = 0.0f;
    vtx[0].rhw = 1.0f;
    vtx[0].color = backColor;

    // background top right
    vtx[1].x = x + width - messageDialogNS::BORDER;
    vtx[1].y = y + messageDialogNS::BORDER;
    vtx[1].z = 0.0f;
    vtx[1].rhw = 1.0f;
    vtx[1].color = backColor;

    // background bottom right
    vtx[2].x = x + width - messageDialogNS::BORDER;
    vtx[2].y = y + height - messageDialogNS::BORDER;
    vtx[2].z = 0.0f;
    vtx[2].rhw = 1.0f;
    vtx[2].color = backColor;

    // background bottom left
    vtx[3].x = x + messageDialogNS::BORDER;
    vtx[3].y = y + height - messageDialogNS::BORDER;
    vtx[3].z = 0.0f;
    vtx[3].rhw = 1.0f;
    vtx[3].color = backColor;

    graphics->createVertexBuffer(vtx, sizeof vtx, dialogVerts);

    // button top left
    vtx[0].x = x + width/2.0f - messageDialogNS::BUTTON_WIDTH/2.0f;
    vtx[0].y = y + height - messageDialogNS::BORDER - messageDialogNS::MARGIN - messageDialogNS::BUTTON_HEIGHT;
    vtx[0].z = 0.0f;
    vtx[0].rhw = 1.0f;
    vtx[0].color = buttonColor;

    // button top right
    vtx[1].x = x + width/2.0f + messageDialogNS::BUTTON_WIDTH/2.0f;
    vtx[1].y = vtx[0].y;
    vtx[1].z = 0.0f;
    vtx[1].rhw = 1.0f;
    vtx[1].color = buttonColor;

    // button bottom right
    vtx[2].x =  vtx[1].x;
    vtx[2].y = vtx[0].y + messageDialogNS::BUTTON_HEIGHT;
    vtx[2].z = 0.0f;
    vtx[2].rhw = 1.0f;
    vtx[2].color = buttonColor;

    // button bottom left
    vtx[3].x = vtx[0].x;
    vtx[3].y = vtx[2].y;
    vtx[3].z = 0.0f;
    vtx[3].rhw = 1.0f;
    vtx[3].color = buttonColor;

    graphics->createVertexBuffer(vtx, sizeof vtx, buttonVerts);

    // set buttonRect
    buttonRect.left   = (long)vtx[0].x;
    buttonRect.right  = (long)vtx[1].x;
    buttonRect.top    = (long)vtx[0].y;
    buttonRect.bottom = (long)vtx[2].y;

    // button2 top left
    vtx[0].x = x + width - messageDialogNS::BUTTON_WIDTH*1.2f;
    vtx[0].y = y + height - messageDialogNS::BORDER - messageDialogNS::MARGIN - messageDialogNS::BUTTON_HEIGHT;
    vtx[0].z = 0.0f;
    vtx[0].rhw = 1.0f;
    vtx[0].color = buttonColor;
    // button2 top right
    vtx[1].x = vtx[0].x + messageDialogNS::BUTTON_WIDTH;
    vtx[1].y = vtx[0].y;
    vtx[1].z = 0.0f;
    vtx[1].rhw = 1.0f;
    vtx[1].color = buttonColor;
    // button2 bottom right
    vtx[2].x =  vtx[1].x;
    vtx[2].y = vtx[0].y + messageDialogNS::BUTTON_HEIGHT;
    vtx[2].z = 0.0f;
    vtx[2].rhw = 1.0f;
    vtx[2].color = buttonColor;
    // button2 bottom left
    vtx[3].x = vtx[0].x;
    vtx[3].y = vtx[2].y;
    vtx[3].z = 0.0f;
    vtx[3].rhw = 1.0f;
    vtx[3].color = buttonColor;
    graphics->createVertexBuffer(vtx, sizeof vtx, button2Verts);

    // set button2Rect
    button2Rect.left   = (long)vtx[0].x;
    button2Rect.right  = (long)vtx[1].x;
    button2Rect.top    = (long)vtx[0].y;
    button2Rect.bottom = (long)vtx[2].y;
}

//=============================================================================
// Draw the MessageDialog
//=============================================================================
const void MessageDialog::draw()
{
    if (!visible || graphics == NULL || !initialized)
        return;

    graphics->drawQuad(borderVerts);        // draw border
    graphics->drawQuad(dialogVerts);        // draw backdrop
    graphics->drawQuad(buttonVerts);        // draw button
    graphics->drawQuad(button2Verts);       // draw button2

    graphics->spriteBegin();                // begin drawing sprites

    if(text.size() == 0)
        return;
    // display text on MessageDialog
    dxFont.setFontColor(fontColor);
    dxFont.print(text,textRect,DT_CENTER|DT_WORDBREAK);

    // display text on buttons
    dxFont.setFontColor(buttonFontColor);
    dxFont.print(messageDialogNS::BUTTON1_TEXT[buttonType],buttonRect,
                 DT_SINGLELINE|DT_CENTER|DT_VCENTER);
    dxFont.print(messageDialogNS::BUTTON2_TEXT[buttonType],button2Rect,
                 DT_SINGLELINE|DT_CENTER|DT_VCENTER);

    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// Checks for DIALOG_CLOSE_KEY and mouse click on OK button
//=============================================================================
void MessageDialog::update()
{
    if (!initialized || !visible)
        return;
    if (input->wasKeyPressed(messageDialogNS::DIALOG_CLOSE_KEY))
    {
        visible = false;
        buttonClicked = 1;              // button1 was clicked
        return;
    }

    if (graphics->getFullscreen() == false) // if windowed
    {
        // calculate screen ratios incase window was resized
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        screenRatioX = (float)GAME_WIDTH / clientRect.right;
        screenRatioY = (float)GAME_HEIGHT / clientRect.bottom;
    }

    if (input->getMouseLButton())       // if mouse left button
    {
        // if mouse clicked inside button1 (OK)
        if (input->getMouseX()*screenRatioX >= buttonRect.left &&
            input->getMouseX()*screenRatioX <= buttonRect.right &&
            input->getMouseY()*screenRatioY >= buttonRect.top &&
            input->getMouseY()*screenRatioY <= buttonRect.bottom)
        {
            visible = false;            // hide message dialog
            buttonClicked = 1;          // button1 was clicked
            return;
        }

        // if mouse clicked inside button2 (cancel)
        if (input->getMouseX()*screenRatioX >= button2Rect.left &&
            input->getMouseX()*screenRatioX <= button2Rect.right &&
            input->getMouseY()*screenRatioY >= button2Rect.top &&
            input->getMouseY()*screenRatioY <= button2Rect.bottom)
        {
            visible = false;            // hide message dialog
            buttonClicked = 2;          // button2 was clicked
        }
    }
}

//=============================================================================
// Set text string, size dialog bottom to fit text and set visible = true
//=============================================================================
void MessageDialog::print(const std::string &str)         
{
    if (!initialized || visible)    // if not initialized or already in use
        return;
    text = str + "\n\n\n\n";        // leave some room for buttons

    // Set textRect to text area of dialog
    textRect.left   = (long)(x + messageDialogNS::MARGIN);
    textRect.right  = (long)(x + messageDialogNS::WIDTH - messageDialogNS::MARGIN);
    textRect.top    = (long)(y + messageDialogNS::MARGIN);
    textRect.bottom = (long)(y + messageDialogNS::HEIGHT - messageDialogNS::MARGIN);

    // Set textRect.bottom to precise height required for text
    // No text is printed with DT_CALDRECT option.
    dxFont.print(text,textRect,DT_CENTER|DT_WORDBREAK|DT_CALCRECT);
    height = textRect.bottom - (int)y + messageDialogNS::BORDER + messageDialogNS::MARGIN;

    prepareVerts();                 // prepare the vertex buffers
    buttonClicked = 0;              // clear buttonClicked
    visible = true;
}

//=============================================================================
// Called when graphics device is lost
//=============================================================================
void MessageDialog::onLostDevice()
{
    if (!initialized)
        return;
    dxFont.onLostDevice();
    safeRelease(dialogVerts);
    safeRelease(borderVerts);
    safeRelease(buttonVerts);
    safeRelease(button2Verts);
}

//=============================================================================
// Called when graphics device is reset
//=============================================================================
void MessageDialog::onResetDevice()
{
    if (!initialized)
        return;
    prepareVerts();
    dxFont.onResetDevice();
}

