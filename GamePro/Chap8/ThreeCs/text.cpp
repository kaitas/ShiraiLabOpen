// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// text.cpp v1.0
// Sprite based font.

#include "text.h"

//=============================================================================
// default constructor
//=============================================================================
Text::Text() : Image()
{
    file = NULL;                        // font texture
    graphics = NULL;                    // pointer to graphics
    color = graphicsNS::WHITE;          // default to white font
    backColor = TRANSCOLOR;             // default to transparent (no) fill
    align = textNS::LEFT;
    width = textNS::GRID_WIDTH - 3;     // -2 for transparent border and -1 for divider line
    height = textNS::GRID_HEIGHT - 3;
    fontHeight = textNS::FONT_HEIGHT;
    spriteData.width = 0;
    spriteData.height = 0;
    spriteData.rect.bottom = 1;         // rectangle to select parts of an image
    spriteData.rect.right = 1;
    for(int row=0; row<textNS::ROWS; row++)         // for each row of characters in font
    {
        for(int col=0; col<textNS::COLUMNS; col++)  // for each col of characters in font
        {
            fontData[row][col].left = MAXINT;
            fontData[row][col].right = 0;
        }
    }
    proportional = false;
    proportionalSpacing = textNS::PROPORTIONAL_SPACING;
    underline = false;
    bold = false;
    tabSize = textNS::TAB_SIZE;
}

//=============================================================================
// destructor
//=============================================================================
Text::~Text()
{
    Image::~Image();
}

//=============================================================================
// Initialize the Text.
// Find the left and right edge of each character in the font image 
// Post: returns true if successful, false if failed
//       fontData array contains left and right edge of each character
//=============================================================================
bool Text::initialize(Graphics *g, const char *file)
{
    try {
        graphics = g;           // pointer to graphics object

        //-------------------------------------------------------------
        // Load the font texture and examine it pixel by pixel to find
        // the exact position of each character.
        //-------------------------------------------------------------

        // Load font texture into system memory so it may be locked
        UINT w,h;
        HRESULT result = graphics->loadTextureSystemMem(file, TRANSCOLOR, w, h, textureData);
        if (FAILED(result))
        {
            SAFE_RELEASE(textureData);
            return false;
        }

        // textureData.width & textureData.height contain size of entire font texture
        // Each character has a 1 pixel wide border
        // There are ROWS * COLS characters

        // Lock the font texture, required to access the pixel data
        D3DLOCKED_RECT rect;
        result = textureData->LockRect(0, &rect, NULL, D3DLOCK_READONLY);
        if(FAILED(result))                          // if lock failed
        {
            SAFE_RELEASE(textureData);
            return false;
        }

        for(DWORD row=0; row<textNS::ROWS; row++)   // for each row of characters in font
        {
            for(DWORD col=0; col<textNS::COLUMNS; col++)    // for each col of characters in font
            {
                fontData[row][col].left = MAXINT;    // initialize fontData
                fontData[row][col].right = 0;

                // Process each character pixel by pixel    
                // for y = top pixel; y <= bottom pixel; y++
                for(DWORD y=row*textNS::GRID_HEIGHT+1; y<(row+1)*textNS::GRID_HEIGHT-1; y++)
                {
                    // Get a pointer to the start of this scanline in the texture
                    DWORD* pBits = (DWORD*)((BYTE*)rect.pBits + y*rect.Pitch);
                    // Process this line of pixels
                    for(DWORD x=col*textNS::GRID_WIDTH+1; x<(col+1)*textNS::GRID_WIDTH-1; x++)
                    {
                        // Get this pixel
                        DWORD dwPixel = pBits[x];

                        // If the alpha is not transparent
                        if((dwPixel & 0xff000000) != 0x00)
                        {
                            if(x < fontData[row][col].left)     // if this pixel is more left
                                fontData[row][col].left = x;    // save as left edge of character
                            if(x > fontData[row][col].right)    // if this pixel is more right
                                fontData[row][col].right = x;   // save right edge of character
                        }
                    }
                }
            }
        }

        // Done with the texture, so unlock it
        textureData->UnlockRect(0);
    
        // release this font texture, we just needed it to get font spacing
        SAFE_RELEASE(textureData);    

        //-------------------------------------------------------------
        // load the font image into a texture manager for use
        //-------------------------------------------------------------
        if (!fontTexture.initialize(graphics, file))
            return false;                   // if error loading font texture
        // prepare the font image
        if (!Image::initialize(graphics,textNS::FONT_WIDTH,textNS::FONT_HEIGHT,0,&fontTexture))
            return false;                   // if failed
    }
    catch(...)
    {
        return false;
    }
    return true;                    // successful
}

