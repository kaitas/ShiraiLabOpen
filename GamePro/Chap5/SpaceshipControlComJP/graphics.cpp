// Programming 2D Games
// Copyright (c) 2011,2012 by:
// Charles Kelly
// Chapter 5 graphics.cpp v1.1

#include "graphics.h"

//=============================================================================
// Constructor
//=============================================================================
Graphics::Graphics()
{
    direct3d = NULL;
    device3d = NULL;
    sprite = NULL;
    fullscreen = false;
    width = GAME_WIDTH;    // width & height are replaced in initialize()
    height = GAME_HEIGHT;
    backColor = graphicsNS::BACK_COLOR;
}

//=============================================================================
// Destructor
//=============================================================================
Graphics::~Graphics()
{
    releaseAll();
}

//=============================================================================
// Release all
//=============================================================================
void Graphics::releaseAll()
{
    SAFE_RELEASE(sprite);
    SAFE_RELEASE(device3d);
    SAFE_RELEASE(direct3d);
}

//=============================================================================
// Initialize DirectX graphics
// throws GameError on error
//=============================================================================
void Graphics::initialize(HWND hw, int w, int h, bool full)
{
    hwnd = hw;
    width = w;
    height = h;
    fullscreen = full;

    //initialize Direct3D
    direct3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (direct3d == NULL)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Direct3D"));

    initD3Dpp();        // init D3D presentation parameters
    if(fullscreen)      // if full-screen mode
    {
        if(isAdapterCompatible())   // is the adapter compatible
            // set the refresh rate with a compatible one
            d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
        else
            throw(GameError(gameErrorNS::FATAL_ERROR, 
            "The graphics device does not support the specified resolution and/or format."));
    }

    // determine if graphics card supports harware texturing and lighting and vertex shaders
    D3DCAPS9 caps;
    DWORD behavior;
    result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
    // If device doesn't support HW T&L or doesn't support 1.1 vertex 
    // shaders in hardware, then switch to software vertex processing.
    if( (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
            caps.VertexShaderVersion < D3DVS_VERSION(1,1) )
        behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;  // use software only processing
    else
        behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;  // use hardware only processing

    //create Direct3D device
	// Direct3Dデバイスを作成 P132
    result = direct3d->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hwnd,
        behavior,
        &d3dpp, 
        &device3d);

    if (FAILED(result))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D device"));
 
    result = D3DXCreateSprite(device3d, &sprite);
    if (FAILED(result))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D sprite"));

}

//=============================================================================
// Initialize D3D presentation parameters
//=============================================================================
void Graphics::initD3Dpp()
{
    try{
        ZeroMemory(&d3dpp, sizeof(d3dpp));              // fill the structure with 0
        // fill in the parameters we need
        d3dpp.BackBufferWidth   = width;
        d3dpp.BackBufferHeight  = height;
        if(fullscreen)                                  // if fullscreen
            d3dpp.BackBufferFormat  = D3DFMT_X8R8G8B8;  // 24 bit color
        else
            d3dpp.BackBufferFormat  = D3DFMT_UNKNOWN;   // use desktop setting
        d3dpp.BackBufferCount   = 1;
        d3dpp.SwapEffect        = D3DSWAPEFFECT_DISCARD;
        d3dpp.hDeviceWindow     = hwnd;
        d3dpp.Windowed          = (!fullscreen);
        d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
    } catch(...)
    {
        throw(GameError(gameErrorNS::FATAL_ERROR, 
                "Error initializing D3D presentation parameters"));
    }
}

