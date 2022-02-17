#pragma once
#include "../Common/EngineCommon.h"

MIDL_INTERFACE("9b21cb7c-cd6c-4eb7-8c95-717ba772649c")
ITexture:public IUnknown
{
	virtual HRESULT GetBuffer(const IID& iid,void** ppvObject) = 0;

	virtual ID3D11ShaderResourceView* GetShaderResourceView() = 0;

	virtual IDXGISurface* GetBufferSurface() = 0;
};