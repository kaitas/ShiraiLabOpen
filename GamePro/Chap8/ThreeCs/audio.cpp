// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// audio.cpp v1.0

#include "audio.h"

//=============================================================================
// default constructor
//=============================================================================
Audio::Audio()
{
    xactEngine = NULL;
    waveBank = NULL;
    soundBank = NULL;
    cueI = 0;
    mapWaveBank = NULL;         // Call UnmapViewOfFile() to release file
    soundBankData = NULL;

    HRESULT hr = CoInitializeEx( NULL, COINIT_MULTITHREADED );
    if( SUCCEEDED( hr ) )
        coInitialized = true;
    else
        coInitialized = false;
}

//=============================================================================
// destructor
//=============================================================================
Audio::~Audio()
{
    // Shutdown XACT
    if( xactEngine )
    {
        xactEngine->ShutDown(); // shut down XACT engine and free resources
        xactEngine->Release();
    }

    if ( soundBankData )
        delete[] soundBankData;
    soundBankData = NULL;

    // After xactEngine->ShutDown() returns, release memory mapped files
    if( mapWaveBank )
        UnmapViewOfFile( mapWaveBank );
    mapWaveBank = NULL;

    if( coInitialized )        // if CoInitializeEx succeeded
        CoUninitialize();
}

//=============================================================================
// initialize
// This function does the following:
//      1. Initialize XACT by calling xactEngine->Initialize 
//      2. Create the XACT wave bank(s) you want to use
//      3. Create the XACT sound bank(s) you want to use
//      4. Store indices to the XACT cue(s) your game uses
//=============================================================================
HRESULT Audio::initialize()
{
    HRESULT result = E_FAIL;
    HANDLE hFile;
    DWORD fileSize;
    DWORD bytesRead;
    HANDLE hMapFile;

    if( coInitialized == false)
        return E_FAIL;

    result = XACT3CreateEngine( 0, &xactEngine );
    if( FAILED( result ) || xactEngine == NULL )
        return E_FAIL;

    // Initialize & create the XACT runtime 
    XACT_RUNTIME_PARAMETERS xactParams = {0};
    xactParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
    result = xactEngine->Initialize( &xactParams );
    if( FAILED( result ) )
        return result;

    // Create an "in memory" XACT wave bank file using memory mapped file IO
    result = E_FAIL; // default to failure code, replaced on success
    hFile = CreateFile( WAVE_BANK, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        fileSize = GetFileSize( hFile, NULL );
        if( fileSize != -1 )
        {
            hMapFile = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, fileSize, NULL );
            if( hMapFile )
            {
                mapWaveBank = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
                if( mapWaveBank )
                    result = xactEngine->CreateInMemoryWaveBank( mapWaveBank, fileSize, 0, 0, &waveBank );

                CloseHandle( hMapFile );    // mapWaveBank maintains a handle on the file so close this unneeded handle
            }
        }
        CloseHandle( hFile );    // mapWaveBank maintains a handle on the file so close this unneeded handle
    }
    if( FAILED( result ) )
        return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );

    // Read and register the sound bank file with XACT.
    result = E_FAIL;    // default to failure code, replaced on success
    hFile = CreateFile( SOUND_BANK, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        fileSize = GetFileSize( hFile, NULL );
        if( fileSize != -1 )
        {
            soundBankData = new BYTE[fileSize];    // reserve memory for sound bank
            if( soundBankData )
            {
                if( 0 != ReadFile( hFile, soundBankData, fileSize, &bytesRead, NULL ) )
                    result = xactEngine->CreateSoundBank( soundBankData, fileSize, 0, 0, &soundBank );
            }
        }
        CloseHandle( hFile );
    }
    if( FAILED( result ) )
        return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );

    return S_OK;
}

//=============================================================================
// perform periodic sound engine tasks
//=============================================================================
void Audio::run()
{
    if (xactEngine == NULL)
        return;
    xactEngine->DoWork();
}

//=============================================================================
// play sound specified by cue from sound bank
// if cue does not exist no error occurs, there is simply no sound played
//=============================================================================
void Audio::playCue(const char cue[])
{
    if (soundBank == NULL)
        return;
    cueI = soundBank->GetCueIndex( cue );        // get cue index from sound bank
    soundBank->Play( cueI, 0, 0, NULL );
}

//=============================================================================
// stop a playing sound specified by cue from sound bank
// if cue does not exist no error occurs
//=============================================================================
void Audio::stopCue(const char cue[])
{
    if (soundBank == NULL)
        return;
    cueI = soundBank->GetCueIndex( cue );        // get cue index from sound bank
    soundBank->Stop( cueI, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
}
