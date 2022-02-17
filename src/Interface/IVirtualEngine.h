#pragma once
#include "../Common/EngineCommon.h"
#include "IScene.h"
#include "IVirtualBase.h"
#include "IRenderTarget.h"
#include "ITexture.h"
#include "ITextFormat.h"
#include "IRenderTargetTreeNode.h"
#include "IRenderTargetTreeView.h"
#include "IBuffer.h"

MIDL_INTERFACE("2e771471-330b-ed21-7de2-7ef8c88f4a1a")
IVirtualEngine :public IVirtualBase
{
	virtual void Start(IScene * scene) = 0;

	virtual void ReportFatalError(LPCSTR msg, int errcode) = 0;

	virtual void ReportError(LPCSTR msg) = 0;

	virtual EngineAttributes GetAttributes() = 0;

	virtual void SetAttributes(EngineAttributes& attr) = 0;

	virtual vector<EngineEventCallBack*> GetEventCallBacks() = 0;

	virtual void AddEventCallBack(EngineEventCallBack* callback) = 0;

	virtual void RemoveEventCallBack(EngineEventCallBack* callback) = 0;

	virtual HRESULT CreateScene(const SceneDesc* desc, IScene** ppScene) = 0;

	virtual IScene* GetNowScene() = 0;

	virtual void SetNowScene(IScene* pScene) = 0;

	virtual void BroadcastMessage(BYTE msgType, LPVOID param1, LPVOID param2) = 0;

	virtual HRESULT CreateRenderTarget(const RenderTargetDesc& desc, IRenderTarget** ppRenderTarget) = 0;

	virtual HRESULT CreateTextureFromFile(LPCSTR filename, ITexture** ppTexture) = 0;

	virtual HRESULT CreateTextureFromBuffer(IBuffer* pBuffer, ITexture** ppTexture) = 0;

	virtual HRESULT CreateTextFormat(LPCSTR lpFaceName, float fontSize, ITextFormat** ppTextFormat) = 0;

	virtual HRESULT CreateTextFormat(LPCSTR lpFaceName, float fontSize, DWRITE_FONT_WEIGHT weight,
		DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, LPCSTR localeName, ITextFormat** ppTextFormat) = 0;

	virtual HRESULT CreateTextFormatFromFile(LPCSTR filename, LPCSTR lpFaceName, float fontSize, ITextFormat** ppTextFormat) = 0;

	virtual HRESULT CreateTextFormatFromFile(LPCSTR filename, LPCSTR lpFaceName, float fontSize, DWRITE_FONT_WEIGHT weight,
		DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, LPCSTR localeName, ITextFormat** ppTextFormat) = 0;

	virtual HRESULT CreateTextFormatFromBuffer(IBuffer* pBuffer, LPCSTR lpFaceName, float fontSize, ITextFormat** ppTextFormat) = 0;

	virtual HRESULT CreateTextFormatFromBuffer(IBuffer* pBuffer, LPCSTR lpFaceName, float fontSize, DWRITE_FONT_WEIGHT weight,
		DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, LPCSTR localeName, ITextFormat** ppTextFormat) = 0;

	virtual HRESULT CreateRenderTargetTreeView(IRenderTarget* pTarget, IRenderTargetTreeView** ppView) = 0;

	virtual HRESULT CreateRenderTargetTreeNode(IRenderTarget* pTarget, IRenderable* pRenderable, IRenderTargetTreeNode** ppNode) = 0;

	virtual HRESULT CreateBuffer(const void* pData, UINT pSize, IBuffer** ppBuffer) = 0;

	virtual HRESULT CreateBuffer(LPCSTR filename, IBuffer** ppBuffer) = 0;
};