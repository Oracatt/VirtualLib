#include "../Include/Texture.h"
#include "../../Extend/ResourceLoader.h"
#include "../Include/MiscFunc.h"
#include <WICTextureLoader.h>
#include <io.h>

Texture::Texture() :mRef(0) {}

HRESULT Texture::Initialize(IVirtualEngine* engine,ID3D11Device* pDevice, LPCSTR filename)
{
    if (FAILED(CreateWICTextureFromFile(pDevice, c2w(filename).c_str(), reinterpret_cast<ID3D11Resource**>(mTextureBuffer.ReleaseAndGetAddressOf()), mTextureSRView.ReleaseAndGetAddressOf())))
        return ERR_UNEXPECTED;
    return SUCCEED_OPERATE;
}

HRESULT Texture::Initialize(IVirtualEngine* engine, ID3D11Device* pDevice, IBuffer* pBuffer)
{
    if (FAILED(CreateWICTextureFromMemory(pDevice, (uint8_t*)pBuffer->GetPointer(), pBuffer->GetSize(), reinterpret_cast<ID3D11Resource**>(mTextureBuffer.ReleaseAndGetAddressOf()), mTextureSRView.ReleaseAndGetAddressOf())))
        return ERR_UNEXPECTED;
    return SUCCEED_OPERATE;
}

HRESULT _stdcall Texture::QueryInterface(const IID& iid, void** ppvObject)
{
    if (iid == IID_ITexture)
    {
        *ppvObject = static_cast<ITexture*>(this);
    }
    else
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }
    static_cast<IUnknown*>(*ppvObject)->AddRef();
    return S_OK;
}

ULONG _stdcall Texture::AddRef()
{
    return InterlockedIncrement(&mRef);
}

ULONG _stdcall Texture::Release()
{
    ULONG val = InterlockedDecrement(&mRef);
    if (val == 0)
    {
        delete this;
    }
    return val;
}

HRESULT Texture::GetBuffer(const IID& iid, void** ppvObject)
{
    if (iid == IID_ID3D11Texture2D)
    {
        *ppvObject = mTextureBuffer.Get();
    }
    else if (iid == IID_ID3D11ShaderResourceView)
    {
        *ppvObject = mTextureSRView.Get();
    }
    else if (iid == IID_IDXGISurface)
    {
        *ppvObject = mBufferSurface.Get();
    }
    else
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }
    static_cast<IUnknown*>(*ppvObject)->AddRef();
    return S_OK;
}

ID3D11ShaderResourceView* Texture::GetShaderResourceView()
{
    return mTextureSRView.Get();
}

IDXGISurface* Texture::GetBufferSurface()
{
    return mBufferSurface.Get();
}