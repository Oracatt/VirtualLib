#pragma once
#include "../../Common/EngineDef.h"
#include "../../Extend/EngineDefault.h"
#include "../Include/Engine.h"
#include "../Include/Scene.h"
#include "../Include/RenderTarget.h"
#include "../Include/Texture.h"
#include "../Include/TextFormat.h"
#include "../Include/MiscFunc.h"
#include "../Include/RenderTargetTreeNode.h"
#include "../Include/RenderTargetTreeView.h"
#include "../../Extend/ResourceLoader.h"
#include "../Include/Buffer.h"

vector<Engine*> engineArray;
DefaultEngineErrorCallBack defaultErrorCallBack;

Engine::Engine(EngineDescription& desc) :AudioPlayer(this), Timer(this), PhysicsManager(this), Renderer(this)
{
    mRef = 0;
    mDesc = desc;
    thispointer = this;
    logicLock = CreateSemaphore(NULL, 0, 10, NULL);
    renderLock = CreateSemaphore(NULL, 1, 10, NULL);
    renderEnd = CreateSemaphore(NULL, 0, 10, NULL);
    rtTreeMutex = CreateMutex(0, FALSE, "rtTreeMutex");
    rendererDelegateMutex = CreateMutex(0, FALSE, "rendererDelegateMutex");
    sceneDelegateMutex = CreateMutex(0, FALSE, "sceneDelegateMutex");
}

void Engine::ReportFatalError(LPCSTR msg, int errcode)
{
    mDesc.errorCallBack->ReportFatalError(msg, errcode);
}

void Engine::ReportError(LPCSTR msg)
{
    mDesc.errorCallBack->ReportError(msg);
}

void Engine::Start(IScene* scene)
{
    SetNowScene(scene);
    for (auto e : eventCallBacks)
        e->OnEngineInitialized(this);
    MessageLoop();
}

EngineAttributes Engine::GetAttributes()
{
    return mDesc.attributes;
}

void Engine::SetAttributes(EngineAttributes& attr)
{
    mDesc.attributes = attr;
}

vector<EngineEventCallBack*> Engine::GetEventCallBacks()
{
    return eventCallBacks;
}

void Engine::AddEventCallBack(EngineEventCallBack* callback)
{
    eventCallBacks.push_back(callback);
}

void Engine::RemoveEventCallBack(EngineEventCallBack* callback)
{
    for (vector<EngineEventCallBack*>::iterator it = eventCallBacks.begin(); it != eventCallBacks.end();)
    {
        if (*it == callback)
            it = eventCallBacks.erase(it);
        else
            it++;
    }
}

HRESULT Engine::CreateScene(const SceneDesc* desc, IScene** ppScene)
{
    if (quitIsSignalled)
    {
        *ppScene = 0;
        return ERR_UNEXPECTED;
    }
    HRESULT hr;
    Scene* scene = new Scene(this);
    scene->AddRef();
    if (desc)
        hr = scene->InitPxScene(&desc->pxSceneDesc);
    else
        hr = scene->InitPxScene(0);
    if (FAILED(hr))
    {
        *ppScene = 0;
        scene->Destroy();
        scene->Release();
        return hr;
    }
    *ppScene = static_cast<IScene*>(scene);
    mScenes.emplace_back(*ppScene);
    return SUCCEED_OPERATE;
}

IScene* Engine::GetNowScene()
{
    return nowScene.Get();
}

void Engine::SetNowScene(IScene* pScene)
{
    if (nowScene)
    {
        nowScene->WriteMessageStream(SMSG_CHANGESCENE, pScene, 0);
    }
    else
    {
        nowScene = pScene;
        if (pScene)
            pScene->WriteMessageStream(SMSG_SCENEAWAKE, 0, 0);
    }
}

bool Engine::IsValid()
{
    return isValid;
}

void Engine::Destroy()
{
    if (isValid)
    {
        isValid = false;
        quitIsSignalled = true;
        while (mThreadNum)
            Sleep(1);
        ReleaseDirect3D();
        ReleasePhysX();
        for (vector<Engine*>::iterator it = engineArray.begin(); it != engineArray.end();)
        {
            if ((*it) == this)
                it = engineArray.erase(it);
            else
                it++;
        }
        rendererDelegates.clear();
        CloseHandle(logicLock);
        CloseHandle(renderLock);
        CloseHandle(renderEnd);
        CloseHandle(rtTreeMutex);
        CloseHandle(rendererDelegateMutex);
        CloseHandle(sceneDelegateMutex);
        for (auto e : eventCallBacks)
            e->OnEngineDestroy();
        if (autoCreateWindow)
        {
            HWND hwnd;
            if (hwnd = FindWindow(mDesc.className, mDesc.windowName))
                DestroyWindow(hwnd);
            UnregisterClassA(mDesc.className, mDesc.hInstance);
        }
        CoUninitialize();
        thispointer.Reset();
    }
}