//=============================================================================
// setXY
// set x,y for text output
//=============================================================================
void Text::setXY(int x2, int y2)
{
    if (x2 < 0 || y2 < 0)
        return;
    spriteData.x = (float)x2;
    spriteData.y = (float)y2;
}

//=============================================================================
// print string at current x,y
// pre: spriteBegin()
// post: spriteEnd()
//=============================================================================
void Text::print(const std::string &str)
{
    print(str, (int)spriteData.x, (int)spriteData.y);
}

//=============================================================================
// print with align at x,y
// text is aligned as specified from:
// LEFT, RIGHT, CENTER, CENTER_MIDDLE, CENTER_BOTTOM, LEFT_BOTTOM, RIGHT_BOTTOM
// pre: spriteBegin()
// post: spriteEnd()
//=============================================================================
void Text::print(const std::string &str, int x, int y, textNS::Alignment al)
{
    align = al;             // save alignment
    print(str,x,y);
    align = textNS::LEFT;
}

//=============================================================================
// Print string at x,y
// pre: spriteBegin()
// post: spriteEnd()
//=============================================================================
void Text::print(const std::string &str, int x, int y)
{
    UCHAR ch = 0, chN = 0;
    std::string str2;
    width = textNS::FONT_WIDTH;
    int scaledWidth = static_cast<int>(textNS::FONT_WIDTH*spriteData.scale);
    float saveY=0;
    int tabX=0, tabW=0;
    
    spriteData.x = (float)x;
    spriteData.y = (float)y;
    doAlign(str);

    for(UINT i=0; i<str.length(); i++)
    {
        ch = str.at(i);
        if (ch > textNS::MIN_CHAR && ch <= textNS::MAX_CHAR)    // if displayable character
        {
            chN = ch - textNS::MIN_CHAR;                // make min_char index 0
            spriteData.rect.top = chN / textNS::COLUMNS * textNS::GRID_HEIGHT + 1;
            spriteData.rect.bottom = spriteData.rect.top + textNS::FONT_HEIGHT;
            if(proportional)
            {
                spriteData.rect.left = fontData[chN/textNS::COLUMNS][chN % textNS::COLUMNS].left;
                // DirectX wants right + 1
                spriteData.rect.right = fontData[chN/textNS::COLUMNS][chN % textNS::COLUMNS].right + 1;
                width = spriteData.rect.right - spriteData.rect.left;
                if(width >= textNS::FONT_WIDTH)         // if full width character do not add spacing
                {
                    width = textNS::FONT_WIDTH;         // limit width
                    spriteData.rect.left = chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
                    spriteData.rect.right = spriteData.rect.left + textNS::FONT_WIDTH;
                }
                else    // not full width so add spacing between characters
                    width += proportionalSpacing;
                scaledWidth = static_cast<int>(width*spriteData.scale);
                drawChar(ch);
            }
            else    // fixed pitch
            {
                width = textNS::FONT_WIDTH;
                spriteData.rect.left = chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
                spriteData.rect.right = spriteData.rect.left + textNS::FONT_WIDTH;
                drawChar(ch);
            }
            spriteData.x += scaledWidth;
        } 
        else    // else, non displayable character
        {
            switch(ch)
            {
            case ' ':                            // space
                if(proportional)
                {
                    width = textNS::FONT_WIDTH/2;
                    scaledWidth = static_cast<int>(width*spriteData.scale);
                }
                drawChar(' ');
                spriteData.x += scaledWidth;
                break;
            // newline advances 1 line down and sets left edge to starting x screen position,
            // not left edge of screen
            case '\n':                            // newline
                spriteData.x = (float)x;
                spriteData.y += static_cast<int>(height*spriteData.scale);
                saveY = spriteData.y;
                str2 = str.substr(i,str.length());
                doAlign(str2);
                spriteData.y = saveY;
                break;
            case '\r':                            // return to starting x position
                spriteData.x = (float)x;
                str2 = str.substr(i,str.length());
                doAlign(str2);
                break;
            case '\t':                            // tab
                width = textNS::FONT_WIDTH;
                scaledWidth = static_cast<int>(width*spriteData.scale);
                tabX = static_cast<int>(spriteData.x) / (scaledWidth * tabSize);
                tabX = (tabX+1) * scaledWidth * tabSize;
                tabW = tabX - static_cast<int>(spriteData.x);
                while(tabW > 0)
                {
                    if(tabW >= scaledWidth)
                    {
                        drawChar(' ');
                        spriteData.x += scaledWidth;
                    }
                    else
                    {
                        width = tabW;        // fractional part of character to align with tab stop
                        drawChar(' ');
                        spriteData.x += tabW;
                    }
                    tabW -= scaledWidth;
                }
                break;
            case '\b':                            // backspace
                spriteData.x -= scaledWidth;
                if(spriteData.x < 0)
                    spriteData.x = 0;
                break;
            case '\v':                            // vertical tab
                spriteData.y += static_cast<int>(height*spriteData.scale);
                break;
            case 0x01:                            // font signature character
                spriteData.rect.top = 1;
                spriteData.rect.bottom = 1 + textNS::FONT_HEIGHT;
                spriteData.rect.left = 1;
                spriteData.rect.right = 1 + textNS::FONT_WIDTH;
                draw(spriteData);
                spriteData.x += scaledWidth;
                break;
            }
        }
    }
    return;
}

