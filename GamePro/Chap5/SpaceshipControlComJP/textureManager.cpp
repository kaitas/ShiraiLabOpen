// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// textureManager.cpp v1.0
// A TextureManager object loads and maintains one texture file. 
// Create a TextureManager object for each texture file in the game.

#include "textureManager.h"

//=============================================================================
// default constructor
//=============================================================================
TextureManager::TextureManager()
{
    texture = NULL;
    width = 0;
    height = 0;
    file = NULL;
    graphics = NULL;
    initialized = false;            // set true when successfully initialized
}

//=============================================================================
// destructor
//=============================================================================
TextureManager::~TextureManager()
{
    SAFE_RELEASE(texture);
}

//=============================================================================
// Loads the texture file from disk.
// テクスチャファイルをディスクから読み込む
// Post: returns true if successful, false if failed
// 実行後：成功した場合はTrue、失敗した場合はfalseを戻す				P147
//=============================================================================
bool TextureManager::initialize(Graphics *g, const char *f)
{
    try{
        graphics = g;                       // the graphics object	// graphicsオブジェクト
        file = f;                           // the texture file		// テクスチャファイル

        hr = graphics->loadTexture(file, TRANSCOLOR, width, height, texture);
        if (FAILED(hr))
        {
            SAFE_RELEASE(texture);
            return false;
        }
    }
    catch(...) {return false;}
    initialized = true;                    // set true when successfully initialized	// 正常に初期化された場合trueを設定
    return true;
}

//=============================================================================
// called when graphics device is lost
// グラフィックスデバイスが消失した場合に呼び出される
//=============================================================================
void TextureManager::onLostDevice()
{
    if (!initialized)
        return;
    SAFE_RELEASE(texture);
}

//=============================================================================
// called when graphics device is reset
// グラフィックスデバイスがリセットされた場合に呼び出される
//=============================================================================
void TextureManager::onResetDevice()
{
    if (!initialized)
        return;
    graphics->loadTexture(file, TRANSCOLOR, width, height, texture);
}


