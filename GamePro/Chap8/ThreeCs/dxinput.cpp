// Beginning Game Programming
// dxinput.cpp - DirectInput framework source file

#include "dxinput.h"

#define BUTTON_DOWN(obj, button) (obj.rgbButtons[button] & 0x80)

LPDIRECTINPUT8 dinput;
LPDIRECTINPUTDEVICE8 dimouse;
LPDIRECTINPUTDEVICE8 dikeyboard;
DIMOUSESTATE mouse_state;

//keyboard input
char keys[256];

int Init_DirectInput(HWND hwnd)
{
    //initialize DirectInput object
    HRESULT result = DirectInput8Create(
        GetModuleHandle(NULL), 
        DIRECTINPUT_VERSION, 
        IID_IDirectInput8,
        (void**)&dinput,
        NULL);

    if (result != DI_OK)
        return 0;

    //initialize the mouse
    result = dinput->CreateDevice(GUID_SysMouse, &dimouse, NULL);
    if (result != DI_OK)
        return 0;

    //initialize the keyboard
    result = dinput->CreateDevice(GUID_SysKeyboard, &dikeyboard, NULL);
    if (result != DI_OK)
        return 0;

    //clean return
    return 1;
}


int Init_Mouse(HWND hwnd)
{
    //set the data format for mouse input
    HRESULT result = dimouse->SetDataFormat(&c_dfDIMouse);
    if (result != DI_OK)
        return 0;

    //set the cooperative level
    //this will also hide the mouse pointer
    result = dimouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
    if (result != DI_OK)
        return 0;

    //acquire the mouse
    result = dimouse->Acquire();
    if (result != DI_OK)
        return 0;

    //give the go-ahead
    return 1;
}

int Mouse_X()
{
    return mouse_state.lX;
}

int Mouse_Y()
{
    return mouse_state.lY;
}

int Mouse_Button(int button)
{
    return BUTTON_DOWN(mouse_state, button);
}

void Poll_Mouse()
{
    dimouse->GetDeviceState(sizeof(mouse_state), (LPVOID)&mouse_state);
}

void Kill_Mouse()
{
    if (dimouse != NULL)
    {
        dimouse->Unacquire();
        dimouse->Release();
        dimouse = NULL;
    }
}

int Init_Keyboard(HWND hwnd)
{
    //set the data format for keyboard input
    HRESULT result = dikeyboard->SetDataFormat(&c_dfDIKeyboard);
    if (result != DI_OK)
        return 0;

    //set the cooperative level
    result = dikeyboard->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
    if (result != DI_OK)
        return 0;

    //acquire the keyboard
    result = dikeyboard->Acquire();
    if (result != DI_OK)
        return 0;

    //give the go-ahead
    return 1;
}

void Poll_Keyboard()
{
    dikeyboard->GetDeviceState(sizeof(keys), (LPVOID)&keys);
}

int Key_Down(int key)
{
    return (keys[key] & 0x80);
}

void Kill_Keyboard()
{
    if (dikeyboard != NULL)
    {
        dikeyboard->Unacquire();
        dikeyboard->Release();
        dikeyboard = NULL;
    }
}