//=============================================================================
// Set spriteData.x,spriteData.y for current string and alignment.
// The default alignment is LEFT.
//=============================================================================
void Text::doAlign(const std::string &str)
{
    if(spriteData.texture == NULL)  // if no texture
        return;

    UINT w, h;
    switch(align) {
    case textNS::CENTER:            // center at x and align top to y
        getWidthHeight(str,w,h);
        spriteData.x -= w/2;
        break;
    case textNS::RIGHT:             // right justify at x,y
        getWidthHeight(str,w,h);
        spriteData.x -= w;
        break;
    case textNS::CENTER_MIDDLE:     // center at x and vertical middle to y
        getWidthHeight(str,w,h);
        spriteData.x -= w/2;
        spriteData.y -= h/2;
        break;
    case textNS::CENTER_BOTTOM:     // center at x and align bottom to y
        getWidthHeight(str,w,h);
        spriteData.x -= w/2;
        spriteData.y -= h;
        break;
    case textNS::LEFT_BOTTOM:       // left justify at x and align bottom to y
        getWidthHeight(str,w,h);
        spriteData.y -= h;
        break;
    case textNS::RIGHT_BOTTOM:      // right justify at x and align bottom to y
        getWidthHeight(str,w,h);
        spriteData.x -= w;
        spriteData.y -= h;
        break;
    }
}

