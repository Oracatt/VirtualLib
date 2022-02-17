#pragma once
#include "../Common/EngineCommon.h"
#include "IRenderTarget.h"
#include "IProvideRenderTarget.h"
#include "IVirtualBase.h"
#include "IRenderable.h"

MIDL_INTERFACE("7b5c1601-21cb-4f43-afc7-a280513ce010")
IRenderTargetTreeNode:public IVirtualBase, public IProvideRenderTarget
{
	virtual IRenderable* GetRenderer() = 0;

	virtual IRenderTarget* GetRenderTarget() = 0;

	virtual IRenderTargetTreeNode* GetParentNode() = 0;

	virtual struct IRenderTargetTreeView* GetRoot() = 0;

	virtual bool EnumTreeNodes(IRenderTargetTreeNode** ppNode, UINT index) = 0;

	virtual void AddTreeNode(IRenderTargetTreeNode* pNode) = 0;

	virtual void SortTreeNodes(bool (*sortfunc)(const ComPtr<IRenderTargetTreeNode>& node1, const ComPtr<IRenderTargetTreeNode>& node2)) = 0;

	virtual void Draw() = 0;

	virtual HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) = 0;

	virtual ULONG _stdcall AddRef() = 0;

	virtual ULONG _stdcall Release() = 0;
};