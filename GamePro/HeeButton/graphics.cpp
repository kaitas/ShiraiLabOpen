// Programming 2D Games
// Copyright (c) 2011 by:
// Charles Kelly
// graphics.cpp v1.7
// Last modification: July-8-2013

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
    stencilSupport = false;
    width = GAME_WIDTH;    // width & height are replaced in initialize()
    height = GAME_HEIGHT;
    backColor = graphicsNS::BACK_COLOR;
    pOcclusionQuery = NULL;
    numberOfPixelsColliding = 0;
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
    safeRelease(pOcclusionQuery);
    safeRelease(sprite);
    safeRelease(device3d);
    safeRelease(direct3d);
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

    // Configure for alpha blend of primitives
    device3d->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    device3d->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device3d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    // Check if the device supports 8-bit stencil buffering. 
    if( FAILED( direct3d->CheckDeviceFormat(caps.AdapterOrdinal,
                                            caps.DeviceType,  
                                            d3dpp.BackBufferFormat,  
                                            D3DUSAGE_DEPTHSTENCIL, 
                                            D3DRTYPE_SURFACE,
                                            D3DFMT_D24S8 ) ) )
        stencilSupport = false;
    else
        stencilSupport = true;
    // Create query object, used for pixel perfect collision detection
    device3d->CreateQuery(D3DQUERYTYPE_OCCLUSION, &pOcclusionQuery);
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
        if(VSYNC)   // if vertical sync is enabled
            d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_ONE;
        else
            d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
        d3dpp.EnableAutoDepthStencil = true;
        d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;    // Depth 24, Stencil 8
    } catch(...)
    {
        throw(GameError(gameErrorNS::FATAL_ERROR, 
                "Error initializing D3D presentation parameters"));
    }
}

//=============================================================================
// Load the texture into default D3D memory (normal texture use)
// For internal engine use only. Use the TextureManager class to load game textures.
// Pre: filename is name of texture file.
//      transcolor is transparent color
// Post: width and height = size of texture
//       texture points to texture
// Returns HRESULT
//=============================================================================
HRESULT Graphics::loadTexture(const char *filename, COLOR_ARGB transcolor,
                                UINT &width, UINT &height, LP_TEXTURE &texture)
{
    // The struct for reading file info
    D3DXIMAGE_INFO info;
    result = E_FAIL;

    try{
        if(filename == NULL)
        {
            texture = NULL;
            return D3DERR_INVALIDCALL;
        }
    
        // Get width and height from file
        result = D3DXGetImageInfoFromFile(filename, &info);
        if (result != D3D_OK)
            return result;
        width = info.Width;
        height = info.Height;
    
        // Create the new texture by loading from file
        result = D3DXCreateTextureFromFileEx( 
            device3d,           //3D device
            filename,           //image filename
            info.Width,         //texture width
            info.Height,        //texture height
            1,                  //mip-map levels (1 for no chain)
            0,                  //usage
            D3DFMT_UNKNOWN,     //surface format (default)
            D3DPOOL_DEFAULT,    //memory class for the texture
            D3DX_DEFAULT,       //image filter
            D3DX_DEFAULT,       //mip filter
            transcolor,         //color key for transparency
            &info,              //bitmap file info (from loaded file)
            NULL,               //color palette
            &texture );         //destination texture

    } catch(...)
    {
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
    }
    return result;
}

//=============================================================================
// Load the texture into system memory (system memory is lockable)
// Provides direct access to pixel data. Use the TextureManager class to load
// textures for display.
// Pre: filename is name of texture file.
//      transcolor transparent color
// Post: width and height = size of texture
//       texture points to texture
// Returns HRESULT and fills TextureData structure.
//=============================================================================
HRESULT Graphics::loadTextureSystemMem(const char *filename, COLOR_ARGB transcolor, 
                                    UINT &width, UINT &height, LP_TEXTURE &texture)
{
    // The struct for reading bitmap file info
    D3DXIMAGE_INFO info;
    result = E_FAIL;        // Standard Windows return value

    try{
        if(filename == NULL)
        {
            texture = NULL;
            return D3DERR_INVALIDCALL;
        }
 
        // Get width and height from bitmap file
        result = D3DXGetImageInfoFromFile(filename, &info);
        if (result != D3D_OK)
            return result;
        width = info.Width;
        height = info.Height;

        // Create the new texture by loading a bitmap image file
        result = D3DXCreateTextureFromFileEx( 
            device3d,           //3D device
            filename,           //bitmap filename
            info.Width,         //bitmap image width
            info.Height,        //bitmap image height
            1,                  //mip-map levels (1 for no chain)
            0,                  //usage
            D3DFMT_UNKNOWN,     //surface format (default)
            D3DPOOL_SYSTEMMEM,  //systemmem is lockable
            D3DX_DEFAULT,       //image filter
            D3DX_DEFAULT,       //mip filter
            transcolor,         //color key for transparency
            &info,              //bitmap file info (from loaded file)
            NULL,               //color palette
            &texture );         //destination texture

    } catch(...)
    {
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
    }
    return result;
}