//=============================================================================
// getWidthHeight
// Determines width and height of string in pixels for current font size.
// Does not display the string
//=============================================================================
void Text::getWidthHeight(const std::string &str, UINT &w, UINT &h)
{
    if (spriteData.texture == NULL)         // if no texture
        return;

    UCHAR ch = 0, chN = 0;
    width = textNS::FONT_WIDTH;
    int scaledWidth = static_cast<int>(width*spriteData.scale);
    int strW = 0;
    h = 0;
    int stringWidth = 0;

    for(UINT i=0; i<str.length(); i++)
    {
        ch = str.at(i);
        // if displayable character
        if (ch > textNS::MIN_CHAR && ch <= textNS::MAX_CHAR)
        {
            chN = ch - textNS::MIN_CHAR;    // make min_char index 0
            if(proportional)
            {
                spriteData.rect.left = fontData[chN/textNS::COLUMNS][chN % textNS::COLUMNS].left;
                // +1 for DirectX sprite width
                spriteData.rect.right = fontData[chN/textNS::COLUMNS][chN % textNS::COLUMNS].right + 1;
                width = spriteData.rect.right - spriteData.rect.left + proportionalSpacing;
                scaledWidth = static_cast<int>(width*spriteData.scale);
            }
            else    // fixed pitch
            {
                width = textNS::FONT_WIDTH;
                spriteData.rect.left = chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
                spriteData.rect.right = spriteData.rect.left + textNS::FONT_WIDTH;
            }
            stringWidth += scaledWidth;
        } 
        else    // else, non displayable character
        {
            switch(ch)
            {
            case ' ':   // space
                if(proportional)
                {
                    width = (textNS::FONT_WIDTH)/2;
                    scaledWidth = static_cast<int>(width*spriteData.scale);
                }
                stringWidth += scaledWidth;
                break;
            case '\n':  // newline
                if(strW == 0)
                    strW = stringWidth;
                stringWidth = 0;
                h += static_cast<int>(height*spriteData.scale);
                break;
            case '\r':  // return
                if(strW == 0)
                    strW = stringWidth;
                stringWidth = 0;
                break;
            case '\t':  // tab
                {
                    width = textNS::FONT_WIDTH;
                    scaledWidth = static_cast<int>(width*spriteData.scale);
                    int tabX = static_cast<int>(spriteData.x) / (scaledWidth * tabSize);
                    tabX = (tabX+1) * scaledWidth * tabSize;
                    int tabW = tabX - static_cast<int>(spriteData.x);
                    while(tabW > 0)
                    {
                        if(tabW >= scaledWidth)
                            stringWidth += scaledWidth;
                        else
                        {
                            // fractional part of character to align with tab stop
                            width = tabW;
                            stringWidth += tabW;
                        }
                        tabW -= scaledWidth;
                    }
                }
                break;
            case '\b':      // backspace
                stringWidth -= scaledWidth;
                if(stringWidth < 0)
                    stringWidth = 0;
                break;
            case 0x01:      // special
                stringWidth += scaledWidth;
                break;
            }
        }
    }
    if(strW==0)
        strW = stringWidth;
    w = strW;
    return;
}

//=============================================================================
// drawChar
// Display character sprite described by spriteData using color and fill
// Does underline and bold
//=============================================================================
void Text::drawChar(UCHAR ch)
{
    SpriteData sd2 = spriteData;    // copy sprite data

    // display backColor color
    if(backColor != TRANSCOLOR)     // if backColor is not transparent
    {
        spriteData.rect.top = (textNS::SOLID-textNS::MIN_CHAR) / textNS::COLUMNS * textNS::GRID_HEIGHT + 1;
        spriteData.rect.bottom = spriteData.rect.top + textNS::GRID_HEIGHT - 2;
        spriteData.rect.left = (textNS::SOLID-textNS::MIN_CHAR) % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
        spriteData.rect.right = spriteData.rect.left + width;
        draw(backColor);        // draw backColor
        spriteData.rect = sd2.rect;     // restore character rect
    }

    // display underline
    if(underline)
    {
        spriteData.rect.top = (textNS::UNDERLINE-textNS::MIN_CHAR) / textNS::COLUMNS * textNS::GRID_HEIGHT + 1;
        spriteData.rect.bottom = spriteData.rect.top + textNS::GRID_HEIGHT - 2;
        spriteData.rect.left = (textNS::UNDERLINE-textNS::MIN_CHAR) % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
        spriteData.rect.right = spriteData.rect.left + width;
        draw(color);
        spriteData.rect = sd2.rect;     // restore character rect
    }

    // display character
    if(ch > textNS::MIN_CHAR && ch <= textNS::MAX_CHAR) // if displayable character
    {
        draw(spriteData, color);
        if (bold)   // bold is done by displaying the character twice with offset x
        {
            spriteData.x += textNS::BOLD_SIZE*spriteData.scale; 
            draw(spriteData, color);
            spriteData.x = sd2.x;
        }
    }
}


//=============================================================================
// called when graphics device is lost
//=============================================================================
void Text::onLostDevice()
{
    try{
        if (!initialized)
            return;
        fontTexture.onLostDevice();
    } catch(...){}
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void Text::onResetDevice()
{
    try{
        if (!initialized)
            return;
        fontTexture.onResetDevice();
    } catch(...){}
}