void* Engine::GetUserData()
{
    return userdata;
}

void Engine::SetUserData(void* pdata)
{
    userdata = pdata;
}

void Engine::BroadcastMessage(BYTE msgType, LPVOID param1, LPVOID param2)
{
    for (vector<ComPtr<IScene>>::iterator it = mScenes.begin(); it != mScenes.end(); it++)
    {
        ComPtr<IScene> scene = *it;
        (*it)->Invoke(Delegate([=]() {scene->WriteMessageStream(msgType, param1, param2); }));
    }
}

HRESULT Engine::CreateRenderTarget(const RenderTargetDesc& desc, IRenderTarget** ppRenderTarget)
{
    RenderTarget* rt = new RenderTarget;
    rt->AddRef();
    HRESULT hr;
    if (FAILED(hr = rt->Initialize(pDevice.Get(), pD2DFactory.Get(), desc)))
    {
        *ppRenderTarget = 0;
        rt->Release();
        return hr;
    }
    *ppRenderTarget = rt;
    return hr;
}

HRESULT Engine::CreateTextureFromFile(LPCSTR filename, ITexture** ppTexture)
{
    if (_access(filename, 0) == -1)
    {
        ComPtr<IBuffer> pBuffer;
        if (FAILED(CreateBuffer(filename, &pBuffer)))
        {
            *ppTexture = 0;
            char msg[512];
            sprintf(msg, "未能加载纹理文件：%hs", filename);
            ReportFatalError(msg, FE_FILENOTFOUND);
            return ERR_ACCESS;
        }
        return CreateTextureFromBuffer(pBuffer.Get(), ppTexture);
    }
    Texture* tex = new Texture;
    tex->AddRef();
    HRESULT hr;
    if (FAILED(hr = tex->Initialize(static_cast<IVirtualEngine*>(this), pDevice.Get(), filename)))
    {
        *ppTexture = 0;
        tex->Release();
        char msg[512];
        sprintf(msg, "未能加载纹理文件：%hs", filename);
        ReportFatalError(msg, FE_CANNOTOPENFILE);
        return hr;
    }
    *ppTexture = tex;
    return hr;
}

HRESULT Engine::CreateTextureFromBuffer(IBuffer* pBuffer, ITexture** ppTexture)
{
    if (!pBuffer)
    {
        *ppTexture = 0;
        return ERR_POINTER;
    }
    Texture* tex = new Texture;
    tex->AddRef();
    HRESULT hr;
    if (FAILED(hr = tex->Initialize(static_cast<IVirtualEngine*>(this), pDevice.Get(), pBuffer)))
    {
        *ppTexture = 0;
        tex->Release();
        char msg[512];
        sprintf(msg, "未能加载纹理文件：%p(size=%d)", pBuffer->GetPointer(), pBuffer->GetSize());
        ReportFatalError(msg, FE_CANNOTOPENFILE);
        return hr;
    }
    *ppTexture = tex;
    return hr;
}

HRESULT Engine::CreateTextFormat(LPCSTR lpFaceName, float fontSize, ITextFormat** ppTextFormat)
{
    TextFormat* format = new TextFormat;
    format->AddRef();
    HRESULT hr;
    if (FAILED(hr = format->Initialize(pDWriteFactory.Get(), lpFaceName, fontSize)))
    {
        *ppTextFormat = 0;
        format->Release();
        char msg[512];
        sprintf(msg, "未能加载字体文件：%hs", lpFaceName);
        ReportFatalError(msg, FE_CANNOTOPENFILE);
        return hr;
    }
    *ppTextFormat = format;
    return hr;
}

HRESULT Engine::CreateTextFormat(LPCSTR lpFaceName, float fontSize, DWRITE_FONT_WEIGHT weight,
    DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, LPCSTR localeName, ITextFormat** ppTextFormat)
{
    TextFormat* format = new TextFormat;
    format->AddRef();
    HRESULT hr;
    if (FAILED(hr = format->Initialize(pDWriteFactory.Get(), lpFaceName, fontSize, weight, style, stretch, localeName)))
    {
        *ppTextFormat = 0;
        format->Release();
        char msg[512];
        sprintf(msg, "未能加载字体文件：%hs", lpFaceName);
        ReportFatalError(msg, FE_CANNOTOPENFILE);
        return hr;
    }
    *ppTextFormat = format;
    return hr;
}

