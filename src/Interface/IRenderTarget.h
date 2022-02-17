#pragma once
#include "../Common/EngineCommon.h"
#include "ITexture.h"

MIDL_INTERFACE("635d44da-448f-4523-9374-88461553a4ce")
IRenderTarget:public ITexture
{
	virtual HRESULT Resize(const RenderTargetDesc& desc) = 0;

	virtual bool Is2DSupported() = 0;

	virtual bool IsMsaaEnabled() = 0;

	virtual USize GetSize() = 0;

	virtual ID3D11DepthStencilView* GetDepthStencilView() = 0;

	virtual ID3D11RenderTargetView* GetRenderTargetView() = 0;

	virtual ID2D1RenderTarget* GetD2DRenderTarget() = 0;

	virtual ID3D11Texture2D* GetDSBuffer() = 0;
};