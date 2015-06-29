// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// dashboard.h v1.1

#ifndef _DASHBOARD_H            // Prevent multiple definitions if this 
#define _DASHBOARD_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

class Dashboard;

#include "image.h"
#include "constants.h"
#include "textureManager.h"

namespace dashboardNS
{
    const int   IMAGE_SIZE = 32;        // each texture size
    const int   TEXTURE_COLS = 8;       // texture columns
    const int   BAR_FRAME = 44;         // the frame number of the bar
}

class Bar : public Image
{
    public:
    // Initialize the Bar 
    // Pre: *graphics = pointer to Graphics object
    //      *textureM = pointer to TextureManager object
    //      left, top = screen location
    //      scale = scaling (zoom) amount
    //      color = color of bar
    bool initialize(Graphics *graphics, TextureManager *textureM, int left, int top,
                    float scale, COLOR_ARGB color);
    // Set Bar Size
    void set(float percentOn);
    // Override update so setRect() is not called.
    virtual void update(float frameTime)    {}
};

#endif