HRESULT Engine::CreateTextFormatFromFile(LPCSTR filename, LPCSTR lpFaceName, float fontSize, ITextFormat** ppTextFormat)
{
    if (_access(filename, 0) == -1)
    {
        ComPtr<IBuffer> pBuffer;
        if (FAILED(CreateBuffer(filename, &pBuffer)))
        {
            *ppTextFormat = 0;
            char msg[512];
            sprintf(msg, "未能加载字体文件：%hs", filename);
            ReportFatalError(msg, FE_FILENOTFOUND);
            return ERR_ACCESS;
        }
        return CreateTextFormatFromBuffer(pBuffer.Get(), lpFaceName, fontSize, ppTextFormat);
    }
    TextFormat* format = new TextFormat;
    format->AddRef();
    HRESULT hr;
    if (FAILED(hr = format->Initialize(pDWriteFactory.Get(), lpFaceName, fontSize, filename)))
    {
        *ppTextFormat = 0;
        format->Release();
        char msg[512];
        sprintf(msg, "未能加载字体文件：%hs", filename);
        ReportFatalError(msg, FE_CANNOTOPENFILE);
        return hr;
    }
    *ppTextFormat = format;
    return hr;
}

HRESULT Engine::CreateTextFormatFromFile(LPCSTR filename, LPCSTR lpFaceName, float fontSize, DWRITE_FONT_WEIGHT weight,
    DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, LPCSTR localeName, ITextFormat** ppTextFormat)
{
    if (_access(filename, 0) == -1)
    {
        ComPtr<IBuffer> pBuffer;
        if (FAILED(CreateBuffer(filename, &pBuffer)))
        {
            *ppTextFormat = 0;
            char msg[512];
            sprintf(msg, "未能加载字体文件：%hs", filename);
            ReportFatalError(msg, FE_FILENOTFOUND);
            return ERR_ACCESS;
        }
        return CreateTextFormatFromBuffer(pBuffer.Get(), lpFaceName,
            fontSize, weight, style, stretch, localeName, ppTextFormat);
    }
    TextFormat* format = new TextFormat;
    format->AddRef();
    HRESULT hr;
    if (FAILED(hr = format->Initialize(pDWriteFactory.Get(), lpFaceName, fontSize, weight, style, stretch, localeName, filename)))
    {
        *ppTextFormat = 0;
        format->Release();
        char msg[512];
        sprintf(msg, "未能加载字体文件：%hs", filename);
        ReportFatalError(msg, FE_CANNOTOPENFILE);
        return hr;
    }
    *ppTextFormat = format;
    return hr;
}

HRESULT Engine::CreateTextFormatFromBuffer(IBuffer* pBuffer, LPCSTR lpFaceName, float fontSize, ITextFormat** ppTextFormat)
{
    if (!pBuffer)
    {
        *ppTextFormat = 0;
        return ERR_POINTER;
    }
    TextFormat* format = new TextFormat;
    format->AddRef();
    HRESULT hr;
    if (FAILED(hr = format->Initialize(pDWriteFactory.Get(), lpFaceName, fontSize, pBuffer)))
    {
        *ppTextFormat = 0;
        format->Release();
        char msg[512];
        sprintf(msg, "未能加载字体文件：%p(size=%d)", pBuffer->GetPointer(), pBuffer->GetSize());
        ReportFatalError(msg, FE_CANNOTOPENFILE);
        return hr;
    }
    *ppTextFormat = format;
    return hr;
}

HRESULT Engine::CreateTextFormatFromBuffer(IBuffer* pBuffer, LPCSTR lpFaceName, float fontSize, DWRITE_FONT_WEIGHT weight,
    DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, LPCSTR localeName, ITextFormat** ppTextFormat)
{
    if (!pBuffer)
    {
        *ppTextFormat = 0;
        return ERR_POINTER;
    }
    TextFormat* format = new TextFormat;
    format->AddRef();
    HRESULT hr;
    if (FAILED(hr = format->Initialize(pDWriteFactory.Get(), lpFaceName, fontSize, weight, style, stretch, localeName, pBuffer)))
    {
        *ppTextFormat = 0;
        format->Release();
        char msg[512];
        sprintf(msg, "未能加载字体文件：%p(size=%d)", pBuffer->GetPointer(), pBuffer->GetSize());
        ReportFatalError(msg, FE_CANNOTOPENFILE);
        return hr;
    }
    *ppTextFormat = format;
    return hr;
}

HRESULT Engine::CreateRenderTargetTreeView(IRenderTarget* pTarget, IRenderTargetTreeView** ppView)
{
    RenderTargetTreeView* tv = new RenderTargetTreeView;
    tv->AddRef();
    tv->mutex = rtTreeMutex;
    tv->pRenderTarget = pTarget;
    *ppView = tv;
    return SUCCEED_OPERATE;
}

HRESULT Engine::CreateRenderTargetTreeNode(IRenderTarget* pTarget, IRenderable* pRenderable, IRenderTargetTreeNode** ppNode)
{
    if (!pRenderable)
    {
        *ppNode = 0;
        return ERR_POINTER;
    }
    RenderTargetTreeNode* tn = new RenderTargetTreeNode;
    tn->AddRef();
    tn->mutex = rtTreeMutex;
    tn->pRenderTarget = pTarget;
    tn->pRenderable = pRenderable;
    tn->renderer = static_cast<IRenderer*>(this);
    *ppNode = tn;
    return SUCCEED_OPERATE;
}

