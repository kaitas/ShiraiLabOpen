// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// text.h v1.0
// Sprite based font

#ifndef _text_h                 // Prevent multiple definitions if this 
#define _text_h                 // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "image.h"
#include "constants.h"


namespace textNS
{
    enum Alignment{LEFT, RIGHT, CENTER, CENTER_MIDDLE, CENTER_BOTTOM, LEFT_BOTTOM, RIGHT_BOTTOM};
    struct FontData
    {
        UINT left;
        UINT right;
    };
    // Actual font width is 48, font height is 62
    const int FONT_BORDER = 3;      // 1 pixel wide transparent border + visible cell border on right and bottom
    const int FONT_WIDTH  = 48;
    const int FONT_HEIGHT = 62;     // font is 62 pixels high
    const int GRID_WIDTH  = FONT_WIDTH + FONT_BORDER;    
    const int GRID_HEIGHT = FONT_HEIGHT + FONT_BORDER;
    const int COLUMNS = 16;         // number of columns in font image
    const int ROWS = 14;            // number of rows in font image
    const int FRAMES = 1;           // how many frames of animation (1 = not animated)
    const double ANIM_DELAY = 0.0;  // delay between animation frames
    const int MAX_FONT_HEIGHT = 1000;
    const int MIN_CHAR = 0x0020;    // minimum character code
    const int MAX_CHAR = 0x00FF;    // maximum character code
    const int PROPORTIONAL_SPACING = 5; // pixels between 1:1 scaled characters
    const int TAB_SIZE = 8;
    const char UNDERLINE = '_';
    const char SOLID = 0x7F;        // solid block used for fill character ASCII $7F
    const int BOLD_SIZE = 4;        // pixel shift for bold display
}


class Text : public Image
{
private:
    TextureManager fontTexture;     // a texture for each font
    LP_TEXTURE textureData;         // temp font texture
    char    *file;                  // name of texture file
    Graphics *graphics;             // save pointer to graphics
    UINT width, height;             // width & height of 1 character
    textNS::FontData fontData[textNS::ROWS][textNS::COLUMNS]; // left & right edge of each character in pixels
    COLOR_ARGB color;               // font color (a,r,g,b)
    COLOR_ARGB backColor;           // background color (a,r,g,b)
    UINT  fontHeight;               // font height in pixels
    UINT  tabSize;                  // character spacing for tab
    UINT  proportionalSpacing;      // spacing in pixels between proportional characters
    bool proportional;              // true for proportional spacing
    bool underline;
    bool bold;
    textNS::Alignment align;        // how is text aligned (center, left, etc)

public:
    // default constructor (sprite text)
    Text();

    // destructor
    virtual ~Text();

    //=============================================================
    // inherited member functions
    //=============================================================

    // Initialize font using file texture image.
    virtual bool initialize(Graphics *g, const char *file);
    // disable inherited update()
    virtual void update(float frameTime) {};
    virtual void onLostDevice();
    virtual void onResetDevice();

    //=============================================================
    // new member functions
    //=============================================================

    // Position display point at x,y
    virtual void setXY(int x, int y);

    // Print at current x,y. Call between spriteBegin()/spriteEnd()
    virtual void print(const std::string &str);

    // Print at x,y. Call between spriteBegin()/spriteEnd()
    virtual void print(const std::string &str, int x, int y);

    // Print at x,y using align. Call between spriteBegin()/spriteEnd()
    virtual void print(const std::string &str, int x, int y, textNS::Alignment align);

    // Determines width and height of string in pixels for current point size.
    virtual void getWidthHeight(const std::string &str, UINT &width, UINT &height);

    // Set the font color. Use SETCOLOR_ARGB macro or colors from graphicsNS::
    virtual void setFontColor(COLOR_ARGB c) {color = c;}

    // Returns font color
    virtual COLOR_ARGB getFontColor() {return color;}

    // Set background color
    virtual void setBackColor(COLOR_ARGB bc) {backColor = bc;}

    // Returns background color
    virtual COLOR_ARGB getBackColor() {return backColor ;}

    // Set height of font in pixels
    virtual void setFontHeight(UINT height)
    {
        if(height == 0 || height > textNS::MAX_FONT_HEIGHT)  // if invalid size
            return;
        fontHeight = height;
        spriteData.scale = (float)height/(float)textNS::FONT_HEIGHT;
    }

    // Returns height of font
    virtual UINT getFontHeight() {return fontHeight;}

    // Set proportional spacing true/false
    virtual void setProportional(bool p) {proportional = p;}

    // Returns proportional spacing setting
    virtual bool getProportional() {return proportional;}

    // Set the spacing used for proportional spacing
    virtual void setProportionalSpacing(UINT s) {proportionalSpacing = s;}

    // Return proportional spacing size
    virtual UINT getProportionalSpacing() {return proportionalSpacing;}

    // Set underline true/false
    virtual void setUnderline(bool u) {underline = u;}

    // Returns underline setting
    virtual bool getUnderline() {return underline;}

    // Set bold true/false
    virtual void setBold(bool b) {bold = b;}

    // Returns bold setting
    virtual bool getBold() {return bold;}

    // Set tab size
    virtual void setTabSize(UINT size)  
    {
        if(size == 0)
            return;
        tabSize = size;
    }

    // Return tab size
    virtual UINT getTabSize() {return tabSize;}

    // Set spriteData.x,spriteData.y for current string and alignment.
    // The default alignment is LEFT.
    virtual void doAlign(const std::string &str);

    // Display character sprite described by spriteData using color and fill
    // Does underline and bold
    virtual void drawChar(UCHAR ch);
};

#endif