//=============================================================================
// Load the texture into default D3D memory (normal texture use)
// テクスチャをデフォルトのD3Dメモリに読み込む(通常のテクスチャで使用)
// For internal engine use only. Use the TextureManager class to load game textures.
// エンジンの使用時に内部にのみ使用します。
// ゲームテクスチャの読み込みには、TextureManagerクラスを使用します。
// Pre: filename is name of texture file.
//      transcolor is transparent color
// 実行前：filenameは、テクスチャファイルの名前
// 　　　　transcolorは、透明として扱う色
// Post: width and height = size of texture
//       texture points to texture
// 実行後：widthとheight=テクスチャの寸法
// 　　　　textureは、テクスチャを指す
// Returns HRESULT
// HRESULTを戻す       P137
//=============================================================================
HRESULT Graphics::loadTexture(const char *filename, COLOR_ARGB transcolor,
                                UINT &width, UINT &height, LP_TEXTURE &texture)
{
    // The struct for reading file info
	// ファイル情報を読み取るための構造体　P138
    D3DXIMAGE_INFO info;
    result = E_FAIL;

    try{
        if(filename == NULL)
        {
            texture = NULL;
            return D3DERR_INVALIDCALL;
        }
    
        // Get width and height from file
		// 幅と高さをファイルから取得
        result = D3DXGetImageInfoFromFile(filename, &info);
        if (result != D3D_OK)
            return result;
        width = info.Width;
        height = info.Height;
    
        // Create the new texture by loading from file
		// ファイルを読み込んで、新しいテクスチャを作成
        result = D3DXCreateTextureFromFileEx( 
            device3d,           //3D device			// 3Dデバイス
            filename,           //image filename	// 画像ファイルの名前
            info.Width,         //texture width		// テクスチャの幅
            info.Height,        //texture height	// テクスチャの高さ
            1,                  //mip-map levels (1 for no chain)	//ミップマップのレベル(チェーンなしの場合は1)
            0,                  //usage				// 使用方法(Usage)
            D3DFMT_UNKNOWN,     //surface format (default)			// サーフェイスフォーマット(デフォルト)
            D3DPOOL_DEFAULT,    //memory class for the texture		// テクスチャ用メモリの種類
            D3DX_DEFAULT,       //image filter		// 画像フィルタ
            D3DX_DEFAULT,       //mip filter		// ミップフィルタ
            transcolor,         //color key for transparency		// 透明用の色キー
            &info,              //bitmap file info (from loaded file)// ビットマップファイル情報(読み込んだファイルから)
            NULL,               //color palette		// カラーパレット
            &texture );         //destination texture// 目的のテクスチャ

    } catch(...)
    {
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
    }
    return result;
}

//=============================================================================
// Display the backbuffer
//=============================================================================
HRESULT Graphics::showBackbuffer()
{
    result = E_FAIL;    // default to fail, replace on success
    // Display backbuffer to screen
    result = device3d->Present(NULL, NULL, NULL, NULL);
    return result;
}

//=============================================================================
// Checks the adapter to see if it is compatible with the BackBuffer height,
// width and refresh rate specified in d3dpp. Fills in the pMode structure with
// the format of the compatible mode, if found.
// Pre: d3dpp is initialized.
// Post: Returns true if compatible mode found and pMode structure is filled.
//       Returns false if no compatible mode found.
//=============================================================================
bool Graphics::isAdapterCompatible()
{
    UINT modes = direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, 
                                            d3dpp.BackBufferFormat);
    for(UINT i=0; i<modes; i++)
    {
        result = direct3d->EnumAdapterModes( D3DADAPTER_DEFAULT, 
                                        d3dpp.BackBufferFormat,
                                        i, &pMode);
        if( pMode.Height == d3dpp.BackBufferHeight &&
            pMode.Width  == d3dpp.BackBufferWidth &&
            pMode.RefreshRate >= d3dpp.FullScreen_RefreshRateInHz)
            return true;
    }
    return false;
}

