#pragma once
#include "../Common/EngineCommon.h"
#include "../Interface/IRenderTarget.h"

MIDL_INTERFACE("8b08ac67-1492-43ba-a4fd-629589289ade")
IProvideRenderTarget:public IUnknown
{
	virtual IRenderTarget* GetRenderTarget() = 0;
};