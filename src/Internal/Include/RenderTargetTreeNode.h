#pragma once
#include "../../Interface/IRenderTargetTreeNode.h"
#include "../../Interface/IRenderTargetTreeView.h"
#include "../../Interface/IRenderable.h"
#include "../../Interface/IRenderer.h"

class RenderTargetTreeNode :public IRenderTargetTreeNode
{
public:

	ComPtr<IRenderTarget> pRenderTarget;

	ComPtr<IRenderable> pRenderable;

	ComPtr<IRenderer> renderer;

	HANDLE mutex;

	vector<ComPtr<IRenderTargetTreeNode>> nodes;

	ComPtr<IProvideRenderTarget> father;

	RenderTargetTreeNode();

	IRenderable* GetRenderer() override;

	IRenderTarget* GetRenderTarget() override;

	IRenderTargetTreeNode* GetParentNode() override;

	IRenderTargetTreeView* GetRoot() override;

	bool EnumTreeNodes(IRenderTargetTreeNode** ppNode, UINT index) override;

	void AddTreeNode(IRenderTargetTreeNode* pNode) override;

	void SortTreeNodes(bool (*sortfunc)(const ComPtr<IRenderTargetTreeNode>& node1, const ComPtr<IRenderTargetTreeNode>& node2)) override;

	void Draw() override;

	bool IsValid() override;

	void Destroy() override;

	void* GetUserData() override;

	void SetUserData(void* pdata) override;

	HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) override;

	ULONG _stdcall AddRef() override;

	ULONG _stdcall Release() override;

private:

	ULONG mRef;

	ComPtr<IRenderTargetTreeNode> thispointer;

	bool isValid;

	void* userdata;
};