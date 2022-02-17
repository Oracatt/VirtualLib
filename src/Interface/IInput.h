#pragma once
#include "../Common/EngineCommon.h"

MIDL_INTERFACE("d8043428-4271-4dcb-98d3-2c51026fee97")
IInput:public IUnknown
{
    virtual bool KeyDown(BYTE key) = 0;

    virtual bool KeyPress(BYTE key) = 0;

    virtual bool KeyRelease(BYTE key) = 0;

    virtual bool MouseLeftDown() = 0;

    virtual bool MouseMiddleDown() = 0;

    virtual bool MouseRightDown() = 0;

    virtual bool MouseLeftPress() = 0;

    virtual bool MouseMiddlePress() = 0;

    virtual bool MouseRightPress() = 0;

    virtual bool MouseLeftRelease() = 0;

    virtual bool MouseMiddleRelease() = 0;

    virtual bool MouseRightRelease() = 0;

    virtual Vector2 GetMouseDisplacement() = 0;

    virtual long GetWheelDisplacement() = 0;

    virtual HRESULT GetInputDevicePointer(const IID& iid, void** ppvObject) = 0;
};