HRESULT Engine::CreateBuffer(const void* pData, UINT pSize, IBuffer** ppBuffer)
{
    unsigned char* data = new unsigned char[pSize];
    if (!data)
    {
        *ppBuffer = 0;
        return ERR_OUTOFMEMORY;
    }
    memcpy(data, pData, pSize);
    Buffer* buffer = new Buffer(data, pSize);
    buffer->AddRef();
    *ppBuffer = buffer;
    return SUCCEED_OPERATE;
}

HRESULT Engine::CreateBuffer(LPCSTR filename, IBuffer** ppBuffer)
{
    if (_access(filename, 0) == -1)
    {
        RCInfo info;
        vector<BYTE> buffer;
        LoadResourceFromFile(filename, buffer, &info, 0);
        if (!info.exist)
        {
            *ppBuffer = 0;
            return ERR_ACCESS;
        }
        return CreateBuffer(buffer.data(), buffer.size(), ppBuffer);
    }
    FileStream fs(filename, FileStream::Mode::Open);
    unsigned char* data = new unsigned char[fs.GetLength()];
    if (!data)
    {
        *ppBuffer = 0;
        return ERR_OUTOFMEMORY;
    }
    fs.Read(data, 0, fs.GetLength());
    Buffer* buffer = new Buffer(data, fs.GetLength());
    fs.Close();
    buffer->AddRef();
    *ppBuffer = buffer;
    return SUCCEED_OPERATE;
}

HRESULT _stdcall Engine::QueryInterface(const IID& iid, void** ppvObject)
{
    if (iid == IID_IVirtualEngine)
    {
        *ppvObject = static_cast<IVirtualEngine*>(this);
    }
    else if (iid == IID_IRenderer)
    {
        *ppvObject = static_cast<IRenderer*>(this);
    }
    else if (iid == IID_IInput)
    {
        *ppvObject = static_cast<IInput*>(this);
    }
    else if (iid == IID_ITimer)
    {
        *ppvObject = static_cast<ITimer*>(this);
    }
    else if (iid == IID_IPhysicsManager)
    {
        *ppvObject = static_cast<IPhysicsManager*>(this);
    }
    else if (iid == IID_IVirtualBase)
    {
        *ppvObject = static_cast<IVirtualBase*>(this);
    }
    else if (iid == IID_IAudioPlayer)
    {
        *ppvObject = static_cast<IAudioPlayer*>(this);
    }
    else
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }
    static_cast<IUnknown*>(*ppvObject)->AddRef();
    return S_OK;
}

ULONG _stdcall Engine::AddRef()
{
    return InterlockedIncrement(&mRef);
}

ULONG _stdcall Engine::Release()
{
    ULONG val = InterlockedDecrement(&mRef);
    if (val == 0)
    {
        delete this;
    }
    return val;
}

void Engine::MessageLoop()
{
    mThreadNum = 2;
    thread(&Engine::LogicLoop, this).detach();
    thread(&Engine::RenderLoop, this).detach();

    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    while (msg.message != WM_QUIT && !quitIsSignalled)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Sleep(1);
        }
    }
    quitIsSignalled = true;
    while (mThreadNum)
        Sleep(1);
}

void Engine::LogicLoop()
{
    while (!quitIsSignalled)
    {
        WaitForSingleObject(renderLock, INFINITE);
        int updateCount = GetUpdateCount();
        if (updateCount < 1)
            updateCount = 1;
        AudioUpdate();
        for (int i = 0; i < updateCount; i++)
        {
            TimeUpdate();
            InputUpdate();

            if (mDesc.videoDesc.altEnter)
            {
                if (KeyDown(DIK_LMENU) && KeyPress(DIK_RETURN))
                {
                    Invoke(Delegate([&]()->void {SetWindowed(!GetWindowed(), SCALING_MODE_UNSPECIFIED); }));
                }
            }

            if (nowScene)
                nowScene->SceneUpdate();
            vector<ComPtr<IScene>> scenes = mScenes;
            for (vector<ComPtr<IScene>>::iterator it = scenes.begin(); it != scenes.end(); it++)
                (*it)->ProcessMessages();
        }
        ReleaseSemaphore(logicLock, 1, 0);
    }
    WaitForSingleObject(renderEnd, INFINITE);
    vector<ComPtr<IScene>> scenes = mScenes;
    for (vector<ComPtr<IScene>>::iterator it = scenes.begin(); it != scenes.end(); it++)
        (*it)->Destroy();
    for (vector<ComPtr<IScene>>::iterator it = scenes.begin(); it != scenes.end(); it++)
        (*it)->ProcessMessages();
    ReleaseSemaphore(logicLock, 5, 0);
    InterlockedDecrement(&mThreadNum);
}

