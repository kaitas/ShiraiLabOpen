// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// createThisClass.cpp v1.1
// This class is the core of the game

#include "createThisClass.h"

//=============================================================================
// Constructor
//=============================================================================
CreateThis::CreateThis()
{
    dxFont = new TextDX();  // DirectX font
    messageY = 0;
}

//=============================================================================
// Destructor
//=============================================================================
CreateThis::~CreateThis()
{
    releaseAll();           // call onLostDevice() for every graphics item
    safeDelete(dxFont);
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void CreateThis::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError

	//LoadOtogeFile
	std::ifstream ifs;  // ファイル読み取り用ストリーム
	ifs.open(OTOGE);	// 音ゲースコアファイルオープン
	if(ifs.fail()){	// ファイルオープンに失敗したらそこで終了
		 throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Loading Otoge File"));
	}
	char c;
	int i=0;
	while(i < 12387) {
		ifs >> oto[i][0] >> c >> oto[i][1] >> c >> oto[i][2] >> c >> oto[i][3];
		if(ifs.eof()) {
			break;
		}
		i++;
	}
	ifs.close();
	counter = 0; max_target = 0;

    // menu texture
    if (!menuTexture.initialize(graphics,MENU_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));
    // menu image
    if (!menu.initialize(graphics,0,0,0,&menuTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

	//Add by aki, preparing {ship, zako, boss, bomb} sprites

    if (!shipTexture.initialize(graphics,SHIP_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship texture"));
    if (!ship.initialize(graphics,16, 16, 1, &shipTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship"));
    ship.setX(GAME_WIDTH/4);
    ship.setY(GAME_HEIGHT/4);
	ship.setScale(2.0);

	//targets
	for (int i=0; i<MAX_FRAME; i++ ){
		if (oto[i][1]==1 || oto[i][2]==1 || oto[i][3]==1 ) {
		    if (!target[i].initialize(graphics,30, 30, 8, &shipTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing target"));
			target[counter].setDegrees(180);
			target[counter].setX(GAME_WIDTH/2);
			target[counter].setY(-i);
			target[counter].setScale(2);
		    target[counter].setFrames(0, 7);   // animation frames
		    target[counter].setCurrentFrame(0);     // starting frame
			target[counter].setFrameDelay(0.1);
			counter++;
		}
	}
	max_target = counter;


	//zako
    if (!zakoTexture.initialize(graphics,ZAKO_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing zako texture"));
    if (!zako.initialize(graphics,18, 17, 7, &zakoTexture))  //andor genesis
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing zako"));
    zako.setX(GAME_WIDTH*2/4);
    zako.setY(GAME_HEIGHT/4);
    zako.setFrames(0, 6);   // animation frames
	zako.setScale(2.0);
    zako.setCurrentFrame(0);     // starting frame
    zako.setFrameDelay(0.1);

	//boss
    if (!bossTexture.initialize(graphics,BOSS_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing boss texture"));
    if (!boss.initialize(graphics,99, 99, 5, &bossTexture))  //andor genesis
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing boss"));
	boss.setX(GAME_WIDTH/2 - 90);
    boss.setY(GAME_HEIGHT*3/4);
    boss.setFrames(0, 4);   // animation frames
	boss.setScale(2.0);
    boss.setCurrentFrame(0);     // starting frame
    boss.setFrameDelay(0.1);

	//bomb
    if (!bombTexture.initialize(graphics,BOMB_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bomb texture"));
    if (!bomb.initialize(graphics,30, 30, 8, &bombTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bomb"));
    bomb.setX(GAME_WIDTH/4);
    bomb.setY(GAME_HEIGHT/4);
	bomb.setScale(2.0);
    bomb.setFrames(0, 7);   // animation frames
    bomb.setCurrentFrame(0);     // starting frame
    bomb.setFrameDelay(0.1);



    // initialize DirectX font
    // 18 pixel high Arial
    if(dxFont->initialize(graphics, 18, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    menu.setDegrees(300);
    menu.setScale(0.002861f);

    message = "\n\n\nUtilizes Object Oriented C++ and DirectX\n\n";
    message += "Sprites with Transparency\n\n";
    message += "Xbox 360 Controller Input\n\n";
    message += "Collision Detection Supported:\n";
    message += "     - Circular (Distance)\n";
    message += "     - Axis-aligned bounding box\n";
    message += "     - Rotated bounding box\n";
    message += "     - Rotated bounding box and circle\n\n";
    message += "XACT Audio\n\n";
    message += "Sprite and DirectX Text\n\n";
    message += "Tile Based Graphics\n\n";
    message += "TCP/IP and UDP/IP Network Support\n\n";
    messageY = GAME_HEIGHT;

    audio->playCue(BGM);

    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void CreateThis::update()
{
    if(input->isKeyDown(SHIP_SPACE_KEY)) {
 		audio->playCue(HIT);
	}

	/*背景で何かやりたいときはこちら
	if(menu.getDegrees() > 0)
    {
        menu.setDegrees(menu.getDegrees() - frameTime * 120);
        menu.setScale(menu.getScale() + frameTime * 0.4f);
    }
    else if(messageY > -500)
    {
        menu.setDegrees(0);
        menu.setY(menu.getY() - frameTime * 50);
        messageY -= frameTime * 50;
    }
    else    // start over
    {
        menu.setDegrees(360);
        menu.setScale(0.002861f);
        menu.setX(GAME_WIDTH/2);
        menu.setY(GAME_HEIGHT/2);
        messageY = GAME_HEIGHT;
    }
	*/
	
	for (int i=0; i<max_target;i++ ) {
		target[i].setY(target[i].getY()+frameTime*100);
//		target[i].update(frameTime);
	}

	boss.update(frameTime);
	bomb.update(frameTime);


}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void CreateThis::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void CreateThis::collisions()
{}

//=============================================================================
// Render game items
//=============================================================================
void CreateThis::render()
{
    graphics->spriteBegin();                // begin drawing sprites

    menu.draw();
    ship.draw(); boss.draw(); zako.draw(); bomb.draw();    // add by Aki
	for (int i=0; i<max_target; i++) {
		target[i].draw();
	}
    dxFont->setFontColor(graphicsNS::ORANGE);
    dxFont->print(message,20,(int)messageY);

    graphics->spriteEnd();                  // end drawing sprites



}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void CreateThis::releaseAll()
{
    dxFont->onLostDevice();
    menuTexture.onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void CreateThis::resetAll()
{
    menuTexture.onResetDevice();
    dxFont->onResetDevice();
    Game::resetAll();
    return;
}