//=============================================================================
// Draw the sprite described in SpriteData structure
// SpriteData構造体に記述されたスプライトを描画
// Color is optional, it is applied like a filter, WHITE is default (no change)
// Color(オプション)は、フィルタのように適用される、デフォルトは白（変化なし）
// Pre : sprite->Begin() is called
// 実行前：sprite->Begin()を呼び出す
// Post: sprite->End() is called
// 実行後：sprite->End()を呼び出す
//   spriteData.rect defines the portion of spriteData.texture to draw
//   spriteData.rect は、描画するspriteData.textureの部分を定義
//   spriteData.rect.right must be right edge + 1
//   spriteData.rect.right は、右端 + 1 する必要がある
//   spriteData.rect.bottom must be bottom edge + 1
//   spriteData.rect.bottom は、下端 + 1 する必要がある					P143
//=============================================================================
void Graphics::drawSprite(const SpriteData &spriteData, COLOR_ARGB color)
{
    if(spriteData.texture == NULL)      // if no texture	//テクスチャがない場合
        return;

    // Find center of sprite			//スプライトの中心を特定
    D3DXVECTOR2 spriteCenter=D3DXVECTOR2((float)(spriteData.width/2*spriteData.scale),
                                        (float)(spriteData.height/2*spriteData.scale));
    // Screen position of the sprite	//スプライトの画面位置
    D3DXVECTOR2 translate=D3DXVECTOR2((float)spriteData.x,(float)spriteData.y);
    // Scaling X,Y						//X方向とY方向の拡大縮小
    D3DXVECTOR2 scaling(spriteData.scale,spriteData.scale);
    if (spriteData.flipHorizontal)  // if flip horizontal	//水平方向に反転する場合	P144
    {
        scaling.x *= -1;            // negative X scale to flip//Xスケールを負にして反転
        // Get center of flipped image.	//反転された画像の中心を取得
        spriteCenter.x -= (float)(spriteData.width*spriteData.scale);
        // Flip occurs around left edge, translate right to put
        // Flipped image in same location as original.
		// 左端を中心に反転が起きるので、元と同じ位置に配置するために、右へ移動します。
        translate.x += (float)(spriteData.width*spriteData.scale);
    }
    if (spriteData.flipVertical)    // if flip vertical			//垂直方向に反転する場合
    {
        scaling.y *= -1;            // negative Y scale to flip	//Yスケールを負にして反転
        // Get center of flipped image
        spriteCenter.y -= (float)(spriteData.height*spriteData.scale);
        // Flip occurs around top edge, translate down to put
        // Flipped image in same location as original.
		// 上端を中心に反転が起きるので、元と同じ位置に配置するために、下へ移動します。
        translate.y += (float)(spriteData.height*spriteData.scale);
    }
    // Create a matrix to rotate, scale and position our sprite
	//スプライトの回転、拡大縮小、配置を行うための行列を作成
    D3DXMATRIX matrix;
    D3DXMatrixTransformation2D(
        &matrix,                // the matrix			// 行列
        NULL,                   // keep origin at top left when scaling	// 拡大縮小を行うとき、原点は左上のまま
        0.0f,                   // no scaling rotation	// 拡大縮小時の回転なし
        &scaling,               // scale amount			// 拡大縮小の倍率
        &spriteCenter,          // rotation center		// 回転の中心
        (float)(spriteData.angle),  // rotation angle	// 回転の角度
        &translate);            // X,Y location			// X,Y位置

    // Tell the sprite about the matrix "Hello Neo"
	// 行列をスプライトに適用			P145
    sprite->SetTransform(&matrix);

    // Draw the sprite
	// スプライトを描画
    sprite->Draw(spriteData.texture,&spriteData.rect,NULL,NULL,color);
}

//=============================================================================
// Test for lost device
//=============================================================================
HRESULT Graphics::getDeviceState()
{ 
    result = E_FAIL;    // default to fail, replace on success
    if (device3d == NULL)
        return  result;
    result = device3d->TestCooperativeLevel(); 
    return result;
}

//=============================================================================
// Reset the graphics device
//=============================================================================
HRESULT Graphics::reset()
{
    result = E_FAIL;    // default to fail, replace on success
    initD3Dpp();                        // init D3D presentation parameters
    sprite->OnLostDevice();
    result = device3d->Reset(&d3dpp);   // attempt to reset graphics device

    sprite->OnResetDevice();
    return result;
}

//=============================================================================
// Toggle window or fullscreen mode
// Pre: All user created D3DPOOL_DEFAULT surfaces are freed.
// Post: All user surfaces are recreated.
//=============================================================================
void Graphics::changeDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
    try{
        switch(mode)
        {
        case graphicsNS::FULLSCREEN:
            if(fullscreen)      // if already in fullscreen mode
                return;
            fullscreen = true; break;
        case graphicsNS::WINDOW:
            if(fullscreen == false) // if already in window mode
                return;
            fullscreen = false; break;
        default:        // default to toggle window/fullscreen
            fullscreen = !fullscreen;
        }
        reset();
        if(fullscreen)  // fullscreen
        {
            SetWindowLong(hwnd, GWL_STYLE,  WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP);
        }
        else            // windowed
        {
            SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
            SetWindowPos(hwnd, HWND_TOP, 0,0,GAME_WIDTH,GAME_HEIGHT,
                SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

            // Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);   // get size of client area of window
            MoveWindow(hwnd,
                       0,                                           // Left
                       0,                                           // Top
                       GAME_WIDTH+(GAME_WIDTH-clientRect.right),    // Right
                       GAME_HEIGHT+(GAME_HEIGHT-clientRect.bottom), // Bottom
                       TRUE);                                       // Repaint the window
        }

    } catch(...)
    {
        // An error occured, try windowed mode 
        SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
        SetWindowPos(hwnd, HWND_TOP, 0,0,GAME_WIDTH,GAME_HEIGHT,
            SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

        // Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);   // get size of client area of window
        MoveWindow(hwnd,
                    0,                                           // Left
                    0,                                           // Top
                    GAME_WIDTH+(GAME_WIDTH-clientRect.right),    // Right
                    GAME_HEIGHT+(GAME_HEIGHT-clientRect.bottom), // Bottom
                    TRUE);                                       // Repaint the window
    }
}
