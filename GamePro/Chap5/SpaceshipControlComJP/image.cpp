// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Image.cpp v1.0

#include "image.h"

//=============================================================================
// default constructor
//=============================================================================
Image::Image()
{
    initialized = false;            // set true when successfully initialized
    spriteData.width = 2;
    spriteData.height = 2;
    spriteData.x = 0.0;
    spriteData.y = 0.0;
    spriteData.scale = 1.0;
    spriteData.angle = 0.0;
    spriteData.rect.left = 0;       // used to select one frame from multi-frame image
    spriteData.rect.top = 0;
    spriteData.rect.right = spriteData.width;
    spriteData.rect.bottom = spriteData.height;
    spriteData.texture = NULL;      // the sprite texture (picture)
    spriteData.flipHorizontal = false;
    spriteData.flipVertical = false;
    cols = 1;
    textureManager = NULL;
    startFrame = 0;
    endFrame = 0;
    currentFrame = 0;
    frameDelay = 1.0;               // default to 1 second per frame of animation
    animTimer = 0.0;
    visible = true;                 // the image is visible
    loop = true;                    // loop frames
    animComplete = false;
    graphics = NULL;                // link to graphics system
    colorFilter = graphicsNS::WHITE; // WHITE for no change
}

//=============================================================================
// destructor
//=============================================================================
Image::~Image()
{}

//=============================================================================
// Initialize the Image.
// Imageを初期化
// Post: returns true if successful, false if failed
// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
// pointer to Graphics
// Graphicsへのポインタ
// width of Image in pixels  (0 = use full texture width)
// Imageの幅(ピクセル単位)(0 = テクスチャ全体の幅を使用)
// height of Image in pixels (0 = use full texture height)
// Imageの高さ(ピクセル単位)(0 = テクスチャ全体の高さを使用)
// number of columns in texture (1 to n) (0 same as 1)
// テクスチャ内の列数(1からnまで)(0は1と同じ)
// pointer to TextureManager
// TextureManagerへのポインタ						P149
//=============================================================================
bool Image::initialize(Graphics *g, int width, int height, int ncols,
                       TextureManager *textureM)
{
    try{
        graphics = g;                               // the graphics object			// graphicsオブジェクト
        textureManager = textureM;                  // pointer to texture object	// テクスチャオブジェクトへのポインタ

        spriteData.texture = textureManager->getTexture();
        if(width == 0)
            width = textureManager->getWidth();     // use full width of texture	// 全体の幅を使用	P150
        spriteData.width = width;
        if(height == 0)
            height = textureManager->getHeight();   // use full height of texture	// 全体の高さを使用
        spriteData.height = height;
        cols = ncols;
        if (cols == 0)
            cols = 1;                               // if 0 cols use 1				// colsが0の場合には1を使用

        // configure spriteData.rect to draw currentFrame
		// currentFrameを描画するspriteData.rectを構成
        spriteData.rect.left = (currentFrame % cols) * spriteData.width;
        // right edge + 1
		// 右端 + 1
        spriteData.rect.right = spriteData.rect.left + spriteData.width;
        spriteData.rect.top = (currentFrame / cols) * spriteData.height;
        // bottom edge + 1
		// 下端 + 1
        spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
    }
    catch(...) {return false;}
    initialized = true;                                // successfully initialized	// 正常に初期化された場合
    return true;
}


//=============================================================================
// Draw the image using color as filter
// colorをフィルタとして使って画像を描画
// The color parameter is optional, WHITE is assigned as default in image.h
// colorパラメータはオプション(image.hで、デフォルトとして白が割り当てられている)
// Pre : spriteBegin() is called
// 実行前：spriteBegin()を呼び出す
// Post: spriteEnd() is called
// 実行後：spriteEnd()を呼び出す						P151
//=============================================================================
void Image::draw(COLOR_ARGB color)
{
    if (!visible || graphics == NULL)
        return;
    // get fresh texture incase onReset() was called
	// onReset()が呼び出されたときに新しいテクスチャを取得
    spriteData.texture = textureManager->getTexture();
    if(color == graphicsNS::FILTER)                     // if draw with filter	// フィルタを使って描画する場合
        graphics->drawSprite(spriteData, colorFilter);  // use colorFilter		// colorFilterを使用
    else
        graphics->drawSprite(spriteData, color);        // use color as filter	// フィルタとしてcolorを使用
}

//=============================================================================
// Draw this image using the specified SpriteData.
// 指定されたSpriteDataを使ってこの画像を描画
//   The current SpriteData.rect is used to select the texture.
// 現在のSpriteData.Rectを使ってテクスチャを選択
// Pre : spriteBegin() is called
// 実行前：spriteBegin()を呼び出す
// Post: spriteEnd() is called
// 実行後：spriteEnd()を呼び出す						P151
//=============================================================================
void Image::draw(SpriteData sd, COLOR_ARGB color)
{
    if (!visible || graphics == NULL)
        return;
    sd.rect = spriteData.rect;                  // use this Images rect to select texture		// この画像の矩形を使ってテクスチャを選択
    sd.texture = textureManager->getTexture();  // get fresh texture incase onReset() was called// onReset()が呼ばれたときに新しいテクスチャを取得

    if(color == graphicsNS::FILTER)             // if draw with filter	// フィルタを使って描画する場合
        graphics->drawSprite(sd, colorFilter);  // use colorFilter		// colorFilterを使用
    else
        graphics->drawSprite(sd, color);        // use color as filter	// フィルタとしてcolorを使用
}

//=============================================================================
// update
// typically called once per frame
// 通常、フレームごとに1回呼び出す
// frameTime is used to regulate the speed of movement and animation
// frameTimeは、移動とアニメーションの速さを制御するために使用		P163
//=============================================================================
void Image::update(float frameTime)
{
    if (endFrame - startFrame > 0)          // if animated sprite	// アニメーション化するスプライトの場合
    {
        animTimer += frameTime;             // total elapsed time	// 合計の経過時間
        if (animTimer > frameDelay)
        {
            animTimer -= frameDelay;
            currentFrame++;
            if (currentFrame < startFrame || currentFrame > endFrame)
            {
                if(loop == true)            // if looping animation	// ループするアニメーションの場合
                    currentFrame = startFrame;
                else                        // not looping animation// ループしないアニメーションの場合
                {
                    currentFrame = endFrame;
                    animComplete = true;    // animation complete	// アニメーションの終了
                }
            }
            setRect();                      // set spriteData.rect	// spriteData.rectを設定
        }
    }
}

//=============================================================================
// Set the current frame of the image
// 画像の現在のフレームを設定			P164
//=============================================================================
void Image::setCurrentFrame(int c) 
{
    if(c >= 0)
    {
        currentFrame = c;
        animComplete = false;
        setRect();                          // set spriteData.rect	// spriteData.rectを設定
    }
}

//=============================================================================
//  Set spriteData.rect to draw currentFrame
// currentFrameを描画するspriteData.rectを設定		P164
//=============================================================================
inline void Image::setRect() 
{
    // configure spriteData.rect to draw currentFrame
	// currentFrameを描画するspriteData.rectを設定
    spriteData.rect.left = (currentFrame % cols) * spriteData.width;
    // right edge + 1
	// 右端 + 1
    spriteData.rect.right = spriteData.rect.left + spriteData.width;
    spriteData.rect.top = (currentFrame / cols) * spriteData.height;
    // bottom edge + 1
	// 下端 + 1
    spriteData.rect.bottom = spriteData.rect.top + spriteData.height;       
}