void Engine::RenderLoop()
{
    while (!quitIsSignalled)
    {
        WaitForSingleObject(logicLock, INFINITE);
        while (!rendererDelegates.empty())
        {
            vector<ComPtr<IDelegate>> delTemp = rendererDelegates;
            rendererDelegates.clear();
            for (auto e : delTemp)
                e->Invoke();
        }
        if (!needTest)
        {
            if (switchModeIsSignalled)
            {
                switchModeIsSignalled = false;
                SwitchMode();
            }
            if (nowScene && nowScene->IsValid())
            {
                LPACTOR gp = nowScene->GetFirstActor();
                vector<LPACOM> acoms;
                ComPtr<IRenderable> renderable;
                ComPtr<IAContainer> gContainer;
                for (; gp; gp = gp->GetNext())
                {
                    gp.As(&renderable);
                    if (renderable)
                    {
                        rendercoms.emplace_back(renderable);
                        renderable->OnFetchData();
                    }
                    gp.As(&gContainer);
                    if (!gContainer)
                        continue;
                    acoms.clear();
                    gContainer->GetAllComponents(&acoms);
                    for (LPACOMITERATOR it = acoms.begin(); it != acoms.end(); it++)
                    {
                        if ((*it)->GetActive())
                        {
                            (*it).As(&renderable);
                            if (renderable)
                            {
                                rendercoms.emplace_back(renderable);
                                renderable->OnFetchData();
                            }
                        }
                    }
                }
                if (nowScene->GetMainCamera())
                {
                    SendShaderData(SD_UPDATEMATRIX, nowScene->GetMainCamera(), 0);
                }
            }
            if (mDesc.attributes.autoClear)
            {
                ClearRenderTarget(0, mDesc.attributes.backColor);
            }
            ReleaseSemaphore(renderLock, 1, 0);
            SetRenderTarget(0);
            for (vector<ComPtr<IRenderable>>::iterator it = rendercoms.begin(); it != rendercoms.end(); it++)
                (*it)->OnRender();
            SetRenderTarget(0);
            if (mDesc.videoDesc.msaaEnabled)
            {
                pContext->ResolveSubresource(mCanvasBuffer.Get(), D3D11CalcSubresource(0, 0, 1), mRenderTargetBuffer.Get(), D3D11CalcSubresource(0, 0, 1), DXGI_FORMAT_B8G8R8A8_UNORM);
                for (vector<ComPtr<IRenderable>>::iterator it = rendercoms.begin(); it != rendercoms.end(); it++)
                    (*it)->OnRender2D();
                SaveState();
                SetRenderTarget(0);
                SetBlendState(BLEND_STATE_NORMAL);
                pContext->IASetInputLayout(mCanvasInput.Get());
                UINT stride = 20;
                UINT offset = 0;
                pContext->IASetVertexBuffers(0, 1, mCanvasVertexBuffer.GetAddressOf(), &stride, &offset);
                SetWorldMatrix(XMMatrixScaling(bufferSize.width, bufferSize.height, 1) * XMMatrixTranslation(0, 0, 1));
                SetViewMatrix(XMMatrixIdentity());
                SetProjectionMatrix(XMMatrixOrthographicLH(bufferSize.width, bufferSize.height, 0.1, 1000));
                mCanvasMatWVP->SetMatrix((float*)&GetWVPMatrix());
                mCanvasMatTexture->SetMatrix((float*)&XMMatrixIdentity());
                mCanvasTexture->SetResource(mCanvasSRView.Get());
                mCanvasPass->Apply(0, pContext.Get());
                D3D11_VIEWPORT vp;
                vp.Width = bufferSize.width;
                vp.Height = bufferSize.height;
                vp.MinDepth = 0;
                vp.MaxDepth = 1;
                vp.TopLeftX = 0;
                vp.TopLeftY = 0;
                pContext->RSSetViewports(1, &vp);
                pContext->Draw(6, 0);
                Color color = { 0,0,0,0 };
                pContext->ClearRenderTargetView(mCanvasRTView.Get(), (float*)&color);
                LoadState();
            }
            else
            {
                pContext->OMSetRenderTargets(0, 0, 0);
                pContext->CopyResource(mCanvasBuffer.Get(), mRenderTargetBuffer.Get());
                for (vector<ComPtr<IRenderable>>::iterator it = rendercoms.begin(); it != rendercoms.end(); it++)
                    (*it)->OnRender2D();
                pContext->CopyResource(mRenderTargetBuffer.Get(), mCanvasBuffer.Get());
                Color color = { 0,0,0,0 };
                pContext->ClearRenderTargetView(mCanvasRTView.Get(), (float*)&color);
            }
            rendercoms.clear();
            if (mRenderTargetTreeView)
            {
                WaitForSingleObject(rtTreeMutex, INFINITE);
                SaveState();
                mRenderTargetTreeView->Draw();
                LoadState();
                ReleaseMutex(rtTreeMutex);
            }
            SetRenderTarget(0);

            int renderCount = GetPresentCount();
            if (renderCount < 1)
                renderCount = 1;
            for (int i = 0; i < renderCount; i++)
            {
                if (pSwapChain->Present(mDesc.videoDesc.vSync, 0) != S_OK)
                    needTest = true;
            }
        }
        else
        {
            ReleaseSemaphore(renderLock, 1, 0);
            int renderCount = GetPresentCount();
            if (renderCount < 1)
                renderCount = 1;
            for (int i = 0; i < renderCount; i++)
            {
                if (pSwapChain->Present(mDesc.videoDesc.vSync, DXGI_PRESENT_TEST) == S_OK)
                    needTest = false;
                Sleep(20);
            }
        }
    }
    ReleaseSemaphore(renderLock, 5, 0);
    ReleaseSemaphore(renderEnd, 5, 0);
    InterlockedDecrement(&mThreadNum);
}