//=============================================================================
// Create a vertex buffer.
// Pre: verts[] contains vertex data.
//      size = size of verts[]
// Post: &vertexBuffer points to buffer if successful.
//=============================================================================
HRESULT Graphics::createVertexBuffer(VertexC verts[], UINT size, LP_VERTEXBUFFER &vertexBuffer)
{
    // Standard Windows return value
    HRESULT result = E_FAIL;

    // Create a vertex buffer
    result = device3d->CreateVertexBuffer(size, D3DUSAGE_WRITEONLY, D3DFVF_VERTEX,
                                            D3DPOOL_DEFAULT, &vertexBuffer, NULL);
    if(FAILED(result))
        return result;

    void *ptr;
    // must lock buffer before data can be transferred in
    result = vertexBuffer->Lock(0, size, (void**)&ptr, 0);
    if(FAILED(result))
        return result;
    memcpy(ptr, verts, size);   // copy vertex data into buffer
    vertexBuffer->Unlock();     // unlock buffer

    return result;
}

//=============================================================================
// Display a quad with alpha transparency using Triangle Fan
// Pre: createVertexBuffer was used to create vertexBuffer containing four
//      vertices defining the quad in clockwise order.
//      g3ddev->BeginScene was called
// Post: Quad is drawn
//=============================================================================
bool Graphics::drawQuad(LP_VERTEXBUFFER vertexBuffer)
{
    HRESULT result = E_FAIL;    // standard Windows return value

    if(vertexBuffer == NULL)
        return false;
     
    device3d->SetRenderState(D3DRS_ALPHABLENDENABLE, true); // enable alpha blend

    device3d->SetStreamSource(0, vertexBuffer, 0, sizeof(VertexC));
    device3d->SetFVF(D3DFVF_VERTEX);
    result = device3d->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

    device3d->SetRenderState(D3DRS_ALPHABLENDENABLE, false); // alpha blend off
    
    if(FAILED(result))
        return false;

    return true;
}

