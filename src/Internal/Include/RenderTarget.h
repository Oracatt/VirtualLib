#pragma once
#include "../../Interface/IRenderTarget.h"

class RenderTarget :public IRenderTarget
{
	friend class Renderer;

	friend class Engine;

public:

	RenderTarget();

	~RenderTarget();

	HRESULT Initialize(ID3D11Device* pDevice, ID2D1Factory* pD2DFactory, const RenderTargetDesc& desc);

	HRESULT Resize(const RenderTargetDesc& desc) override;

	HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) override;

	ULONG _stdcall AddRef() override;

	ULONG _stdcall Release() override;

	HRESULT GetBuffer(const IID& iid, void** ppvObject) override;

	ID3D11ShaderResourceView* GetShaderResourceView() override;

	IDXGISurface* GetBufferSurface() override;

	bool IsMsaaEnabled() override;

	bool Is2DSupported() override;

	USize GetSize() override;

	ID3D11DepthStencilView* GetDepthStencilView() override;

	ID3D11RenderTargetView* GetRenderTargetView() override;

	ID2D1RenderTarget* GetD2DRenderTarget() override;

	ID3D11Texture2D* GetDSBuffer() override;

private:

	ULONG mRef;

	USize size;

	bool canvasSupported = false;

	bool msaaEnabled = false;

	bool resizable = true;

	bool hasDepth = true;

	ComPtr<ID3D11Device> pDevice;

	ComPtr<ID2D1Factory> pD2DFactory;

	ComPtr<ID3D11Texture2D> mRenderTargetBuffer;

	ComPtr<ID3D11RenderTargetView> mRenderTargetRTView;

	ComPtr<ID3D11ShaderResourceView> mRenderTargetSRView;

	ComPtr<ID2D1RenderTarget> mD2DRenderTarget;

	ComPtr<ID3D11Texture2D> mDepthStencilBuffer;

	ComPtr<ID3D11DepthStencilView> mDepthStencilView;

	ComPtr<IDXGISurface> mBufferSurface;
};