void Engine::SwitchMode()
{
    if (pSwapChain)
    {
        isWindowed = setWindowed;
        if (!setWindowed)
        {
            pContext->OMSetRenderTargets(0, 0, 0);
            mRenderTargetBuffer.Reset();
            mRenderTargetRTView.Reset();
            mRenderTargetSRView.Reset();
            mDepthStencilBuffer.Reset();
            mDepthStencilView.Reset();

            RenderTarget* pRT = static_cast<RenderTarget*>(mRenderTarget.Get());
            pRT->mBufferSurface.Reset();
            pRT->mDepthStencilBuffer.Reset();
            pRT->mDepthStencilView.Reset();
            pRT->mRenderTargetBuffer.Reset();
            pRT->mRenderTargetRTView.Reset();
            pRT->mRenderTargetSRView.Reset();

            int scrWidth = GetSystemMetrics(SM_CXSCREEN);
            int scrHeight = GetSystemMetrics(SM_CYSCREEN);
            float newaspect = (float)scrWidth / (float)scrHeight;
            float oldaspect = (float)mDesc.videoDesc.bufferWidth / (float)mDesc.videoDesc.bufferHeight;
            UINT newWidth, newHeight;
            if (scalingMode == SCALING_MODE_UNSPECIFIED)
            {
                scalingMode = lastSCM;
            }
            if (scalingMode != SCALING_MODE_FIXEDASPECT)
            {
                newWidth = scrWidth;
                newHeight = scrHeight;
                lastSCM = SCALING_MODE_SCREENASPECT;
            }
            else
            {
                if (newaspect > oldaspect)
                {
                    newWidth = (UINT)(oldaspect * scrHeight);
                    newHeight = (UINT)scrHeight;
                }
                else
                {
                    newWidth = (UINT)scrWidth;
                    newHeight = (UINT)(scrWidth / oldaspect);
                }
                lastSCM = SCALING_MODE_FIXEDASPECT;
            }
            bufferSize = { newWidth,newHeight };
            DXGI_MODE_DESC desc;
            desc.Width = newWidth;
            desc.Height = newHeight;
            desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            desc.RefreshRate.Numerator = mDesc.videoDesc.refreshRate;
            desc.RefreshRate.Denominator = 1;
            desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
            desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
            pSwapChain->ResizeTarget(&desc);
            pSwapChain->ResizeBuffers(1, newWidth, newHeight, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&mRenderTargetBuffer));
            pDevice->CreateRenderTargetView(mRenderTargetBuffer.Get(), 0, &mRenderTargetRTView);
            pDevice->CreateShaderResourceView(mRenderTargetBuffer.Get(), 0, &mRenderTargetSRView);
            depthStencilDesc.Width = newWidth;
            depthStencilDesc.Height = newHeight;
            pDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
            pDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), 0, &mDepthStencilView);
            pContext->OMSetRenderTargets(1, mRenderTargetRTView.GetAddressOf(), mDepthStencilView.Get());

            D3D11_TEXTURE2D_DESC canvasDesc;
            ZeroMemory(&canvasDesc, sizeof(canvasDesc));
            canvasDesc.ArraySize = 1;
            canvasDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
            canvasDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            canvasDesc.Width = newWidth;
            canvasDesc.Height = newHeight;
            canvasDesc.MipLevels = 1;
            canvasDesc.MiscFlags = D3D11_RESOURCE_MISC_GDI_COMPATIBLE;
            canvasDesc.SampleDesc.Count = 1;
            canvasDesc.SampleDesc.Quality = 0;
            canvasDesc.Usage = D3D11_USAGE_DEFAULT;
            pDevice->CreateTexture2D(&canvasDesc, 0, &mCanvasBuffer);
            pDevice->CreateRenderTargetView(mCanvasBuffer.Get(), 0, &mCanvasRTView);
            pDevice->CreateShaderResourceView(mCanvasBuffer.Get(), 0, &mCanvasSRView);
            mCanvasBuffer.As(&mCanvasSurface);
            D2D1_RENDER_TARGET_PROPERTIES props = RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
                PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
            if (pD2DRenderTarget == mD2DRenderTarget)
            {
                pD2DFactory->CreateDxgiSurfaceRenderTarget(mCanvasSurface.Get(), &props, &mD2DRenderTarget);
                pD2DRenderTarget = mD2DRenderTarget;
            }
            else
            {
                pD2DFactory->CreateDxgiSurfaceRenderTarget(mCanvasSurface.Get(), &props, &mD2DRenderTarget);
            }
            mRenderTargetBuffer.As(&pRT->mBufferSurface);
            pRT->mD2DRenderTarget = mD2DRenderTarget;
            pRT->mDepthStencilBuffer = mDepthStencilBuffer;
            pRT->mDepthStencilView = mDepthStencilView;
            pRT->mRenderTargetBuffer = mRenderTargetBuffer;
            pRT->mRenderTargetRTView = mRenderTargetRTView;
            pRT->mRenderTargetSRView = mRenderTargetSRView;
            pRT->size = { newWidth,newHeight };

            USize size = pRT->GetSize();
            D3D11_VIEWPORT vp;
            vp.Width = size.width;
            vp.Height = size.height;
            vp.MinDepth = 0;
            vp.MaxDepth = 1;
            vp.TopLeftX = 0;
            vp.TopLeftY = 0;
            pContext->RSSetViewports(1, &vp);

            pCustomTextRenderer->m_pRenderTarget = pD2DRenderTarget;

            if (mDesc.videoDesc.fixedAspect)
                mD2DRenderTarget->SetTransform(Matrix3x2F::Scale({ newWidth / (float)mDesc.videoDesc.bufferWidth,newHeight / (float)mDesc.videoDesc.bufferHeight }));

            pSwapChain->SetFullscreenState(!setWindowed, 0);
            for (auto e : eventCallBacks)
                e->OnBufferResized(newWidth, newHeight);
        }
        else
        {
            pSwapChain->SetFullscreenState(!setWindowed, 0);
            pContext->OMSetRenderTargets(0, 0, 0);
            mRenderTargetBuffer.Reset();
            mRenderTargetRTView.Reset();
            mRenderTargetSRView.Reset();
            mDepthStencilBuffer.Reset();
            mDepthStencilView.Reset();

            RenderTarget* pRT = static_cast<RenderTarget*>(mRenderTarget.Get());
            pRT->mBufferSurface.Reset();
            pRT->mDepthStencilBuffer.Reset();
            pRT->mDepthStencilView.Reset();
            pRT->mRenderTargetBuffer.Reset();
            pRT->mRenderTargetRTView.Reset();
            pRT->mRenderTargetSRView.Reset();

            UINT newWidth = (float)mDesc.videoDesc.bufferWidth;
            UINT newHeight = (float)mDesc.videoDesc.bufferHeight;
            bufferSize = { newWidth,newHeight };
            DXGI_MODE_DESC desc;
            desc.Width = newWidth;
            desc.Height = newHeight;
            desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            desc.RefreshRate.Numerator = mDesc.videoDesc.refreshRate;
            desc.RefreshRate.Denominator = 1;
            desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
            desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
            pSwapChain->ResizeTarget(&desc);
            pSwapChain->ResizeBuffers(1, newWidth, newHeight, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&mRenderTargetBuffer));
            pDevice->CreateRenderTargetView(mRenderTargetBuffer.Get(), 0, &mRenderTargetRTView);
            pDevice->CreateShaderResourceView(mRenderTargetBuffer.Get(), 0, &mRenderTargetSRView);
            depthStencilDesc.Width = newWidth;
            depthStencilDesc.Height = newHeight;
            pDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
            pDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), 0, &mDepthStencilView);
            pContext->OMSetRenderTargets(1, mRenderTargetRTView.GetAddressOf(), mDepthStencilView.Get());

            D3D11_TEXTURE2D_DESC canvasDesc;
            ZeroMemory(&canvasDesc, sizeof(canvasDesc));
            canvasDesc.ArraySize = 1;
            canvasDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
            canvasDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            canvasDesc.Width = newWidth;
            canvasDesc.Height = newHeight;
            canvasDesc.MipLevels = 1;
            canvasDesc.MiscFlags = D3D11_RESOURCE_MISC_GDI_COMPATIBLE;
            canvasDesc.SampleDesc.Count = 1;
            canvasDesc.SampleDesc.Quality = 0;
            canvasDesc.Usage = D3D11_USAGE_DEFAULT;
            pDevice->CreateTexture2D(&canvasDesc, 0, &mCanvasBuffer);
            pDevice->CreateRenderTargetView(mCanvasBuffer.Get(), 0, &mCanvasRTView);
            pDevice->CreateShaderResourceView(mCanvasBuffer.Get(), 0, &mCanvasSRView);
            mCanvasBuffer.As(&mCanvasSurface);
            D2D1_RENDER_TARGET_PROPERTIES props = RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
                PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
            if (pD2DRenderTarget == mD2DRenderTarget)
            {
                pD2DFactory->CreateDxgiSurfaceRenderTarget(mCanvasSurface.Get(), &props, &mD2DRenderTarget);
                pD2DRenderTarget = mD2DRenderTarget;
            }
            else
            {
                pD2DFactory->CreateDxgiSurfaceRenderTarget(mCanvasSurface.Get(), &props, &mD2DRenderTarget);
            }
            mRenderTargetBuffer.As(&pRT->mBufferSurface);
            pRT->mD2DRenderTarget = mD2DRenderTarget;
            pRT->mDepthStencilBuffer = mDepthStencilBuffer;
            pRT->mDepthStencilView = mDepthStencilView;
            pRT->mRenderTargetBuffer = mRenderTargetBuffer;
            pRT->mRenderTargetRTView = mRenderTargetRTView;
            pRT->mRenderTargetSRView = mRenderTargetSRView;
            pRT->size = { newWidth,newHeight };

            USize size = pRT->GetSize();
            D3D11_VIEWPORT vp;
            vp.Width = size.width;
            vp.Height = size.height;
            vp.MinDepth = 0;
            vp.MaxDepth = 1;
            vp.TopLeftX = 0;
            vp.TopLeftY = 0;
            pContext->RSSetViewports(1, &vp);

            pCustomTextRenderer->m_pRenderTarget = pD2DRenderTarget;

            if (mDesc.videoDesc.fixedAspect)
                mD2DRenderTarget->SetTransform(Matrix3x2F::Scale({ newWidth / (float)mDesc.videoDesc.bufferWidth,newHeight / (float)mDesc.videoDesc.bufferHeight }));

            for(auto e : eventCallBacks)
                e->OnBufferResized(newWidth, newHeight);
        }
    }
}

