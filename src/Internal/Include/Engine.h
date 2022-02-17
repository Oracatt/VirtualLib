#pragma once
#include "../../Interface/IVirtualEngine.h"
#include "../../Interface/IRenderer.h"
#include "../../Interface/IRenderable.h"
#include "Renderer.h"
#include "Input.h"
#include "AudioPlayer.h"
#include "Timer.h"
#include "PhysicsManager.h"
#include "FontLoader.h"
#include <mutex>
#include <thread>

class Engine :public IVirtualEngine, public Renderer, public Input,
	public AudioPlayer, public Timer, public PhysicsManager
{
	friend class Timer;

	friend class Scene;

	friend class PhysicsScene;

	friend class Renderer;

public:

	EngineDescription mDesc;

	vector<EngineEventCallBack*> eventCallBacks;

	bool autoCreateWindow = false;

	Engine(EngineDescription& desc);

	void ReportFatalError(LPCSTR msg, int errcode) override;

	void ReportError(LPCSTR msg) override;

	void Start(IScene* scene) override;

	EngineAttributes GetAttributes() override;

	void SetAttributes(EngineAttributes& attr) override;

	vector<EngineEventCallBack*> GetEventCallBacks() override;

	void AddEventCallBack(EngineEventCallBack* callback) override;

	void RemoveEventCallBack(EngineEventCallBack* callback) override;

	HRESULT CreateScene(const SceneDesc* desc, IScene** ppScene) override;

	IScene* GetNowScene() override;

	void SetNowScene(IScene* pScene) override;

	bool IsValid() override;

	void Destroy() override;

	void* GetUserData() override;

	void SetUserData(void* pdata) override;

	void BroadcastMessage(BYTE msgType, LPVOID param1, LPVOID param2) override;

	HRESULT CreateRenderTarget(const RenderTargetDesc& desc, IRenderTarget** ppRenderTarget) override;

	HRESULT CreateTextureFromFile(LPCSTR filename, ITexture** ppTexture) override;

	HRESULT CreateTextureFromBuffer(IBuffer* pBuffer, ITexture** ppTexture) override;

	HRESULT CreateTextFormat(LPCSTR lpFaceName, float fontSize, ITextFormat** ppTextFormat) override;

	HRESULT CreateTextFormat(LPCSTR lpFaceName, float fontSize, DWRITE_FONT_WEIGHT weight,
		DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, LPCSTR localeName, ITextFormat** ppTextFormat) override;

	HRESULT CreateTextFormatFromFile(LPCSTR filename, LPCSTR lpFaceName, float fontSize, ITextFormat** ppTextFormat) override;

	HRESULT CreateTextFormatFromFile(LPCSTR filename, LPCSTR lpFaceName, float fontSize, DWRITE_FONT_WEIGHT weight,
		DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, LPCSTR localeName, ITextFormat** ppTextFormat) override;

	HRESULT CreateTextFormatFromBuffer(IBuffer* pBuffer, LPCSTR lpFaceName, float fontSize, ITextFormat** ppTextFormat) override;

	HRESULT CreateTextFormatFromBuffer(IBuffer* pBuffer, LPCSTR lpFaceName, float fontSize, DWRITE_FONT_WEIGHT weight,
		DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, LPCSTR localeName, ITextFormat** ppTextFormat) override;

	HRESULT CreateRenderTargetTreeView(IRenderTarget* pTarget, IRenderTargetTreeView** ppView) override;

	HRESULT CreateRenderTargetTreeNode(IRenderTarget* pTarget, IRenderable* pRenderable, IRenderTargetTreeNode** ppNode) override;

	HRESULT CreateBuffer(const void* pData, UINT pSize, IBuffer** ppBuffer) override;

	HRESULT CreateBuffer(LPCSTR filename, IBuffer** ppBuffer) override;

	HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) override;

	ULONG _stdcall AddRef() override;

	ULONG _stdcall Release() override;

private:

	bool isValid = true;

	ULONG mRef;

	ComPtr<IScene> nowScene;

	ComPtr<IVirtualEngine> thispointer;

	vector<ComPtr<IScene>> mScenes;

	HANDLE logicLock, renderLock, renderEnd, rtTreeMutex;

	HANDLE sceneDelegateMutex;

	ULONG mThreadNum = 0;

	volatile bool quitIsSignalled = false;

	vector<ComPtr<IRenderable>> rendercoms;

	bool needTest = false;

	void* userdata = 0;

	ComPtr<ID3D11BlendState> s_blendState;

	ComPtr<ID3D11DepthStencilState> s_dsState;

	ComPtr<ID3D11RasterizerState> s_rsState;

	UINT s_stencilRef;

	float s_blendFactor[4];

	UINT s_blendMask;

	ComPtr<ID3D11InputLayout> s_inputLayout;

	void MessageLoop();

	void LogicLoop();

	void RenderLoop();

	void SwitchMode();

	void SaveState();

	void LoadState();
};