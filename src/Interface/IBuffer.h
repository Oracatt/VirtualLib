#pragma once
#include "../Common/EngineCommon.h"

MIDL_INTERFACE("993376cd-43a8-440e-b987-8c573275ef83")
IBuffer:public IUnknown
{
	virtual UINT GetSize() = 0;

	virtual void* GetPointer() = 0;
};