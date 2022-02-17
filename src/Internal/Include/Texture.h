#pragma once
#include "../../Interface/ITexture.h"
#include "../../Interface/IVirtualEngine.h"
#include "../../Interface/IBuffer.h"

class Texture :public ITexture
{
public:

	Texture();

	HRESULT Initialize(IVirtualEngine* engine, ID3D11Device* pDevice, LPCSTR filename);

	HRESULT Initialize(IVirtualEngine* engine, ID3D11Device* pDevice, IBuffer* pBuffer);

	HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) override;

	ULONG _stdcall AddRef() override;

	ULONG _stdcall Release() override;

	HRESULT GetBuffer(const IID& iid, void** ppvObject) override;

	ID3D11ShaderResourceView* GetShaderResourceView() override;

	IDXGISurface* GetBufferSurface() override;

private:

	ULONG mRef;

	ComPtr<ID3D11Texture2D> mTextureBuffer;

	ComPtr<ID3D11ShaderResourceView> mTextureSRView;

	ComPtr<IDXGISurface> mBufferSurface;
};