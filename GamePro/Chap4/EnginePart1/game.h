// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 4 game.h v1.0
//GameProg �ۑ�̂��߉��ς����R�[�h�B�^�C�g���o�[�Ɍ��݂�FPS��\��
//�ύX�_�́@19�s�� tchar.h�̃C���N���[�h�C107�s��setWindowTitleFPS��`�̒ǉ�

#ifndef _GAME_H                 // �����̏ꏊ�ŃC���N���[�h�����̂�h������
#define _GAME_H                 // ���d��`�Ń`�F�b�N���܂�
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <Mmsystem.h>
#include "graphics.h"
#include "input.h"
#include "constants.h"
#include "gameError.h"
/////GameProg �ۑ�T���v���@�ύX�ӏ�/////////
#include <tchar.h>

class Game
{
protected:
    // ���ʂ̃Q�[���v���p�e�B�iproperties�j
    Graphics *graphics;         // Graphics�N���X�ւ̃|�C���^
    Input   *input;             // Input�N���X�ւ̃|�C���^
    HWND    hwnd;               // window handle
    HRESULT hr;                 // �W���̖߂�^
    LARGE_INTEGER timeStart;    // �p�t�H�[�}���X�J�E���^�[�̊J�n�l
    LARGE_INTEGER timeEnd;      // �p�t�H�[�}���X�J�E���^�[�̏I���l
    LARGE_INTEGER timerFreq;    // �p�t�H�[�}���X�J�E���^�[�̎��g��
    float   frameTime;          // �Ō�̃t���[���ɗv��������
    float   fps;                // �t���[�����[�g�iframes per second�j
    DWORD   sleepTime;          // �t���[���Ԃ̃X���[�v���鎞�ԁi�~���b�j
    bool    paused;             // �Q�[�����ꎞ��~����Ă���ꍇtrue
    bool    initialized;

public:
    // �R���X�g���N�^
    Game();
    // �f�X�g���N�^
    virtual ~Game();

    // �����o�֐�

    // �E�C���h�E���b�Z�[�W�n���h��
    LRESULT messageHandler( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

    // �Q�[���̏�����
    // ���s�O�Fhwnd�̓E�C���h�E�ւ̃n���h��
    virtual void initialize(HWND hwnd);

    // WinMain�̃��b�Z�[�W���[�v����run���J��Ԃ��Ăт܂�
    virtual void run(HWND);

	// �O���t�B�b�N�X�f�o�C�X���Ď������Ƃ��ɌĂяo��
	// �O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽�߁B
	// �\�񂳂�Ă����r�f�I�����������ׂĉ��
    // Call when the graphics device was lost.
    // Release all reserved video memory so graphics device may be reset.
    virtual void releaseAll();

	//���ׂẴT�[�t�F�C�X���Đ������A���ׂẴG���e�B�e�B(���́E���ݕ�)�����Z�b�g
    // Recreate all surfaces and reset all entities.
    virtual void resetAll();

	// ���ׂĂ̗\�񃁃������폜
    // Delete all reserved memory.
    virtual void deleteAll();

	// �Q�[���A�C�e���������_�����O
    // Render game items.
    virtual void renderGame();

	// ���������O���t�B�b�N�X�f�o�C�X���n���h��
    // Handle lost graphics device
    virtual void handleLostGraphicsDevice();

	// Graphics�N���X�ւ̃|�C���^��߂�
    // Return pointer to Graphics.
    Graphics* getGraphics() {return graphics;}

	// Input�N���X�ւ̃|�C���^��߂�
    // Return pointer to Input.
    Input* getInput()       {return input;}

	// �Q�[�����I��
    // Exit the game
    void exitGame()         {PostMessage(hwnd, WM_DESTROY, 0, 0);}

	// �������z�֐��̐錾
    // Pure virtual function declarations
	// �����̊֐���Game����̌p��(inherits)�ŋL�q����K�v������܂��B
    // These functions MUST be written in any class that inherits from Game

	// �Q�[���A�C�e�����X�V
    // Update game items.
    virtual void update() = 0;

	//AI�v�Z������
    // Perform AI calculations.
    virtual void ai() = 0;

	// �Փ˂��`�F�b�N
    // Check for collisions.
    virtual void collisions() = 0;

	// �O���t�B�b�N�X�������_�����O
    // Render graphics.
    // Call graphics->spriteBegin();
	//   �X�v���C�g��`��
    //   draw sprites
    // Call graphics->spriteEnd();
    //   draw non-sprites
    virtual void render() = 0;

/////GameProg �ۑ�T���v���@�ύX�ӏ�/////////
    virtual void setWindowTitleFPS(HWND hwnd,float frameTime);

};

#endif
