#pragma once
#include "../Common/EngineCommon.h"
#include "../Interface/IRenderTarget.h"
#include "IRenderTargetTreeNode.h"
#include "IVirtualBase.h"
#include "IProvideRenderTarget.h"

MIDL_INTERFACE("f48a6e90-5742-4c07-bd16-c4bfe24fe89e")
IRenderTargetTreeView:public IVirtualBase, public IProvideRenderTarget
{
	virtual bool EnumTreeNodes(IRenderTargetTreeNode** ppNode, UINT index) = 0;

	virtual void AddTreeNode(IRenderTargetTreeNode* pNode) = 0;

	virtual void SortTreeNodes(bool (*sortfunc)(const ComPtr<IRenderTargetTreeNode>& node1, const ComPtr<IRenderTargetTreeNode>& node2)) = 0;

	virtual void Draw() = 0;

	virtual HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) = 0;

	virtual ULONG _stdcall AddRef() = 0;

	virtual ULONG _stdcall Release() = 0;
};