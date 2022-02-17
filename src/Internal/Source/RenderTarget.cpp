#include "../Include/RenderTarget.h"

class MultiThreadProtector
{
public:

    MultiThreadProtector(ID3D11Multithread* p) :m_pro(p)
    {
        m_pro->Enter();
    }

    ~MultiThreadProtector()
    {
        m_pro->Leave();
    }

private:

    ComPtr<ID3D11Multithread> m_pro;
};

RenderTarget::RenderTarget() :mRef(0) {}

RenderTarget::~RenderTarget()
{
    if (pDevice)
    {
        ComPtr<ID3D11Multithread> mProtector;
        pDevice.As(&mProtector);
        MultiThreadProtector pro(mProtector.Get());
        pDevice.Reset();
        pD2DFactory.Reset();
        mRenderTargetBuffer.Reset();
        mRenderTargetRTView.Reset();
        mRenderTargetSRView.Reset();
        mD2DRenderTarget.Reset();
        mDepthStencilBuffer.Reset();
        mDepthStencilView.Reset();
        mBufferSurface.Reset();
    }
}

HRESULT RenderTarget::Initialize(ID3D11Device* _pDevice, ID2D1Factory* _pD2DFactory, const RenderTargetDesc& desc)
{
    pDevice = _pDevice;
    pD2DFactory = _pD2DFactory;
    hasDepth = desc.hasDepth;
    D3D11_TEXTURE2D_DESC rtDesc;
    ZeroMemory(&rtDesc, sizeof(rtDesc));
    rtDesc.ArraySize = 1;
    rtDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    rtDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    rtDesc.Width = desc.width;
    rtDesc.Height = desc.height;
    rtDesc.MipLevels = 1;
    rtDesc.MiscFlags = desc.support2D ? D3D11_RESOURCE_MISC_GDI_COMPATIBLE : 0;
    if (desc.msaaEnabled)
    {
        rtDesc.SampleDesc.Count = desc.multiSampleCount;
        UINT  msaaQuality;
        if (FAILED(pDevice->CheckMultisampleQualityLevels(
            DXGI_FORMAT_B8G8R8A8_UNORM,
            desc.multiSampleCount,
            &msaaQuality)) || msaaQuality < 1)
        {
            return ERR_INVALIDARG;
        }
        rtDesc.SampleDesc.Quality = msaaQuality - 1;
    }
    else
    {
        rtDesc.SampleDesc.Count = 1;
        rtDesc.SampleDesc.Quality = 0;
    }
    rtDesc.SampleDesc.Count = 1;
    rtDesc.SampleDesc.Quality = 0;
    rtDesc.Usage = D3D11_USAGE_DEFAULT;
    if (FAILED(pDevice->CreateTexture2D(&rtDesc, 0, &mRenderTargetBuffer)))
        return ERR_INVALIDARG;
    pDevice->CreateRenderTargetView(mRenderTargetBuffer.Get(), 0, &mRenderTargetRTView);
    pDevice->CreateShaderResourceView(mRenderTargetBuffer.Get(), 0, &mRenderTargetSRView);
    mRenderTargetBuffer.As(&mBufferSurface);

    if (hasDepth)
    {
        D3D11_TEXTURE2D_DESC depthStencilDesc;
        ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
        depthStencilDesc.Width = desc.width;
        depthStencilDesc.Height = desc.height;
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.ArraySize = 1;
        depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        if (desc.msaaEnabled)
        {
            depthStencilDesc.SampleDesc.Count = rtDesc.SampleDesc.Count;
            depthStencilDesc.SampleDesc.Quality = rtDesc.SampleDesc.Quality;
            msaaEnabled = true;
        }
        else
        {
            depthStencilDesc.SampleDesc.Count = 1;
            depthStencilDesc.SampleDesc.Quality = 0;
            msaaEnabled = false;
        }
        depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
        depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthStencilDesc.CPUAccessFlags = 0;
        depthStencilDesc.MiscFlags = 0;
        if (FAILED(pDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer)))
            return ERR_INVALIDARG;
        pDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), 0, &mDepthStencilView);
    }

    if (desc.support2D)
    {
        ComPtr<ID3D11Multithread> mProtector;
        pDevice.As(&mProtector);
        MultiThreadProtector pro(mProtector.Get());
        canvasSupported = true;
        D2D1_RENDER_TARGET_PROPERTIES props = RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
            PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
        if (FAILED(pD2DFactory->CreateDxgiSurfaceRenderTarget(mBufferSurface.Get(), &props, &mD2DRenderTarget)))
            return ERR_INVALIDARG;
    }
    size = { desc.width,desc.height };
    return SUCCEED_OPERATE;
}

HRESULT RenderTarget::Resize(const RenderTargetDesc& desc)
{
    if (resizable)
        return Initialize(pDevice.Get(), pD2DFactory.Get(), desc);
    return ERR_UNEXPECTED;
}

HRESULT _stdcall RenderTarget::QueryInterface(const IID& iid, void** ppvObject)
{
    if (iid == IID_IRenderTarget)
    {
        *ppvObject = static_cast<IRenderTarget*>(this);
    }
    else if (iid == IID_ITexture)
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

ULONG _stdcall RenderTarget::AddRef()
{
    return InterlockedIncrement(&mRef);
}

ULONG _stdcall RenderTarget::Release()
{
    ULONG val = InterlockedDecrement(&mRef);
    if (val == 0)
    {
        delete this;
    }
    return val;
}

HRESULT RenderTarget::GetBuffer(const IID& iid, void** ppvObject)
{
    if (iid == IID_ID3D11Texture2D)
    {
        *ppvObject = mRenderTargetBuffer.Get();
    }
    else if (iid == IID_ID3D11RenderTargetView)
    {
        *ppvObject = mRenderTargetRTView.Get();
    }
    else if (iid == IID_ID3D11ShaderResourceView)
    {
        *ppvObject = mRenderTargetSRView.Get();
    }
    else if (iid == IID_ID2D1RenderTarget)
    {
        *ppvObject = mD2DRenderTarget.Get();
        if (!mD2DRenderTarget)
            return E_NOINTERFACE;
    }
    else if (iid == IID_ID3D11DepthStencilView)
    {
        *ppvObject = mDepthStencilView.Get();
        if (!mDepthStencilView)
            return E_NOINTERFACE;
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

ID3D11ShaderResourceView* RenderTarget::GetShaderResourceView()
{
    return mRenderTargetSRView.Get();
}

IDXGISurface* RenderTarget::GetBufferSurface()
{
    return mBufferSurface.Get();
}

bool RenderTarget::IsMsaaEnabled()
{
    return msaaEnabled;
}

bool RenderTarget::Is2DSupported()
{
    return canvasSupported;
}

USize RenderTarget::GetSize()
{
    return size;
}

ID3D11DepthStencilView* RenderTarget::GetDepthStencilView()
{
    return mDepthStencilView.Get();
}

ID3D11RenderTargetView* RenderTarget::GetRenderTargetView()
{
    return mRenderTargetRTView.Get();
}

ID2D1RenderTarget* RenderTarget::GetD2DRenderTarget()
{
    return mD2DRenderTarget.Get();
}

ID3D11Texture2D* RenderTarget::GetDSBuffer()
{
    return mDepthStencilBuffer.Get();
}