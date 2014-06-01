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
// このクラスはゲームエンジンのコア					P156
//=============================================================================
class Spacewar : public Game
{
private:
    // game items
	// ゲームアイテム
    TextureManager nebulaTexture;   // nebula texture	// 星雲のテクスチャ
    TextureManager planetTexture;   // planet texture	// 惑星のテクスチャ
    TextureManager shipTexture;     // ship texture		// 宇宙船のテクスチャ
    Image   planet;                 // planet image		// 惑星の画像
    Image   nebula;                 // nebula image		// 星雲の画像
    Image   ship;                   // ship image		// 宇宙船の画像


public:
    // Constructor
	// コンストラクタ
    Spacewar();

    // Destructor
	// デストラクタ
    virtual ~Spacewar();

    // Initialize the game
	// ゲームを初期化
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // Gameからの純粋仮想関数をオーバーライドする必要がある
    void collisions();  // "
    void render();      // "
    void releaseAll();
    void resetAll();
};

#endif
