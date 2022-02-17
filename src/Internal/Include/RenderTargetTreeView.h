#pragma once
#include "../../Interface/IRenderTargetTreeView.h"

class RenderTargetTreeView :public IRenderTargetTreeView
{
public:

	ComPtr<IRenderTarget> pRenderTarget;

	HANDLE mutex;

	vector<ComPtr<IRenderTargetTreeNode>> nodes;

	RenderTargetTreeView();

	IRenderTarget* GetRenderTarget() override;

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

	ComPtr<IRenderTargetTreeView> thispointer;

	bool isValid;

	void* userdata;
};