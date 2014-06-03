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
// Image��������
// Post: returns true if successful, false if failed
// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
// pointer to Graphics
// Graphics�ւ̃|�C���^
// width of Image in pixels  (0 = use full texture width)
// Image�̕�(�s�N�Z���P��)(0 = �e�N�X�`���S�̂̕����g�p)
// height of Image in pixels (0 = use full texture height)
// Image�̍���(�s�N�Z���P��)(0 = �e�N�X�`���S�̂̍������g�p)
// number of columns in texture (1 to n) (0 same as 1)
// �e�N�X�`�����̗�(1����n�܂�)(0��1�Ɠ���)
// pointer to TextureManager
// TextureManager�ւ̃|�C���^						P149
//=============================================================================
bool Image::initialize(Graphics *g, int width, int height, int ncols,
                       TextureManager *textureM)
{
    try{
        graphics = g;                               // the graphics object			// graphics�I�u�W�F�N�g
        textureManager = textureM;                  // pointer to texture object	// �e�N�X�`���I�u�W�F�N�g�ւ̃|�C���^

        spriteData.texture = textureManager->getTexture();
        if(width == 0)
            width = textureManager->getWidth();     // use full width of texture	// �S�̂̕����g�p	P150
        spriteData.width = width;
        if(height == 0)
            height = textureManager->getHeight();   // use full height of texture	// �S�̂̍������g�p
        spriteData.height = height;
        cols = ncols;
        if (cols == 0)
            cols = 1;                               // if 0 cols use 1				// cols��0�̏ꍇ�ɂ�1���g�p

        // configure spriteData.rect to draw currentFrame
		// currentFrame��`�悷��spriteData.rect���\��
        spriteData.rect.left = (currentFrame % cols) * spriteData.width;
        // right edge + 1
		// �E�[ + 1
        spriteData.rect.right = spriteData.rect.left + spriteData.width;
        spriteData.rect.top = (currentFrame / cols) * spriteData.height;
        // bottom edge + 1
		// ���[ + 1
        spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
    }
    catch(...) {return false;}
    initialized = true;                                // successfully initialized	// ����ɏ��������ꂽ�ꍇ
    return true;
}


//=============================================================================
// Draw the image using color as filter
// color���t�B���^�Ƃ��Ďg���ĉ摜��`��
// The color parameter is optional, WHITE is assigned as default in image.h
// color�p�����[�^�̓I�v�V����(image.h�ŁA�f�t�H���g�Ƃ��Ĕ������蓖�Ă��Ă���)
// Pre : spriteBegin() is called
// ���s�O�FspriteBegin()���Ăяo��
// Post: spriteEnd() is called
// ���s��FspriteEnd()���Ăяo��						P151
//=============================================================================
void Image::draw(COLOR_ARGB color)
{
    if (!visible || graphics == NULL)
        return;
    // get fresh texture incase onReset() was called
	// onReset()���Ăяo���ꂽ�Ƃ��ɐV�����e�N�X�`�����擾
    spriteData.texture = textureManager->getTexture();
    if(color == graphicsNS::FILTER)                     // if draw with filter	// �t�B���^���g���ĕ`�悷��ꍇ
        graphics->drawSprite(spriteData, colorFilter);  // use colorFilter		// colorFilter���g�p
    else
        graphics->drawSprite(spriteData, color);        // use color as filter	// �t�B���^�Ƃ���color���g�p
}

//=============================================================================
// Draw this image using the specified SpriteData.
// �w�肳�ꂽSpriteData���g���Ă��̉摜��`��
//   The current SpriteData.rect is used to select the texture.
// ���݂�SpriteData.Rect���g���ăe�N�X�`����I��
// Pre : spriteBegin() is called
// ���s�O�FspriteBegin()���Ăяo��
// Post: spriteEnd() is called
// ���s��FspriteEnd()���Ăяo��						P151
//=============================================================================
void Image::draw(SpriteData sd, COLOR_ARGB color)
{
    if (!visible || graphics == NULL)
        return;
    sd.rect = spriteData.rect;                  // use this Images rect to select texture		// ���̉摜�̋�`���g���ăe�N�X�`����I��
    sd.texture = textureManager->getTexture();  // get fresh texture incase onReset() was called// onReset()���Ă΂ꂽ�Ƃ��ɐV�����e�N�X�`�����擾

    if(color == graphicsNS::FILTER)             // if draw with filter	// �t�B���^���g���ĕ`�悷��ꍇ
        graphics->drawSprite(sd, colorFilter);  // use colorFilter		// colorFilter���g�p
    else
        graphics->drawSprite(sd, color);        // use color as filter	// �t�B���^�Ƃ���color���g�p
}

//=============================================================================
// update
// typically called once per frame
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// frameTime is used to regulate the speed of movement and animation
// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p		P163
//=============================================================================
void Image::update(float frameTime)
{
    if (endFrame - startFrame > 0)          // if animated sprite	// �A�j���[�V����������X�v���C�g�̏ꍇ
    {
        animTimer += frameTime;             // total elapsed time	// ���v�̌o�ߎ���
        if (animTimer > frameDelay)
        {
            animTimer -= frameDelay;
            currentFrame++;
            if (currentFrame < startFrame || currentFrame > endFrame)
            {
                if(loop == true)            // if looping animation	// ���[�v����A�j���[�V�����̏ꍇ
                    currentFrame = startFrame;
                else                        // not looping animation// ���[�v���Ȃ��A�j���[�V�����̏ꍇ
                {
                    currentFrame = endFrame;
                    animComplete = true;    // animation complete	// �A�j���[�V�����̏I��
                }
            }
            setRect();                      // set spriteData.rect	// spriteData.rect��ݒ�
        }
    }
}

//=============================================================================
// Set the current frame of the image
// �摜�̌��݂̃t���[����ݒ�			P164
//=============================================================================
void Image::setCurrentFrame(int c) 
{
    if(c >= 0)
    {
        currentFrame = c;
        animComplete = false;
        setRect();                          // set spriteData.rect	// spriteData.rect��ݒ�
    }
}

//=============================================================================
//  Set spriteData.rect to draw currentFrame
// currentFrame��`�悷��spriteData.rect��ݒ�		P164
//=============================================================================
inline void Image::setRect() 
{
    // configure spriteData.rect to draw currentFrame
	// currentFrame��`�悷��spriteData.rect��ݒ�
    spriteData.rect.left = (currentFrame % cols) * spriteData.width;
    // right edge + 1
	// �E�[ + 1
    spriteData.rect.right = spriteData.rect.left + spriteData.width;
    spriteData.rect.top = (currentFrame / cols) * spriteData.height;
    // bottom edge + 1
	// ���[ + 1
    spriteData.rect.bottom = spriteData.rect.top + spriteData.height;       
}

