#pragma once
#include "../Common/EngineCommon.h"
#include "../FXMaster/Include/d3dx11Effect.h"
#include "../Interface/IVirtualEngine.h"
#include "../Interface/IRenderer.h"
#include "../Interface/IDelegate.h"
#include <ManagedStream.h>

class DefaultEngineEventCallBack :public EngineEventCallBack
{
protected:

	Light lights[10];

	bool lightsEnabled[10] = { false };

	ComPtr<ID3DX11Effect> effect;

	ComPtr<IVirtualEngine> engine;

	ComPtr<IRenderer> renderer;

	ComPtr<ID3D11Device> pDevice;

	ComPtr<ID3D11DeviceContext> pContext;

	ComPtr<ID3D11InputLayout> mInputLayout;

	ComPtr<ID3DX11EffectTechnique> mTech;

	ComPtr<ID3DX11EffectPass> mPass;

	ComPtr<ID3DX11EffectMatrixVariable> mMatWVP;

	ComPtr<ID3DX11EffectMatrixVariable> mMatWorld;

	ComPtr<ID3DX11EffectMatrixVariable> mMatWIT;

	ComPtr<ID3DX11EffectMatrixVariable> mMatTexture;

	ComPtr<ID3DX11EffectVariable> mLights;

	ComPtr<ID3DX11EffectVariable> mMaterial;

	ComPtr<ID3DX11EffectScalarVariable> mLightCount;

	ComPtr<ID3DX11EffectShaderResourceVariable> mTexture;

	ComPtr<ID3DX11EffectScalarVariable> mFogEnabled;

	ComPtr<ID3DX11EffectScalarVariable> mFogStart;

	ComPtr<ID3DX11EffectScalarVariable> mFogRange;

	ComPtr<ID3DX11EffectVectorVariable> mFogColor;

	ComPtr<ID3DX11EffectVariable> mEyePos;

	ComPtr<ID3DX11EffectScalarVariable> mInvEnabled;

	void OnEngineInitialized(IVirtualEngine* engine) override;

	void OnEngineDestroy() override;

	void OnStartRender() override;

	void OnBufferResized(UINT width, UINT height) override;

	void RecvShaderData(UINT type, LPVOID data1, LPVOID data2) override;

	HRESULT LoadShader();

	void UpdateLights();

};

class DefaultEngineErrorCallBack :public EngineErrorCallBack
{
	void ReportFatalError(LPCSTR msg, int errcode) override;

	void ReportError(LPCSTR msg) override;
};

class DefaultRenderTargetRenderer :public IRenderable
{
public:

	bool autoClear = true;

	Color backColor = {0,0,0,0};

	DefaultRenderTargetRenderer(IRenderer* renderer, IRenderTarget* srcTarget);

	void OnFetchData() override;

	void OnRender() override;

	void OnRender2D() override;

	HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) override;

	ULONG _stdcall AddRef() override;

	ULONG _stdcall Release() override;

	void SetSourceRect(const RectangleF& rect);

	RectangleF GetBounds();

	void SetBounds(const RectangleF& rect);

protected:

	ComPtr<IRenderer> renderer;

	ComPtr<ID3D11DeviceContext> pContext;

	ComPtr<IRenderTarget> srcTarget;

	XMMATRIX texMat;

	RectangleF bounds;

private:

	ULONG mRef;
};

HRESULT CreateDefaultWindow(EngineDescription& des);