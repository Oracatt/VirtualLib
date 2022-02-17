#pragma once
#include "../../Interface/IInput.h"
#include "dinput.h"

class Input :public IInput
{
public:

    HRESULT InitDirectInput(EngineDescription& desc);

    bool KeyDown(BYTE key) override;

    bool KeyPress(BYTE key) override;

    bool KeyRelease(BYTE key) override;

    bool MouseLeftDown() override;

    bool MouseMiddleDown() override;

    bool MouseRightDown() override;

    bool MouseLeftPress() override;

    bool MouseMiddlePress() override;

    bool MouseRightPress() override;

    bool MouseLeftRelease() override;

    bool MouseMiddleRelease() override;

    bool MouseRightRelease() override;

    Vector2 GetMouseDisplacement() override;

    long GetWheelDisplacement() override;

    void InputUpdate();

    HRESULT GetInputDevicePointer(const IID& iid, void** ppvObject) override;

private:

    ComPtr<IDirectInputDevice8A> lpdikey = 0;

    ComPtr<IDirectInputDevice8A> lpdimouse = 0;

    UCHAR keyboard_state[256];

    UCHAR oldkeyboard_state[256];

    DIMOUSESTATE mouse_state;

    DIMOUSESTATE oldmouse_state;

};