void Engine::SaveState()
{
    pContext->OMGetBlendState(&s_blendState, s_blendFactor, &s_blendMask);
    pContext->IAGetInputLayout(&s_inputLayout);
    pContext->OMGetDepthStencilState(&s_dsState, &s_stencilRef);
    pContext->RSGetState(&s_rsState);
}

void Engine::LoadState()
{
    pContext->OMSetBlendState(s_blendState.Get(), s_blendFactor, s_blendMask);
    pContext->IASetInputLayout(s_inputLayout.Get());
    pContext->OMSetDepthStencilState(s_dsState.Get(), s_stencilRef);
    pContext->RSSetState(s_rsState.Get());
}

HRESULT CreateVirtualEngine(DWORD sdkVersion, EngineDescription& des, IVirtualEngine** ppEngine)
{
    if (sdkVersion != VENGINE_SDK_VERSION)
    {
        *ppEngine = 0;
        return ERR_INVALIDVERSION;
    }
    if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
    {
        *ppEngine = 0;
        return ERR_COINITIALIZE;
    }
    HRESULT hr;
    bool autoCreateWindow = false;
    if (!des.outputWindow)
    {
        autoCreateWindow = true;
        hr = CreateDefaultWindow(des);
        if (FAILED(hr))
        {
            ppEngine = 0;
            return hr;
        }
        ShowWindow(des.outputWindow, SW_SHOW);
    }
    if (!des.errorCallBack)
        des.errorCallBack = &defaultErrorCallBack;
    Engine* engine = new Engine(des);
    engine->AddRef();
    engine->autoCreateWindow = autoCreateWindow;
    hr = engine->InitDirect3D(des.videoDesc, des.outputWindow);
    if (FAILED(hr))
    {
        if (autoCreateWindow)
            DestroyWindow(des.outputWindow);
        engine->Destroy();
        engine->Release();
        ppEngine = 0;
        return hr;
    }
    hr = engine->InitDirectInput(des);
    if (FAILED(hr))
    {
        if (autoCreateWindow)
            DestroyWindow(des.outputWindow);
        engine->Destroy();
        engine->Release();
        ppEngine = 0;
        return hr;
    }
    hr = engine->InitAudioPlayer();
    if (FAILED(hr))
    {
        if (autoCreateWindow)
            DestroyWindow(des.outputWindow);
        engine->Destroy();
        engine->Release();
        ppEngine = 0;
        return hr;
    }
    hr = engine->InitPhysX(des.physxDesc);
    if (FAILED(hr))
    {
        if (autoCreateWindow)
            DestroyWindow(des.outputWindow);
        engine->Destroy();
        engine->Release();
        ppEngine = 0;
        return hr;
    }
    *ppEngine = engine;
    engineArray.emplace_back(engine);
    engine->mDesc = des;
    return SUCCEED_OPERATE;
}