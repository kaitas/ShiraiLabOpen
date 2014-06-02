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
	// SpriteData�\���̂ւ̎Q�Ƃ�߂�
    const virtual SpriteData& getSpriteInfo() {return spriteData;}

    // Return visible parameter.
	// Visible�p�����[�^��߂�
    virtual bool  getVisible()  {return visible;}

    // Return X position.
	// X�ʒu��߂�
    virtual float getX()        {return spriteData.x;}

    // Return Y position.
	// Y�ʒu��߂�
    virtual float getY()        {return spriteData.y;}

    // Return scale factor.
	// �{����߂�
    virtual float getScale()    {return spriteData.scale;}

    // Return width.
	// ����߂�
    virtual int   getWidth()    {return spriteData.width;}

    // Return height.
	// ������߂�
    virtual int   getHeight()   {return spriteData.height;}

    // Return center X.
	// ���S�_��X��߂�
    virtual float getCenterX()      {return spriteData.x + spriteData.width/2*getScale();}

    // Return center Y.
	// ���S�_��Y��߂�
    virtual float getCenterY()      {return spriteData.y + spriteData.height/2*getScale();}

    // Return rotation angle in degrees.
	// ��]�p�x(�x�P��)��߂�
    virtual float getDegrees()      {return spriteData.angle*(180.0f/(float)PI);}

    // Return rotation angle in radians.
	// ��]�p�x(���W�A���P��)��߂�
    virtual float getRadians()      {return spriteData.angle;}

    // Return delay between frames of animation.
	// �A�j���[�V�����̃t���[���Ԃ̊Ԋu��߂�
    virtual float getFrameDelay()   {return frameDelay;}

    // Return number of starting frame.
	// �J�n�t���[���̔ԍ���߂�
    virtual int   getStartFrame()   {return startFrame;}

    // Return number of ending frame.
	// �I���t���[���̔ԍ���߂�
    virtual int   getEndFrame()     {return endFrame;}

    // Return number of current frame.
	// ���݂̃t���[���ԍ���߂�
    virtual int   getCurrentFrame() {return currentFrame;}

    // Return RECT structure of Image.
	// Image��RECT�\���̂�߂�
    virtual RECT  getSpriteDataRect() {return spriteData.rect;}

    // Return state of animation complete.
	// �A�j���[�V���������̏�Ԃ�߂�
    virtual bool  getAnimationComplete() {return animComplete;}

    // Return colorFilter.
	// colorFilter��߂�
    virtual COLOR_ARGB getColorFilter() {return colorFilter;}

    ////////////////////////////////////////
    //           Set functions            //P154
    ////////////////////////////////////////

    // Set X location.
	// X�ʒu��ݒ�
    virtual void setX(float newX)   {spriteData.x = newX;}

    // Set Y location.
	// Y�ʒu��ݒ�
    virtual void setY(float newY)   {spriteData.y = newY;}

    // Set scale.
	// �g��k����ݒ�
    virtual void setScale(float s)  {spriteData.scale = s;}

    // Set rotation angle in degrees.
	// ��]�p�x��ݒ�i�x�P�ʁj
    // 0 degrees is up. Angles progress clockwise.
	// 0�x�͏�����ł��B�p�x�͎��v���ɐi�݂܂��B
    virtual void setDegrees(float deg)  {spriteData.angle = deg*((float)PI/180.0f);}

    // Set rotation angle in radians.
	// ��]�p�x��ݒ�(���W�A���P��)
    // 0 radians is up. Angles progress clockwise.
	// 0���W�A���͏�����ł��B�p�x�͎��v���ɐi�݂܂��B
    virtual void setRadians(float rad)  {spriteData.angle = rad;}

    // Set visible.
	// ������ݒ�
    virtual void setVisible(bool v) {visible = v;}

    // Set delay between frames of animation.
	// �A�j���[�V�����̃t���[���Ԃ̊Ԋu��ݒ�
    virtual void setFrameDelay(float d) {frameDelay = d;}

    // Set starting and ending frames of animation.
	// �A�j���[�V�����̊J�n�t���[���ƏI���t���[����ݒ�
    virtual void setFrames(int s, int e){startFrame = s; endFrame = e;}

    // Set current frame of animation.
	// �A�j���[�V�����̌��݂̃t���[����ݒ�
    virtual void setCurrentFrame(int c);

    // Set spriteData.rect to draw currentFrame
	// currentFrame��`�悷��spriteData.rect��ݒ�
    virtual void setRect(); 

    // Set spriteData.rect to r.
	// spriteData.rect��r�ɐݒ�
    virtual void setSpriteDataRect(RECT r)  {spriteData.rect = r;}

    // Set animation loop. lp = true to loop.
	// �A�j���[�V�������[�v��ݒ�
    virtual void setLoop(bool lp) {loop = lp;}

    // Set animation complete Boolean.
	// �A�j���[�V���������u�[���l��ݒ�
    virtual void setAnimationComplete(bool a) {animComplete = a;};

    // Set color filter. (use WHITE for no change)
	// �F�t�B���^��ݒ�(WHITE��ݒ肵���ꍇ�A�ω����Ȃ�)
    virtual void setColorFilter(COLOR_ARGB color) {colorFilter = color;}

    // Set TextureManager
	// TextureManager��ݒ�
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
	// �摜�𐅕������ɔ��](��)		P152
    virtual void flipHorizontal(bool flip)  {spriteData.flipHorizontal = flip;}

    // Flip image vertically
	// �摜�𐂒������ɔ��]
    virtual void flipVertical(bool flip)    {spriteData.flipVertical = flip;}

    // Draw Image using color as filter. Default color is WHITE.
	// color���t�B���^�Ƃ��Ďg���ĉ摜��`��Bcolor�̃f�t�H���g�͔�
    virtual void draw(COLOR_ARGB color = graphicsNS::WHITE);

    // Draw this image using the specified SpriteData.
    //   The current SpriteData.rect is used to select the texture.
    virtual void draw(SpriteData sd, COLOR_ARGB color = graphicsNS::WHITE); // draw with SpriteData using color as filter

    // Update the animation. frameTime is used to regulate the speed.
    virtual void update(float frameTime);
};

#endif

