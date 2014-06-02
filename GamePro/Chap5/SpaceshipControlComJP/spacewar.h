// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 5 spacewar.h v1.0

#ifndef _SPACEWAR_H             // Prevent multiple definitions if this 
#define _SPACEWAR_H             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"

//=============================================================================
// This class is the core of the game
// ���̃N���X�̓Q�[���G���W���̃R�A					P156
//=============================================================================
class Spacewar : public Game
{
private:
    // game items
	// �Q�[���A�C�e��
    TextureManager nebulaTexture;   // nebula texture	// ���_�̃e�N�X�`��
    TextureManager planetTexture;   // planet texture	// �f���̃e�N�X�`��
    TextureManager shipTexture;     // ship texture		// �F���D�̃e�N�X�`��
    Image   planet;                 // planet image		// �f���̉摜
    Image   nebula;                 // nebula image		// ���_�̉摜
    Image   ship;                   // ship image		// �F���D�̉摜


public:
    // Constructor
	// �R���X�g���N�^
    Spacewar();

    // Destructor
	// �f�X�g���N�^
    virtual ~Spacewar();

    // Initialize the game
	// �Q�[����������
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // Game����̏������z�֐����I�[�o�[���C�h����K�v������
    void collisions();  // "
    void render();      // "
    void releaseAll();
    void resetAll();
};

#endif
