//GameProgramming2014
//2章サンプルHelloWorld + Mutex
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
//関数プロトタイプ
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);
bool AnotherInstance();
//グローバル変数
HINSTANCE hinst;
//定数
const char CLASS_NAME[] = "WinMain";
const char APP_TITLE[] = "Mutex test";
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;
//===============================
//Windowsアプリケーションの開始点
//===============================
int WINAPI WinMain( HINSTANCE hInstance,
				    HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nCmdShow )
{
	MSG msg;

	//ミューテックスを使用し、2つ目以降は終了させる。
	if(AnotherInstance())
	{	return false;	}

	//ウィンドウの作成
	if(!CreateMainWindow(hInstance,nCmdShow))
	{	return false;	}
	//メインループ
	int done = 0;
	while(!done)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{	done = 1;	}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}
//==================================
//ウィンドウイベントコールバック関数
//==================================
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY://windowsにこのプログラムを終了するように伝える
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}
//=================================
//ウィンドウ作成関数
//戻り値：エラーの場合 false
//=================================
bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;
	HWND hwnd;
	//ウィンドウクラスの構造体を、メインウィンドウを記述するパラメータで設定。
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
	//ウィンドウクラスの登録
	if(RegisterClassEx(&wcx) == 0 )
	{	return false;	}	//エラーの場合
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
	//ウィンドウ作成でのエラー
	if(!hwnd)
	{	return false;	}
	//ウィンドウの表示
	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);
	return true;
}
//===============
//現在のアプリケーションの別のインスタンスがないかをチェック
//戻り値：別のインスタンスが見つかった場合はtrue
//　　　：見つからない場合false
//================
bool AnotherInstance(){
	HANDLE ourMutex;
	ourMutex = CreateMutex(NULL,true,"Mutex_ABCD_1234");
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{	return true;	}
	return false;
}