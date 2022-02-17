#pragma once
#include "../../Interface/IRenderer.h"
#include "CustomTextRenderer.h"

class Renderer :public IRenderer
{
public:

	Renderer(class Engine* engine);

	HRESULT InitDirect3D(VideoDesc& desc, HWND hwnd);

	void ReleaseDirect3D();

	HRESULT GetPointer(const IID& iid, void** ppvObject) override;

	XMMATRIX GetTextureMatrix() override;

	XMMATRIX GetWorldMatrix() override;

	XMMATRIX GetViewMatrix() override;

	XMMATRIX GetProjectionMatrix() override;

	XMMATRIX GetWVPMatrix() override;

	void SetTextureMatrix(const XMMATRIX& mat) override;

	void SetWorldMatrix(const XMMATRIX& mat) override;

	void SetViewMatrix(const XMMATRIX& mat) override;

	void SetProjectionMatrix(const XMMATRIX& mat) override;

	void StartRender() override;

	bool GetWindowed() override;

	void SetWindowed(bool windowed, SCALING_MODE scalingMode) override;

	void SendShaderData(UINT type, LPVOID data1, LPVOID data2) override;

	USize GetBufferSize() override;

	void SetBlendState(BLEND_STATE blendState) override;

	IRenderTarget* GetRenderTarget() override;

	void SetRenderTarget(IRenderTarget* pRenderTarget) override;

	void ClearRenderTarget(IRenderTarget* pRenderTarget, const Color& color) override;

	void SetRenderTargetTreeView(IRenderTargetTreeView* pView) override;

	IRenderTargetTreeView* GetRenderTargetTreeView() override;

	void Invoke(IDelegate* pDelegate) override;

	class Engine* engine;

	XMMATRIX matWorld, matView, matProj, matWVP, matTexture;

	ComPtr<ID3D11Device> pDevice = 0;

	ComPtr<ID3D11DeviceContext> pContext;

	ComPtr<IDXGISwapChain> pSwapChain;

	ComPtr<IDXGIDevice> pDxgiDevice;

	ComPtr<IDXGIAdapter> pDxgiAdapter;

	ComPtr<IDXGIFactory> pDxgiFactory;

	ComPtr<ID2D1Factory> pD2DFactory;

	ComPtr<IDWriteFactory> pDWriteFactory;

	ComPtr<ID2D1RenderTarget> pD2DRenderTarget;

	ComPtr<CustomTextRenderer> pCustomTextRenderer;

	ComPtr<ID2D1Brush> pBodyBrush;

	ComPtr<ID2D1Brush> pOutlineBrush;

	ComPtr<ITextFormat> pFormat;

	ComPtr<IRenderTarget> pRenderTarget;

	ComPtr<ID3D11Texture2D> mRenderTargetBuffer;

	ComPtr<ID3D11RenderTargetView> mRenderTargetRTView;

	ComPtr<ID3D11ShaderResourceView> mRenderTargetSRView;

	ComPtr<ID3D11Texture2D> mDepthStencilBuffer;

	ComPtr<ID3D11DepthStencilView> mDepthStencilView;

	ComPtr<ID3D11Buffer> rectVertexBuffer;

	ComPtr<ID3D11Buffer> mCanvasVertexBuffer;

	ComPtr<ID3D11Texture2D> mCanvasBuffer;

	ComPtr<ID3D11RenderTargetView> mCanvasRTView;

	ComPtr<ID3D11ShaderResourceView> mCanvasSRView;

	ComPtr<IDXGISurface> mCanvasSurface;

	ComPtr<ID3DX11Effect> mCanvasEffect;

	ComPtr<ID3DX11EffectTechnique> mCanvasTech;

	ComPtr<ID3DX11EffectPass> mCanvasPass;

	ComPtr<ID3DX11EffectShaderResourceVariable> mCanvasTexture;

	ComPtr<ID3DX11EffectMatrixVariable> mCanvasMatWVP;

	ComPtr<ID3DX11EffectMatrixVariable> mCanvasMatTexture;

	ComPtr<ID3D11InputLayout> mCanvasInput;

	ComPtr<ID3D11BlendState> mBlendStateNormal;

	ComPtr<ID3D11BlendState> mBlendStateBrighten;

	ComPtr<ID3D11BlendState> mBlendStateDarken;

	ComPtr<ID3D11BlendState> mBlendStateInvDest;

	ComPtr<ID2D1RenderTarget> mD2DRenderTarget;

	ComPtr<IRenderTarget> mRenderTarget;

	ComPtr<IRenderTargetTreeView> mRenderTargetTreeView;

	vector<ComPtr<IDelegate>> rendererDelegates;

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	Vector3 cameraPosition;

	bool switchModeIsSignalled = false;

	bool setWindowed;

	SCALING_MODE scalingMode;

	bool isWindowed;

	SCALING_MODE lastSCM;

	USize bufferSize;

	HANDLE rendererDelegateMutex;

};