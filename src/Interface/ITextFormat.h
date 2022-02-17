#pragma once
#include "../Common/EngineCommon.h"

MIDL_INTERFACE("9accfe1b-28f8-4c2d-a552-a623d3474155")
ITextFormat:public IUnknown
{
	virtual UINT GetAlignment() = 0;

	virtual void SetAlignment(UINT alignment) = 0;

	virtual IDWriteTextFormat* GetDWriteTextFormat() = 0;
};