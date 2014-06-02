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
// �e�N�X�`���t�@�C�����f�B�X�N����ǂݍ���
// Post: returns true if successful, false if failed
// ���s��F���������ꍇ��True�A���s�����ꍇ��false��߂�				P147
//=============================================================================
bool TextureManager::initialize(Graphics *g, const char *f)
{
    try{
        graphics = g;                       // the graphics object	// graphics�I�u�W�F�N�g
        file = f;                           // the texture file		// �e�N�X�`���t�@�C��

        hr = graphics->loadTexture(file, TRANSCOLOR, width, height, texture);
        if (FAILED(hr))
        {
            SAFE_RELEASE(texture);
            return false;
        }
    }
    catch(...) {return false;}
    initialized = true;                    // set true when successfully initialized	// ����ɏ��������ꂽ�ꍇtrue��ݒ�
    return true;
}

//=============================================================================
// called when graphics device is lost
// �O���t�B�b�N�X�f�o�C�X�����������ꍇ�ɌĂяo�����
//=============================================================================
void TextureManager::onLostDevice()
{
    if (!initialized)
        return;
    SAFE_RELEASE(texture);
}

//=============================================================================
// called when graphics device is reset
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�ꍇ�ɌĂяo�����
//=============================================================================
void TextureManager::onResetDevice()
{
    if (!initialized)
        return;
    graphics->loadTexture(file, TRANSCOLOR, width, height, texture);
}


