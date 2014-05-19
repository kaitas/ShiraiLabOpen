// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 4 game.h v1.0
//GameProg 課題のため改変したコード。タイトルバーに現在のFPSを表示
//変更点は　19行目 tchar.hのインクルード，107行目setWindowTitleFPS定義の追加

#ifndef _GAME_H                 // 複数の場所でインクルードされるのを防ぐため
#define _GAME_H                 // 多重定義でチェックします
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <Mmsystem.h>
#include "graphics.h"
#include "input.h"
#include "constants.h"
#include "gameError.h"
/////GameProg 課題サンプル　変更箇所/////////
#include <tchar.h>

class Game
{
protected:
    // 共通のゲームプロパティ（properties）
    Graphics *graphics;         // Graphicsクラスへのポインタ
    Input   *input;             // Inputクラスへのポインタ
    HWND    hwnd;               // window handle
    HRESULT hr;                 // 標準の戻り型
    LARGE_INTEGER timeStart;    // パフォーマンスカウンターの開始値
    LARGE_INTEGER timeEnd;      // パフォーマンスカウンターの終了値
    LARGE_INTEGER timerFreq;    // パフォーマンスカウンターの周波数
    float   frameTime;          // 最後のフレームに要した時間
    float   fps;                // フレームレート（frames per second）
    DWORD   sleepTime;          // フレーム間のスリープする時間（ミリ秒）
    bool    paused;             // ゲームが一時停止されている場合true
    bool    initialized;

public:
    // コンストラクタ
    Game();
    // デストラクタ
    virtual ~Game();

    // メンバ関数

    // ウインドウメッセージハンドラ
    LRESULT messageHandler( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

    // ゲームの初期化
    // 実行前：hwndはウインドウへのハンドル
    virtual void initialize(HWND hwnd);

    // WinMainのメッセージループ内でrunを繰り返し呼びます
    virtual void run(HWND);

	// グラフィックスデバイスが焼失したときに呼び出す
	// グラフィックスデバイスをリセット可能にするため。
	// 予約されていたビデオメモリをすべて解放
    // Call when the graphics device was lost.
    // Release all reserved video memory so graphics device may be reset.
    virtual void releaseAll();

	//すべてのサーフェイスを再生成し、すべてのエンティティ(実体・実在物)をリセット
    // Recreate all surfaces and reset all entities.
    virtual void resetAll();

	// すべての予約メモリを削除
    // Delete all reserved memory.
    virtual void deleteAll();

	// ゲームアイテムをレンダリング
    // Render game items.
    virtual void renderGame();

	// 消失したグラフィックスデバイスをハンドル
    // Handle lost graphics device
    virtual void handleLostGraphicsDevice();

	// Graphicsクラスへのポインタを戻す
    // Return pointer to Graphics.
    Graphics* getGraphics() {return graphics;}

	// Inputクラスへのポインタを戻す
    // Return pointer to Input.
    Input* getInput()       {return input;}

	// ゲームを終了
    // Exit the game
    void exitGame()         {PostMessage(hwnd, WM_DESTROY, 0, 0);}

	// 純粋仮想関数の宣言
    // Pure virtual function declarations
	// これらの関数はGameからの継承(inherits)で記述する必要があります。
    // These functions MUST be written in any class that inherits from Game

	// ゲームアイテムを更新
    // Update game items.
    virtual void update() = 0;

	//AI計算を処理
    // Perform AI calculations.
    virtual void ai() = 0;

	// 衝突をチェック
    // Check for collisions.
    virtual void collisions() = 0;

	// グラフィックスをレンダリング
    // Render graphics.
    // Call graphics->spriteBegin();
	//   スプライトを描画
    //   draw sprites
    // Call graphics->spriteEnd();
    //   draw non-sprites
    virtual void render() = 0;

/////GameProg 課題サンプル　変更箇所/////////
    virtual void setWindowTitleFPS(HWND hwnd,float frameTime);

};

#endif
