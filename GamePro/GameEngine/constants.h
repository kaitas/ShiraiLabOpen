// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Game Engine constants.h v3.1
// Last modification: Dec-24-2013

#pragma once
#ifndef _CONSTANTS_H            // Prevent multiple definitions if this 
#define _CONSTANTS_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

//=============================================================================
//                  Constants
//=============================================================================

// window
const char CLASS_NAME[] = "createThisClass";
const char GAME_TITLE[] = "Game Engine v3.1 Demo";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH =  640;               // width of game in pixels
const UINT GAME_HEIGHT = 480;               // height of game in pixels
 
// game
const bool VSYNC = false;                   // true locks display to vertical sync rate
const double PI = 3.14159265;
const float FRAME_RATE = 100.0f;                // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f/FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f/MIN_FRAME_RATE; // maximum time used in calculations

// graphic images
//const char MENU_IMAGE[] =     "pictures\\menu.png";      // menu texture
const char MENU_IMAGE[] =     "pictures\\LV2015akiL.jpg";      // my texture
const char SHIP_IMAGE[] = "pictures\\solvalou3.png"; 
const char BOSS_IMAGE[] = "pictures\\42386.png"; // Andor Genesis
const char ZAKO_IMAGE[] = "pictures\\kapi.png"; 
const char BOMB_IMAGE[] = "pictures\\solvalou-exp.png"; 

// audio files required by audio.cpp
// WAVE_BANK must be location of .xwb file.
const char WAVE_BANK[]  = "audio\\Win\\Wave Bank.xwb";
// SOUND_BANK must be location of .xsb file.
const char SOUND_BANK[] = "audio\\Win\\Sound Bank.xsb";

// audio cues
const char BGM[] = "marcus_kellis_theme"; //marcus_kellis_theme
const char HIT[] = "Hit03-1"; // from http://musicisvfr.com/free/se/hit01.html
// otoge file
const char OTOGE[] = "otoge100fps.txt"; //音ゲー用スコアファイル


// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR CONSOLE_KEY  = '`';         // ` key
const UCHAR ESC_KEY      = VK_ESCAPE;   // escape key
const UCHAR ALT_KEY      = VK_MENU;     // Alt key
const UCHAR ENTER_KEY    = VK_RETURN;   // Enter key
//add by aki
const UCHAR SHIP_LEFT_KEY    = VK_LEFT;     // left arrow
const UCHAR SHIP_RIGHT_KEY   = VK_RIGHT;    // right arrow
const UCHAR SHIP_UP_KEY      = VK_UP;       // up arrow
const UCHAR SHIP_DOWN_KEY    = VK_DOWN;     // down arrow
const UCHAR SHIP_SPACE_KEY   = VK_SPACE;	// スペースキー
//=============================================================================
// Function templates for safely dealing with pointer referenced items.
// The functions defined by these templates may be called using a normal
// function call syntax. The compiler will create a function that replaces T
// with the type of the calling parameter.
//=============================================================================
// Safely release pointer referenced item
template <typename T>
inline void safeRelease(T& ptr)
{
    if (ptr)
    { 
        ptr->Release(); 
        ptr = NULL;
    }
}
#define SAFE_RELEASE safeRelease            // for backward compatiblility

// Safely delete pointer referenced item
template <typename T>
inline void safeDelete(T& ptr)
{
    if (ptr)
    { 
        delete ptr; 
        ptr = NULL;
    }
}
#define SAFE_DELETE safeDelete              // for backward compatiblility

// Safely delete pointer referenced array
template <typename T>
inline void safeDeleteArray(T& ptr)
{
    if (ptr)
    { 
        delete[] ptr; 
        ptr = NULL;
    }
}
#define SAFE_DELETE_ARRAY safeDeleteArray   // for backward compatiblility

// Safely call onLostDevice
template <typename T>
inline void safeOnLostDevice(T& ptr)
{
    if (ptr)
        ptr->onLostDevice(); 
}
#define SAFE_ON_LOST_DEVICE safeOnLostDevice    // for backward compatiblility

// Safely call onResetDevice
template <typename T>
inline void safeOnResetDevice(T& ptr)
{
    if (ptr)
        ptr->onResetDevice(); 
}
#define SAFE_ON_RESET_DEVICE safeOnResetDevice  // for backward compatiblility

#endif
