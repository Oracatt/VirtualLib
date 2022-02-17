#pragma once
#include "../Common/EngineCommon.h"
#include "IRenderTarget.h"
#include "IRenderTargetTreeView.h"
#include "IDelegate.h"

MIDL_INTERFACE("fe2a9aa0-fccd-8628-0057-79c5515c455c")
IRenderer:public IUnknown
{
    virtual HRESULT GetPointer(const IID & iid,void** ppvObject) = 0;

    virtual XMMATRIX GetTextureMatrix() = 0;

    virtual XMMATRIX GetWorldMatrix() = 0;

    virtual XMMATRIX GetViewMatrix() = 0;

    virtual XMMATRIX GetProjectionMatrix() = 0;

    virtual XMMATRIX GetWVPMatrix() = 0;

    virtual void SetTextureMatrix(const XMMATRIX& mat) = 0;

    virtual void SetWorldMatrix(const XMMATRIX& mat) = 0;

    virtual void SetViewMatrix(const XMMATRIX& mat) = 0;

    virtual void SetProjectionMatrix(const XMMATRIX& mat) = 0;

    virtual void StartRender() = 0;

    virtual bool GetWindowed() = 0;

    virtual void SetWindowed(bool windowed, SCALING_MODE scalingMode) = 0;

    virtual void SendShaderData(UINT type, LPVOID data1, LPVOID data2) = 0;

    virtual USize GetBufferSize() = 0;

    virtual void SetBlendState(BLEND_STATE blendState) = 0;

    virtual IRenderTarget* GetRenderTarget() = 0;

    virtual void SetRenderTarget(IRenderTarget* pRenderTarget) = 0;

    virtual void ClearRenderTarget(IRenderTarget* pRenderTarget, const Color& color) = 0;

    virtual void SetRenderTargetTreeView(IRenderTargetTreeView* pView) = 0;

    virtual IRenderTargetTreeView* GetRenderTargetTreeView() = 0;

    virtual void Invoke(IDelegate* pDelegate) = 0;

};