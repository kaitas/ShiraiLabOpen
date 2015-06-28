// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// inputDialog.cpp v1.1

#include "inputDialog.h"

//=============================================================================
// Constructor
//=============================================================================
InputDialog::InputDialog()
{
    textBackColor = inputDialogNS::TEXT_BACK_COLOR;
    textFontColor = inputDialogNS::TEXT_COLOR;
    inTextVerts = NULL;
    inText = "";
}

//=============================================================================
// Destructor
//=============================================================================
InputDialog::~InputDialog()
{
    onLostDevice();            // call onLostDevice() for every graphics item
}

//=============================================================================
// Prepare the vertex buffers for drawing dialog background and button
//=============================================================================
void InputDialog::prepareVerts()
{
    MessageDialog::prepareVerts();  // call perpareVerts in base class
    safeRelease(inTextVerts);

    // inText top left
    vtx[0].x = x + messageDialogNS::BORDER*2;
    vtx[0].y = y + height - messageDialogNS::BORDER - messageDialogNS::MARGIN - messageDialogNS::BUTTON_HEIGHT*2.5f;
    vtx[0].z = 0.0f;
    vtx[0].rhw = 1.0f;
    vtx[0].color = textBackColor;
    // inText top right
    vtx[1].x = x + width - messageDialogNS::BORDER*2;
    vtx[1].y = vtx[0].y;
    vtx[1].z = 0.0f;
    vtx[1].rhw = 1.0f;
    vtx[1].color = textBackColor;
    // inText bottom right
    vtx[2].x =  vtx[1].x;
    vtx[2].y = vtx[0].y + messageDialogNS::BUTTON_HEIGHT;
    vtx[2].z = 0.0f;
    vtx[2].rhw = 1.0f;
    vtx[2].color = textBackColor;
    // inText bottom left
    vtx[3].x = vtx[0].x;
    vtx[3].y = vtx[2].y;
    vtx[3].z = 0.0f;
    vtx[3].rhw = 1.0f;
    vtx[3].color = textBackColor;
    graphics->createVertexBuffer(vtx, sizeof vtx, inTextVerts);

    // set inTextRect
    inTextRect.left   = (long)vtx[0].x;
    inTextRect.right  = (long)vtx[1].x;
    inTextRect.top    = (long)vtx[0].y;
    inTextRect.bottom = (long)vtx[2].y;
}

//=============================================================================
// Draw the InputDialog
//=============================================================================
const void InputDialog::draw()
{
    if (!visible || graphics == NULL || !initialized)
        return;

    graphics->drawQuad(borderVerts);        // draw border
    graphics->drawQuad(dialogVerts);        // draw backdrop
    graphics->drawQuad(buttonVerts);        // draw button
    graphics->drawQuad(button2Verts);       // draw button2
    graphics->drawQuad(inTextVerts);        // draw input text area

    graphics->spriteBegin();                // begin drawing sprites

    if(text.size() == 0)
        return;
    // display text on MessageDialog
    dxFont.setFontColor(fontColor);
    dxFont.print(text,textRect,DT_CENTER|DT_WORDBREAK);

    // display text on buttons
    dxFont.setFontColor(buttonFontColor);
    dxFont.print(messageDialogNS::BUTTON1_TEXT[buttonType],buttonRect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
    dxFont.print(messageDialogNS::BUTTON2_TEXT[buttonType],button2Rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);

    // display input text
    dxFont.setFontColor(textFontColor);
    tempRect = inTextRect;      // save
    // No text is printed with DT_CALDRECT option. It moves RECT.right
    dxFont.print(inText,tempRect,DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_CALCRECT);
    if(tempRect.right > inTextRect.right)   // if text too long, right justify
        dxFont.print(inText,inTextRect,DT_SINGLELINE|DT_RIGHT|DT_VCENTER);
    else    // else, left justify
        dxFont.print(inText,inTextRect,DT_SINGLELINE|DT_LEFT|DT_VCENTER);

    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// Checks for DIALOG_CLOSE_KEY and mouse click on buttons
//=============================================================================
void InputDialog::update()
{
    MessageDialog::update();        // call update in base class
    if (!initialized || !visible)
    {
        if(buttonClicked == 2)      // if Cancel button
            inText = "";            // clear input text
        return;
    }
    inText = input->getTextIn();    // get input text
}

//=============================================================================
// Set text string, size dialog bottom to fit text and set visible = true
//=============================================================================
void InputDialog::print(const std::string &str)         
{
    if (!initialized || visible)    // if not initialized or already in use
        return;
    text = str + "\n\n\n\n\n";   // leave some room for input text and buttons

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
    inText = "";                    // clear old input
    input->clearTextIn();
    buttonClicked = 0;              // clear buttonClicked
    visible = true;
}

//=============================================================================
// Called when graphics device is lost
//=============================================================================
void InputDialog::onLostDevice()
{
    if (!initialized)
        return;
    MessageDialog::onLostDevice();
    safeRelease(inTextVerts);
}