//=============================================================================
// Display the backbuffer
//=============================================================================
HRESULT Graphics::showBackbuffer()
{
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
// Color is optional, it is applied like a filter, WHITE is default (no change)
// Pre : sprite->Begin() is called
// Post: sprite->End() is called
// spriteData.rect defines the portion of spriteData.texture to draw
//   spriteData.rect.right must be right edge + 1
//   spriteData.rect.bottom must be bottom edge + 1
//=============================================================================
void Graphics::drawSprite(const SpriteData &spriteData, COLOR_ARGB color)
{
    if(spriteData.texture == NULL)      // if no texture
        return;

    // Find center of sprite
    D3DXVECTOR2 spriteCenter=D3DXVECTOR2((float)(spriteData.width/2*spriteData.scale),
                                        (float)(spriteData.height/2*spriteData.scale));
    // Screen position of the sprite
    D3DXVECTOR2 translate=D3DXVECTOR2((float)spriteData.x,(float)spriteData.y);
    // Scaling X,Y
    D3DXVECTOR2 scaling(spriteData.scale,spriteData.scale);
    if (spriteData.flipHorizontal)  // if flip horizontal
    {
        scaling.x *= -1;            // negative X scale to flip
        // Get center of flipped image.
        spriteCenter.x -= (float)(spriteData.width*spriteData.scale);
        // Flip occurs around left edge, translate right to put
        // Flipped image in same location as original.
        translate.x += (float)(spriteData.width*spriteData.scale);
    }
    if (spriteData.flipVertical)    // if flip vertical
    {
        scaling.y *= -1;            // negative Y scale to flip
        // Get center of flipped image
        spriteCenter.y -= (float)(spriteData.height*spriteData.scale);
        // Flip occurs around top edge, translate down to put
        // Flipped image in same location as original.
        translate.y += (float)(spriteData.height*spriteData.scale);
    }
    // Create a matrix to rotate, scale and position our sprite
    D3DXMATRIX matrix;
    D3DXMatrixTransformation2D(
        &matrix,                // the matrix
        NULL,                   // keep origin at top left when scaling
        0.0f,                   // no scaling rotation
        &scaling,               // scale amount
        &spriteCenter,          // rotation center
        (float)(spriteData.angle),  // rotation angle
        &translate);            // X,Y location

    // Tell the sprite about the matrix "Hello Neo"
    sprite->SetTransform(&matrix);

    // Draw the sprite
    sprite->Draw(spriteData.texture,&spriteData.rect,NULL,NULL,color);
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

//=============================================================================
// Return the number of pixels colliding between the two sprites.
// Pre: The device supports a stencil buffer and pOcclusionQuery points to
// a valid occlusionQuery object.
// Post: Returns the number of pixels of overlap
// This function waits for the graphics card to render the last frame and return
// the collision query pixel count. To avoid slowing down your game, use a
// simple collison test first to eliminate entities that are not colliding.
// Using pixel perfect collision test with multiple entities would require
// creating multiple occlusionQuery objects and clearing the stencil buffer
// between each test.
//=============================================================================
DWORD Graphics::pixelCollision(const SpriteData &sprite1, const SpriteData &sprite2)
{
    if(!stencilSupport)     // if no stencil buffer support
        return 0;

    beginScene();

    // Set up stencil buffer using current entity
    device3d->SetRenderState(D3DRS_STENCILENABLE,   true);
    device3d->SetRenderState(D3DRS_STENCILFUNC,     D3DCMP_ALWAYS);
    device3d->SetRenderState(D3DRS_STENCILREF,      0x1);
    device3d->SetRenderState(D3DRS_STENCILMASK,     0xffffffff);
    device3d->SetRenderState(D3DRS_STENCILWRITEMASK,0xffffffff);
    device3d->SetRenderState(D3DRS_STENCILFAIL,     D3DSTENCILOP_KEEP);
    device3d->SetRenderState(D3DRS_STENCILPASS,     D3DSTENCILOP_REPLACE);

    // Write a 1 into the stencil buffer for each non-transparent pixel in ent
    spriteBegin();
    // Enable stencil buffer (must be after spriteBegin)
    device3d->SetRenderState(D3DRS_STENCILENABLE,   true);
    drawSprite(sprite2);            // write 1s to stencil buffer
    spriteEnd();

    // Change stencil buffer to only allow writes where the stencil value is 1
    // (where the ent sprite is colliding with the current sprite)
    device3d->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
    device3d->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
    
    // Begin occlusion query to count pixels that are drawn
    pOcclusionQuery->Issue(D3DISSUE_BEGIN);

    spriteBegin();
    // Enable stencil buffer (must be after spriteBegin)
    device3d->SetRenderState(D3DRS_STENCILENABLE,   true);
    drawSprite(sprite1);            // draw current entity 
    spriteEnd();
    // End occlusion query
    pOcclusionQuery->Issue(D3DISSUE_END);

    // Wait until the GPU is finished.
    while(S_FALSE == pOcclusionQuery->GetData( &numberOfPixelsColliding, 
                                  sizeof(DWORD), D3DGETDATA_FLUSH ))
    {}

    // Turn off stencil
    device3d->SetRenderState(D3DRS_STENCILENABLE, false);

    endScene();
    return numberOfPixelsColliding;
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
    safeRelease(pOcclusionQuery);       // release query
    initD3Dpp();                        // init D3D presentation parameters
    sprite->OnLostDevice();             // release sprite
    result = device3d->Reset(&d3dpp);   // attempt to reset graphics device

    // Configure for alpha blend of primitives
    device3d->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    device3d->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device3d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    // recreate query
    device3d->CreateQuery(D3DQUERYTYPE_OCCLUSION, &pOcclusionQuery);
    sprite->OnResetDevice();            // recreate sprite
    return result;
}
