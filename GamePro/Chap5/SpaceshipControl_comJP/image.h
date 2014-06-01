// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// image.h v1.2

#ifndef _IMAGE_H                // Prevent multiple definitions if this 
#define _IMAGE_H                // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "textureManager.h"
#include "constants.h"

class Image
{
    // Image properties
  protected:
    Graphics *graphics;     // pointer to graphics
    TextureManager *textureManager; // pointer to texture manager
    // spriteData contains the data required to draw the image by Graphics::drawSprite()
    SpriteData spriteData;  // SpriteData is defined in "graphics.h"
    COLOR_ARGB colorFilter; // applied as a color filter (use WHITE for no change)
    int     cols;           // number of cols (1 to n) in multi-frame sprite
    int     startFrame;     // first frame of current animation
    int     endFrame;       // end frame of current animation
    int     currentFrame;   // current frame of animation
    float   frameDelay;     // how long between frames of animation
    float   animTimer;      // animation timer
    HRESULT hr;             // standard return type
    bool    loop;           // true to loop frames
    bool    visible;        // true when visible
    bool    initialized;    // true when successfully initialized
    bool    animComplete;   // true when loop is false and endFrame has finished displaying

  public:
    // Constructor
    Image();
    // Destructor
    virtual ~Image();

    ////////////////////////////////////////
    //           Get functions            //P153
    ////////////////////////////////////////

    // Return reference to SpriteData structure.
	// SpriteData構造体への参照を戻す
    const virtual SpriteData& getSpriteInfo() {return spriteData;}

    // Return visible parameter.
	// Visibleパラメータを戻す
    virtual bool  getVisible()  {return visible;}

    // Return X position.
	// X位置を戻す
    virtual float getX()        {return spriteData.x;}

    // Return Y position.
	// Y位置を戻す
    virtual float getY()        {return spriteData.y;}

    // Return scale factor.
	// 倍率を戻す
    virtual float getScale()    {return spriteData.scale;}

    // Return width.
	// 幅を戻す
    virtual int   getWidth()    {return spriteData.width;}

    // Return height.
	// 高さを戻す
    virtual int   getHeight()   {return spriteData.height;}

    // Return center X.
	// 中心点のXを戻す
    virtual float getCenterX()      {return spriteData.x + spriteData.width/2*getScale();}

    // Return center Y.
	// 中心点のYを戻す
    virtual float getCenterY()      {return spriteData.y + spriteData.height/2*getScale();}

    // Return rotation angle in degrees.
	// 回転角度(度単位)を戻す
    virtual float getDegrees()      {return spriteData.angle*(180.0f/(float)PI);}

    // Return rotation angle in radians.
	// 回転角度(ラジアン単位)を戻す
    virtual float getRadians()      {return spriteData.angle;}

    // Return delay between frames of animation.
	// アニメーションのフレーム間の間隔を戻す
    virtual float getFrameDelay()   {return frameDelay;}

    // Return number of starting frame.
	// 開始フレームの番号を戻す
    virtual int   getStartFrame()   {return startFrame;}

    // Return number of ending frame.
	// 終了フレームの番号を戻す
    virtual int   getEndFrame()     {return endFrame;}

    // Return number of current frame.
	// 現在のフレーム番号を戻す
    virtual int   getCurrentFrame() {return currentFrame;}

    // Return RECT structure of Image.
	// ImageのRECT構造体を戻す
    virtual RECT  getSpriteDataRect() {return spriteData.rect;}

    // Return state of animation complete.
	// アニメーション完了の状態を戻す
    virtual bool  getAnimationComplete() {return animComplete;}

    // Return colorFilter.
	// colorFilterを戻す
    virtual COLOR_ARGB getColorFilter() {return colorFilter;}

    ////////////////////////////////////////
    //           Set functions            //P154
    ////////////////////////////////////////

    // Set X location.
	// X位置を設定
    virtual void setX(float newX)   {spriteData.x = newX;}

    // Set Y location.
	// Y位置を設定
    virtual void setY(float newY)   {spriteData.y = newY;}

    // Set scale.
	// 拡大縮小を設定
    virtual void setScale(float s)  {spriteData.scale = s;}

    // Set rotation angle in degrees.
	// 回転角度を設定（度単位）
    // 0 degrees is up. Angles progress clockwise.
	// 0度は上向きです。角度は時計回りに進みます。
    virtual void setDegrees(float deg)  {spriteData.angle = deg*((float)PI/180.0f);}

    // Set rotation angle in radians.
	// 回転角度を設定(ラジアン単位)
    // 0 radians is up. Angles progress clockwise.
	// 0ラジアンは上向きです。角度は時計回りに進みます。
    virtual void setRadians(float rad)  {spriteData.angle = rad;}

    // Set visible.
	// 可視性を設定
    virtual void setVisible(bool v) {visible = v;}

    // Set delay between frames of animation.
	// アニメーションのフレーム間の間隔を設定
    virtual void setFrameDelay(float d) {frameDelay = d;}

    // Set starting and ending frames of animation.
	// アニメーションの開始フレームと終了フレームを設定
    virtual void setFrames(int s, int e){startFrame = s; endFrame = e;}

    // Set current frame of animation.
	// アニメーションの現在のフレームを設定
    virtual void setCurrentFrame(int c);

    // Set spriteData.rect to draw currentFrame
	// currentFrameを描画するspriteData.rectを設定
    virtual void setRect(); 

    // Set spriteData.rect to r.
	// spriteData.rectをrに設定
    virtual void setSpriteDataRect(RECT r)  {spriteData.rect = r;}

    // Set animation loop. lp = true to loop.
	// アニメーションループを設定
    virtual void setLoop(bool lp) {loop = lp;}

    // Set animation complete Boolean.
	// アニメーション完了ブール値を設定
    virtual void setAnimationComplete(bool a) {animComplete = a;};

    // Set color filter. (use WHITE for no change)
	// 色フィルタを設定(WHITEを設定した場合、変化しない)
    virtual void setColorFilter(COLOR_ARGB color) {colorFilter = color;}

    // Set TextureManager
	// TextureManagerを設定
    virtual void setTextureManager(TextureManager *textureM)
    { textureManager = textureM; }

    ////////////////////////////////////////
    //         Other functions            //
    ////////////////////////////////////////

    // Initialize Image
    // Pre: *g = pointer to Graphics object
    //      width = width of Image in pixels  (0 = use full texture width)
    //      height = height of Image in pixels (0 = use full texture height)
    //      ncols = number of columns in texture (1 to n) (0 same as 1)
    //      *textureM = pointer to TextureManager object
    virtual bool Image::initialize(Graphics *g, int width, int height, 
                                    int ncols, TextureManager *textureM);

    // Flip image horizontally (mirror)
	// 画像を水平方向に反転(鏡)		P152
    virtual void flipHorizontal(bool flip)  {spriteData.flipHorizontal = flip;}

    // Flip image vertically
	// 画像を垂直方向に反転
    virtual void flipVertical(bool flip)    {spriteData.flipVertical = flip;}

    // Draw Image using color as filter. Default color is WHITE.
	// colorをフィルタとして使って画像を描画。colorのデフォルトは白
    virtual void draw(COLOR_ARGB color = graphicsNS::WHITE);

    // Draw this image using the specified SpriteData.
    //   The current SpriteData.rect is used to select the texture.
    virtual void draw(SpriteData sd, COLOR_ARGB color = graphicsNS::WHITE); // draw with SpriteData using color as filter

    // Update the animation. frameTime is used to regulate the speed.
    virtual void update(float frameTime);
};

#endif

