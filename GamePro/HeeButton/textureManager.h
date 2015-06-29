// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// textureManager.h v2.0
// Last modification: Feb-18-2013
// v2.0 supports multiple texture files.

#ifndef _TEXTUREMANAGER_H       // Prevent multiple definitions if this 
#define _TEXTUREMANAGER_H       // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

class TextureManager;

#include <vector>
#include <string>
#include <fstream>
#include "graphics.h"
#include "constants.h"

class TextureManager
{
    // TextureManager properties
  private:
    std::vector<UINT>       width;      // width of texture in pixels
    std::vector<UINT>       height;     // height of texture in pixels
    std::vector<LP_TEXTURE> texture;    // pointer to textures
    std::vector<std::string> fileNames; // texture file names
    Graphics *graphics;     // save pointer to graphics
    bool    initialized;    // true when successfully initialized
    HRESULT hr;             // standard return type

  public:
    // Constructor
    TextureManager();

    // Destructor
    virtual ~TextureManager();

    // Returns a pointer to texture n
    LP_TEXTURE getTexture(UINT n=0) const 
    {
        if(n >= texture.size())
            return NULL;
        return texture[n];
    }

    // Returns the width of texture n
    UINT getWidth(UINT n=0) const 
    {
        if(n >= texture.size())
            return 0;
        return width[n];
    }

    // Return the height of texture n
    UINT getHeight(UINT n=0) const
    {
        if(n >= texture.size())
            return 0;
        return height[n];
    }

    // Initialize the textureManager
    // Pre: *g points to Graphics object
    //      file is name of texture file to load.
    //      If file extension is .txt it is assumed to
    //      contain a list of texture file names, one name
    //      per row.
    // Post: The texture file(s) are loaded
    virtual bool initialize(Graphics *g, std::string file);

    // Release resources, all texture memory is released.
    virtual void onLostDevice();

    // Restore resourses, all textures are reloaded.
    virtual void onResetDevice();

    // Safely release texture
    void safeReleaseTexture(LP_TEXTURE& ptr)
    {
        if (ptr)
        { 
            ptr->Release(); 
            ptr = NULL;
        }
    }
};

#endif

