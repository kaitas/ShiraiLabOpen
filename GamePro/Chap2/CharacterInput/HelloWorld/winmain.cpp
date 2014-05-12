// List 2.14
// Programming 2D Games
// Copyright (c) 2011 by Chales Kelly
// ��2�� Windows �X�^�C���́uCharacter Input�vv.1.0
// winmain.cpp
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
//�֐��v���g�^�C�v
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

//�O���[�o���ϐ�
HINSTANCE hinst;
HDC hdc;
TCHAR ch = ' ';
RECT rect;
PAINTSTRUCT ps;
//�萔
const char CLASS_NAME[] = "Keyboard";
//const char APP_TITLE[] = "Char Input"; //��1��
const char APP_TITLE[] = "DirectX Window";   //��2�� DirectX
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 300;
//===============================
//Windows�A�v���P�[�V�����̊J�n�_
//===============================
int WINAPI WinMain( HINSTANCE hInstance,
				    HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nCmdShow )
{
	MSG msg;
	//�E�B���h�E�̍쐬
	if(!CreateMainWindow(hInstance,nCmdShow))
	{	return false;	}
	//���C�����[�v
	int done = 0;
	while(!done)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			//�I�����b�Z�[�W�����n
			if(msg.message == WM_QUIT)
			{	done = 1;	}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}
//==================================
//�E�B���h�E�C�x���g�R�[���o�b�N�֐�
//==================================
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam,LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY://windows�ɂ��̃v���O�������I������悤�ɓ`����
		PostQuitMessage(0);
		return 0;
	case WM_CHAR:
		switch(wParam)
		{
			case 0x08:
			case 0x09:
			case 0x0A:
			case 0x0D:
			case 0x1B:
				MessageBeep((UINT)-1);
				return 0;
			default:
				ch = (TCHAR) wParam;
				InvalidateRect(hwnd, NULL, TRUE);
				return 0;
		}
	case WM_PAINT:
		hdc = BeginPaint(hwnd,&ps);
		GetClientRect(hwnd,&rect);
		//������\��
		TextOut(hdc,rect.right/2,rect.bottom/2,&ch,1);
		EndPaint(hwnd,&ps);
		return 0;
	default:
		return DefWindowProc(hwnd,msg,wParam,lParam);
	}
}
//=================================
//�E�B���h�E�쐬�֐�
//�߂�l�F�G���[�̏ꍇ false
//=================================
bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;
	HWND hwnd;
	//�E�B���h�E�N���X�̍\���̂��A���C���E�B���h�E���L�q����p�����[�^�Őݒ�B
	wcx.cbSize	= sizeof(wcx);
	wcx.style	= CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WinProc;
	wcx.cbClsExtra	=0;
	wcx.cbWndExtra	= 0;
	wcx.hInstance	=hInstance;
	wcx.hIcon	= NULL;
	wcx.hCursor	= LoadCursor(NULL,IDC_ARROW);
	wcx.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName	= NULL;
	wcx.lpszClassName	= CLASS_NAME;
	wcx.hIconSm	= NULL;
	//�E�B���h�E�N���X�̓o�^
	if(RegisterClassEx(&wcx) == 0 )
	{	return false;	}	//�G���[�̏ꍇ
	hwnd = CreateWindow(
		CLASS_NAME,
		APP_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		(HWND)NULL,
		(HMENU)NULL,
		hInstance,
		(LPVOID)NULL );
	//�E�B���h�E�쐬�ł̃G���[
	if(!hwnd)
	{	return false;	}
	//�E�B���h�E�̕\��
	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);
	return true;
}
