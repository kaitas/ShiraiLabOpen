// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Move spaceship with arrow keys.
// Chapter 5 spacewar.cpp v1.0
// This class is the core of the game

#include "spaceWar.h"

//=============================================================================
// Constructor
//=============================================================================
Spacewar::Spacewar()
{}

//=============================================================================
// Destructor
//=============================================================================
Spacewar::~Spacewar()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// �Q�[����������
// Throws GameError on error
// �G���[����GameError���X���[				P157
//=============================================================================
void Spacewar::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError	// GameError���X���[

    // nebula texture
	// ���_�̃e�N�X�`��
    if (!nebulaTexture.initialize(graphics,NEBULA_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));

    // planet texture
	// �f���̃e�N�X�`��
    if (!planetTexture.initialize(graphics,PLANET_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet texture"));

    // spaceship texture
	// �F���D�̃e�N�X�`��		P160
    if (!shipTexture.initialize(graphics,SHIP_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship texture"));

    // nebula
	// ���_
    if (!nebula.initialize(graphics,0,0,0,&nebulaTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));

    // planet
	// �f��
    if (!planet.initialize(graphics,0,0,0,&planetTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));
    // place planet in center of screen
	// ��ʂ̒����ɘf����z�u
    planet.setX(GAME_WIDTH*0.5f  - planet.getWidth()*0.5f);
    planet.setY(GAME_HEIGHT*0.5f - planet.getHeight()*0.5f);

    // ship
	// �F���D	P161
    if (!ship.initialize(graphics,SHIP_WIDTH, SHIP_HEIGHT, SHIP_COLS, &shipTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship"));
    ship.setX(GAME_WIDTH/4);                    // start above and left of planet	//�f���̍��ォ��o��
    ship.setY(GAME_HEIGHT/4);
    ship.setFrames(SHIP_START_FRAME, SHIP_END_FRAME);   // animation frames			// �A�j���[�V�����̃t���[��
    ship.setCurrentFrame(SHIP_START_FRAME);     // starting frame					// �J�n�t���[��
    ship.setFrameDelay(SHIP_ANIMATION_DELAY);

    return;
}

//=============================================================================
// Update all game items
// ���ׂẴQ�[���A�C�e�����X�V		P162�A165
//=============================================================================
void Spacewar::update()
{
    if(input->isKeyDown(SHIP_RIGHT_KEY))            // if move right			// �E�ɓ����ꍇ
    {
        ship.setX(ship.getX() + frameTime * SHIP_SPEED);
        if (ship.getX() > GAME_WIDTH)               // if off screen right		// ��ʂ̉E�ɂ͂ݏo���ꍇ
            ship.setX((float)-ship.getWidth());     // position off screen left // ��ʂ̍��ɔz�u
    }
    if(input->isKeyDown(SHIP_LEFT_KEY))             // if move left				// ���ɓ����ꍇ
    {
        ship.setX(ship.getX() - frameTime * SHIP_SPEED);
        if (ship.getX() < -ship.getWidth())         // if off screen left		// ��ʂ̍��ɂ͂ݏo���ꍇ
            ship.setX((float)GAME_WIDTH);           // position off screen right// ��ʂ̉E�ɔz�u
    }
    if(input->isKeyDown(SHIP_UP_KEY))               // if move up				// ��ɓ����ꍇ
    {
        ship.setY(ship.getY() - frameTime * SHIP_SPEED);
        if (ship.getY() < -ship.getHeight())        // if off screen top		// ��ʂ̏�ɂ͂ݏo���ꍇ
            ship.setY((float)GAME_HEIGHT);          // position off screen bottom // ��ʂ̉��ɔz�u
    }
    if(input->isKeyDown(SHIP_DOWN_KEY))             // if move down				// ���ɓ����ꍇ
    {
        ship.setY(ship.getY() + frameTime * SHIP_SPEED);
        if (ship.getY() > GAME_HEIGHT)              // if off screen bottom		// ��ʂ̉��ɂ͂ݏo���ꍇ
            ship.setY((float)-ship.getHeight());    // position off screen top	// ��ʂ̏�ɔz�u
    }

    ship.update(frameTime); //�A�j���[�V�����̃t���[�����X�V
	//�F���D����]
	ship.setDegrees(ship.getDegrees() - frameTime * ROTATION_RATE ); 
	//�F���D���k��
	ship.setScale(ship.getScale() - frameTime * SCALE_RATE );
	if (ship.getScale()<0) {
		ship.setScale(SHIP_SCALE);
	}
	//�����E�ɂ͂ݏo�����烊�Z�b�g����
	if (ship.getX() > GAME_WIDTH ) {
		ship.setX((float)-ship.getWidth());
		ship.setScale(SHIP_SCALE);
	}

}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void Spacewar::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void Spacewar::collisions()
{}

//=============================================================================
// Render game items
// �Q�[���A�C�e���������_�[			P158
//=============================================================================
void Spacewar::render()
{
    graphics->spriteBegin();                // begin drawing sprites			// �X�v���C�g�̕`����J�n

    nebula.draw();                          // add the orion nebula to the scene// �I���I�����_���V�[���ɒǉ�
//    planet.draw();                          // add the planet to the scene		// �f�����V�[���ɒǉ�
    ship.draw();                            // add the spaceship to the scene	// �F���D���V�[���ɒǉ�
    planet.draw();
	
    graphics->spriteEnd();                  // end drawing sprites				// �X�v���C�g�̕`����I��
}

//=============================================================================
// The graphics device was lost.
// �O���t�B�b�N�f�o�C�X�����������ꍇ
// Release all reserved video memory so graphics device may be reset.
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽��
// �\�񂳂�Ă����r�f�I�����������ׂĉ��				P158
//=============================================================================
void Spacewar::releaseAll()
{
    shipTexture.onLostDevice();
    planetTexture.onLostDevice();
    nebulaTexture.onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�ꍇ
// Recreate all surfaces.
// ���ׂẴT�[�t�F�C�X���č쐬							P159
//=============================================================================
void Spacewar::resetAll()
{
    nebulaTexture.onResetDevice();
    planetTexture.onResetDevice();
    shipTexture.onResetDevice();

    Game::resetAll();
    return;
}
