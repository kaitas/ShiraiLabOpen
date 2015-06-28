// Programming 2D Games
// Copyright (c) 2011, 2013 by: 
// Charles Kelly
// textureManager.cpp v2.1
// Last modification: Apr-12-2013
// A TextureManager object loads and maintains texture files. 

#include "textureManager.h"

//=============================================================================
// default constructor
//=============================================================================
TextureManager::TextureManager()
{
    graphics = NULL;
    initialized = false;            // set true when successfully initialized
}

//=============================================================================
// destructor
//=============================================================================
TextureManager::~TextureManager()
{
    for(UINT i=0; i<texture.size(); i++)
        safeReleaseTexture(texture[i]);
}

//=============================================================================
// Loads the texture file(s) from disk.
// Post: returns true if successful, false if failed
//       If file is a .txt file it is assumed to contain individual texture
//       file names, one name per line.
//=============================================================================
bool TextureManager::initialize(Graphics *g, std::string file)
{
    bool success = true;
    try{
        graphics = g;                       // the graphics object
        for(UINT i=0; i<file.size(); i++)    // convert to lowercase
            file.at(i) = tolower(file.at(i));
        if(file.rfind(".txt") == file.size()-4) // if .txt extension
        {
            // open file containing individual texture names
            std::ifstream infile(file.c_str());   
            if(!infile)                     // if open failed
                return false;
            std::string name;
            while(getline(infile,name))
            {
                fileNames.push_back(name);  // add to files
                width.push_back(0);         // make room for width
                height.push_back(0);        // make room for height
                texture.push_back(NULL);    // make room for texture
            }
            infile.close();
        } 
        else    // not .txt file so file contains name of one texture
        {
            fileNames.push_back(file);      // put one file name in files
            width.push_back(0);         // make room for width
            height.push_back(0);        // make room for height
            texture.push_back(NULL);    // make room for texture
        }

        // load texture files
        for(UINT i=0; i<fileNames.size(); i++)
        {
            hr = graphics->loadTexture(fileNames[i].c_str(), 
                 graphicsNS::TRANSCOLOR, width[i], height[i], texture[i]);
            if (FAILED(hr))
                success = false;    // at least one texture failed to load
        }
    }
    catch(...) {return false;}
    initialized = true;                    // set true when initialized
    return success;
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void TextureManager::onLostDevice()
{
    try
    {
        if (!initialized)
            return;
        for(UINT i=0; i<texture.size(); i++)
            safeReleaseTexture(texture[i]);
    }catch(...)
    {
        throw(GameError(gameErrorNS::WARNING, 
            "Warning, TextureManager onLostDevice attempted to access an invalid texture."));
    }
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void TextureManager::onResetDevice()
{
    if (!initialized)
        return;
    // load texture files
    for(UINT i=0; i<fileNames.size(); i++)
    {
        hr = graphics->loadTexture(fileNames[i].c_str(), 
             graphicsNS::TRANSCOLOR, width[i], height[i], texture[i]);
        if (FAILED(hr))
            safeReleaseTexture(texture[i]);
    }
}


