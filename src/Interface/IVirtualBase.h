#pragma once
#include "../Common/EngineCommon.h"

MIDL_INTERFACE("0d619ced-5034-4804-9ec6-6fb78d1ca04c")
IVirtualBase:public IUnknown
{
	virtual bool IsValid() = 0;

	virtual void Destroy() = 0;

	virtual void* GetUserData() = 0;

	virtual void SetUserData(void* pdata) = 0;
};