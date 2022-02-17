#pragma once
#include "../Common/EngineCommon.h"

MIDL_INTERFACE("e72b4ce5-bdc5-4df5-8f84-0b52fc16f7da")
IRenderable:public IUnknown
{
    virtual void OnFetchData() = 0;

    virtual void OnRender() = 0;

    virtual void OnRender2D() = 0;
};