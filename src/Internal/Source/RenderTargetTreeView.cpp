#include "../Include/RenderTargetTreeView.h"
#include <algorithm>
#include "../Include/RenderTargetTreeNode.h"

RenderTargetTreeView::RenderTargetTreeView() :mRef(0), thispointer(this), isValid(true), userdata(0) {}

IRenderTarget* RenderTargetTreeView::GetRenderTarget()
{
	return pRenderTarget.Get();
}

bool RenderTargetTreeView::EnumTreeNodes(IRenderTargetTreeNode** ppNode, UINT index)
{
	if (index < nodes.size())
	{
		*ppNode = nodes[index].Get();
		return true;
	}
	*ppNode = 0;
	return false;
}

void RenderTargetTreeView::AddTreeNode(IRenderTargetTreeNode* pNode)
{
	WaitForSingleObject(mutex, INFINITE);
	nodes.emplace_back(pNode);
	static_cast<RenderTargetTreeNode*>(pNode)->father = static_cast<IProvideRenderTarget*>(this);
	ReleaseMutex(mutex);
}

void RenderTargetTreeView::SortTreeNodes(bool (*sortfunc)(const ComPtr<IRenderTargetTreeNode>& node1, const ComPtr<IRenderTargetTreeNode>& node2))
{
	WaitForSingleObject(mutex, INFINITE);
	sort(nodes.begin(), nodes.end(), sortfunc);
	ReleaseMutex(mutex);
}

void RenderTargetTreeView::Draw()
{
	for (vector<ComPtr<IRenderTargetTreeNode>>::iterator it = nodes.begin(); it != nodes.end(); it++)
		(*it)->Draw();
}

bool RenderTargetTreeView::IsValid()
{
	return isValid;
}

void RenderTargetTreeView::Destroy()
{
	WaitForSingleObject(mutex, INFINITE);
	if (isValid)
	{
		isValid = false;
		for (vector<ComPtr<IRenderTargetTreeNode>>::iterator it = nodes.begin(); it != nodes.end(); it++)
		{
			static_cast<RenderTargetTreeNode*>((*it).Get())->father.Reset();
		}
		nodes.clear();
		pRenderTarget.Reset();
		thispointer.Reset();
	}
	ReleaseMutex(mutex);
}

void* RenderTargetTreeView::GetUserData()
{
	return userdata;
}

void RenderTargetTreeView::SetUserData(void* pdata)
{
	userdata = pdata;
}

HRESULT _stdcall RenderTargetTreeView::QueryInterface(const IID& iid, void** ppvObject)
{
	if (iid == IID_IRenderTargetTreeView)
	{
		*ppvObject = static_cast<IRenderTargetTreeView*>(this);
	}
	else if (iid == IID_IProvideRenderTarget)
	{
		*ppvObject = static_cast<IProvideRenderTarget*>(this);
	}
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	static_cast<IUnknown*>(*ppvObject)->AddRef();
	return S_OK;
}

ULONG _stdcall RenderTargetTreeView::AddRef()
{
	return InterlockedIncrement(&mRef);
}

ULONG _stdcall RenderTargetTreeView::Release()
{
	ULONG val = InterlockedDecrement(&mRef);
	if (val == 0)
	{
		delete this;
	}
	return val;
}