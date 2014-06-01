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
// ゲームを初期化
// Throws GameError on error
// エラー時にGameErrorをスロー				P157
//=============================================================================
void Spacewar::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError	// GameErrorをスロー

    // nebula texture
	// 星雲のテクスチャ
    if (!nebulaTexture.initialize(graphics,NEBULA_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));

    // planet texture
	// 惑星のテクスチャ
    if (!planetTexture.initialize(graphics,PLANET_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet texture"));

    // spaceship texture
	// 宇宙船のテクスチャ		P160
    if (!shipTexture.initialize(graphics,SHIP_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship texture"));

    // nebula
	// 星雲
    if (!nebula.initialize(graphics,0,0,0,&nebulaTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));

    // planet
	// 惑星
    if (!planet.initialize(graphics,0,0,0,&planetTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));
    // place planet in center of screen
	// 画面の中央に惑星を配置
    planet.setX(GAME_WIDTH*0.5f  - planet.getWidth()*0.5f);
    planet.setY(GAME_HEIGHT*0.5f - planet.getHeight()*0.5f);

    // ship
	// 宇宙船	P161
    if (!ship.initialize(graphics,SHIP_WIDTH, SHIP_HEIGHT, SHIP_COLS, &shipTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship"));
    ship.setX(GAME_WIDTH/4);                    // start above and left of planet	//惑星の左上から出発
    ship.setY(GAME_HEIGHT/4);
    ship.setFrames(SHIP_START_FRAME, SHIP_END_FRAME);   // animation frames			// アニメーションのフレーム
    ship.setCurrentFrame(SHIP_START_FRAME);     // starting frame					// 開始フレーム
    ship.setFrameDelay(SHIP_ANIMATION_DELAY);

    return;
}

//=============================================================================
// Update all game items
// すべてのゲームアイテムを更新		P162、165
//=============================================================================
void Spacewar::update()
{
    if(input->isKeyDown(SHIP_RIGHT_KEY))            // if move right			// 右に動く場合
    {
        ship.setX(ship.getX() + frameTime * SHIP_SPEED);
        if (ship.getX() > GAME_WIDTH)               // if off screen right		// 画面の右にはみ出す場合
            ship.setX((float)-ship.getWidth());     // position off screen left // 画面の左に配置
    }
    if(input->isKeyDown(SHIP_LEFT_KEY))             // if move left				// 左に動く場合
    {
        ship.setX(ship.getX() - frameTime * SHIP_SPEED);
        if (ship.getX() < -ship.getWidth())         // if off screen left		// 画面の左にはみ出す場合
            ship.setX((float)GAME_WIDTH);           // position off screen right// 画面の右に配置
    }
    if(input->isKeyDown(SHIP_UP_KEY))               // if move up				// 上に動く場合
    {
        ship.setY(ship.getY() - frameTime * SHIP_SPEED);
        if (ship.getY() < -ship.getHeight())        // if off screen top		// 画面の上にはみ出す場合
            ship.setY((float)GAME_HEIGHT);          // position off screen bottom // 画面の下に配置
    }
    if(input->isKeyDown(SHIP_DOWN_KEY))             // if move down				// 下に動く場合
    {
        ship.setY(ship.getY() + frameTime * SHIP_SPEED);
        if (ship.getY() > GAME_HEIGHT)              // if off screen bottom		// 画面の下にはみ出す場合
            ship.setY((float)-ship.getHeight());    // position off screen top	// 画面の上に配置
    }

    ship.update(frameTime);
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
// ゲームアイテムをレンダー			P158
//=============================================================================
void Spacewar::render()
{
    graphics->spriteBegin();                // begin drawing sprites			// スプライトの描画を開始

    nebula.draw();                          // add the orion nebula to the scene// オリオン星雲をシーンに追加
    planet.draw();                          // add the planet to the scene		// 惑星をシーンに追加
    ship.draw();                            // add the spaceship to the scene	// 宇宙船をシーンに追加

    graphics->spriteEnd();                  // end drawing sprites				// スプライトの描画を終了
}

//=============================================================================
// The graphics device was lost.
// グラフィックデバイスが消失した場合
// Release all reserved video memory so graphics device may be reset.
// グラフィックスデバイスをリセット可能にするため
// 予約されていたビデオメモリをすべて解放				P158
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
// グラフィックスデバイスがリセットされた場合
// Recreate all surfaces.
// すべてのサーフェイスを再作成							P159
//=============================================================================
void Spacewar::resetAll()
{
    nebulaTexture.onResetDevice();
    planetTexture.onResetDevice();
    shipTexture.onResetDevice();

    Game::resetAll();
    return;
}
