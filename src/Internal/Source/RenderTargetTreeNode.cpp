#include "../Include/RenderTargetTreeNode.h"
#include "../../Interface/IRenderer.h"
#include <algorithm>
#include "../Include/RenderTargetTreeView.h"

RenderTargetTreeNode::RenderTargetTreeNode() :mRef(0), father(0), isValid(true), userdata(0), thispointer(this) {}

IRenderable* RenderTargetTreeNode::GetRenderer()
{
	return pRenderable.Get();
}

IRenderTarget* RenderTargetTreeNode::GetRenderTarget()
{
	return pRenderTarget.Get();
}

IRenderTargetTreeNode* RenderTargetTreeNode::GetParentNode()
{
	ComPtr<IRenderTargetTreeNode> node;
	father.As(&node);
	return node.Get();
}

IRenderTargetTreeView* RenderTargetTreeNode::GetRoot()
{
	if (!father)
		return 0;
	ComPtr<IRenderTargetTreeNode> node;
	father.As(&node);
	if (!node)
	{
		ComPtr<IRenderTargetTreeView> view;
		father.As(&view);
		return view.Get();
	}
	node = node->GetParentNode();
	if (node)
		return node->GetRoot();
	return 0;
}

bool RenderTargetTreeNode::EnumTreeNodes(IRenderTargetTreeNode** ppNode, UINT index)
{
	if (index < nodes.size())
	{
		*ppNode = nodes[index].Get();
		return true;
	}
	*ppNode = 0;
	return false;
}

void RenderTargetTreeNode::AddTreeNode(IRenderTargetTreeNode* pNode)
{
	WaitForSingleObject(mutex, INFINITE);
	nodes.emplace_back(pNode);
	static_cast<RenderTargetTreeNode*>(pNode)->father = static_cast<IProvideRenderTarget*>(this);
	ReleaseMutex(mutex);
}

void RenderTargetTreeNode::SortTreeNodes(bool (*sortfunc)(const ComPtr<IRenderTargetTreeNode>& node1, const ComPtr<IRenderTargetTreeNode>& node2))
{
	WaitForSingleObject(mutex, INFINITE);
	sort(nodes.begin(), nodes.end(), sortfunc);
	ReleaseMutex(mutex);
}

void RenderTargetTreeNode::Draw()
{
	for (vector<ComPtr<IRenderTargetTreeNode>>::iterator it = nodes.begin(); it != nodes.end(); it++)
		(*it)->Draw();
	renderer->SetRenderTarget(father->GetRenderTarget());
	pRenderable->OnRender();
}

bool RenderTargetTreeNode::IsValid()
{
	return isValid;
}

void RenderTargetTreeNode::Destroy()
{
	WaitForSingleObject(mutex, INFINITE);
	if (isValid)
	{
		isValid = false;
		if (father)
		{
			ComPtr<IRenderTargetTreeNode> node;
			father.As(&node);
			if (node)
			{
				RenderTargetTreeNode* tn = static_cast<RenderTargetTreeNode*>(father.Get());
				for (vector<ComPtr<IRenderTargetTreeNode>>::iterator it = tn->nodes.begin(); it != tn->nodes.end();)
				{
					if ((*it).Get() == this)
						it = tn->nodes.erase(it);
					else
						it++;
				}
			}
			else
			{
				RenderTargetTreeView* tv = static_cast<RenderTargetTreeView*>(father.Get());
				for (vector<ComPtr<IRenderTargetTreeNode>>::iterator it = tv->nodes.begin(); it != tv->nodes.end();)
				{
					if ((*it).Get() == this)
						it = tv->nodes.erase(it);
					else
						it++;
				}
			}
			father.Reset();
		}
		for (vector<ComPtr<IRenderTargetTreeNode>>::iterator it = nodes.begin(); it != nodes.end(); it++)
		{
			static_cast<RenderTargetTreeNode*>((*it).Get())->father.Reset();
		}
		nodes.clear();
		pRenderTarget.Reset();
		pRenderable.Reset();
		renderer.Reset();
		thispointer.Reset();
	}
	ReleaseMutex(mutex);
}

void* RenderTargetTreeNode::GetUserData()
{
	return userdata;
}

void RenderTargetTreeNode::SetUserData(void* pdata)
{
	userdata = pdata;
}

HRESULT _stdcall RenderTargetTreeNode::QueryInterface(const IID& iid, void** ppvObject)
{
	if (iid == IID_IRenderTargetTreeNode)
	{
		*ppvObject = static_cast<IRenderTargetTreeNode*>(this);
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

ULONG _stdcall RenderTargetTreeNode::AddRef()
{
	return InterlockedIncrement(&mRef);
}

ULONG _stdcall RenderTargetTreeNode::Release()
{
	ULONG val = InterlockedDecrement(&mRef);
	if (val == 0)
	{
		delete this;
	}
	return val;
}