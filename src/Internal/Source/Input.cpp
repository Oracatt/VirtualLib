#include "../Include/Input.h"
#include "dinput.h"

HRESULT Input::InitDirectInput(EngineDescription& desc)
{
    ComPtr<IDirectInput8A> lpdi;

    if (FAILED(DirectInput8Create(desc.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&lpdi, 0)))
    {
        return ERR_INITINPUT;
    }
    if (FAILED(lpdi->CreateDevice(GUID_SysKeyboard, &lpdikey, NULL)))
    {
        return ERR_INITINPUT;
    }
    lpdikey->SetCooperativeLevel(desc.outputWindow, desc.inputDesc.keyboardLevel);
    lpdikey->SetDataFormat(&c_dfDIKeyboard);
    lpdikey->Acquire();

    if (FAILED(lpdi->CreateDevice(GUID_SysMouse, &lpdimouse, NULL)))
    {
        return ERR_INITINPUT;
    }
    lpdimouse->SetCooperativeLevel(desc.outputWindow, desc.inputDesc.mouseLevel);
    lpdimouse->SetDataFormat(&c_dfDIMouse);
    lpdimouse->Acquire();

    return SUCCEED_OPERATE;
}

bool Input::KeyDown(BYTE key)
{
    return keyboard_state[key] & 0x80;
}

bool Input::KeyPress(BYTE key)
{
    return KeyDown(key) && !(oldkeyboard_state[key] & 0x80);
}

bool Input::KeyRelease(BYTE key)
{
    return (!KeyDown(key)) && (oldkeyboard_state[key] & 0x80);
}

bool Input::MouseLeftDown()
{
    return mouse_state.rgbButtons[0] & 0x80;
}

bool Input::MouseMiddleDown()
{
    return mouse_state.rgbButtons[2] & 0x80;
}

bool Input::MouseRightDown()
{
    return mouse_state.rgbButtons[1] & 0x80;
}

bool Input::MouseLeftPress()
{
    return MouseLeftDown() && !(oldmouse_state.rgbButtons[0] & 0x80);
}

bool Input::MouseMiddlePress()
{
    return MouseMiddleDown() && !(oldmouse_state.rgbButtons[2] & 0x80);
}

bool Input::MouseRightPress()
{
    return MouseRightDown() && !(oldmouse_state.rgbButtons[1] & 0x80);
}

bool Input::MouseLeftRelease()
{
    return (!MouseLeftDown()) && (oldmouse_state.rgbButtons[0] & 0x80);
}

bool Input::MouseMiddleRelease()
{
    return (!MouseMiddleDown()) && (oldmouse_state.rgbButtons[2] & 0x80);
}

bool Input::MouseRightRelease()
{
    return (!MouseRightDown()) && (oldmouse_state.rgbButtons[1] & 0x80);
}

Vector2 Input::GetMouseDisplacement()
{
    return Vector2(mouse_state.lX, mouse_state.lY);
}

long Input::GetWheelDisplacement()
{
    return mouse_state.lZ;
}

void Input::InputUpdate()
{
    memcpy(&oldmouse_state, &mouse_state, sizeof(DIMOUSESTATE));
    if (lpdimouse)
    {
        if (FAILED(lpdimouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouse_state)))
            lpdimouse->Acquire();
    }
    else
        memset(&mouse_state, 0, sizeof(mouse_state));

    memcpy(oldkeyboard_state, keyboard_state, 256);
    if (lpdikey)
    {
        if (FAILED(lpdikey->GetDeviceState(256, (LPVOID)keyboard_state)))
            lpdikey->Acquire();
    }
    else
        memset(keyboard_state, 0, sizeof(keyboard_state));
}

HRESULT Input::GetInputDevicePointer(const IID& iid, void** ppvObject)
{
    if (iid == GUID_SysKeyboard)
    {
        *ppvObject = lpdikey.Get();
    }
    else if (iid == GUID_SysMouse)
    {
        *ppvObject = lpdimouse.Get();
    }
    else
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }
    static_cast<IUnknown*>(*ppvObject)->AddRef();
    return S_OK;
}