#include "../../EasyDevelop/SceneCom.h"

SComponent::SComponent() :mRef(0), scene(0), thispointer(this), isValid(true), priority(100), userdata(0) {}

ISComponent::~ISComponent() {}
SComponent::~SComponent() {}

IScene* SComponent::GetScene()
{
	return scene.Get();
}

void SComponent::SetScene(IScene* pScene)
{
	if (pScene->HasComponent(this))
		scene = pScene;
	else
		pScene->AddComponent(this);
}

int SComponent::GetPriority()
{
	return priority;
}

void SComponent::SetPriority(int p, bool autosort)
{
	priority = p;
	if (autosort)
		scene->SortComponents();
}

bool SComponent::IsValid()
{
	return isValid;
}

void SComponent::Destroy()
{
	if (isValid)
	{
		isValid = false;
		OnRemoved();
		if (scene)
		{
			vector<LPSCOM>* pScoms;
			scene->GetAllComponentsReference(&pScoms);
			for (LPSCOMITERATOR it = pScoms->begin(); it != pScoms->end();)
			{
				if (*it == thispointer)
					it = pScoms->erase(it);
				else
					it++;
			}
			scene->SortComponents();
			scene.Reset();
		}
		thispointer.Reset();
	}
}

void* SComponent::GetUserData()
{
	return userdata;
}

void SComponent::SetUserData(void* pdata)
{
	userdata = pdata;
}

ISRESULT SComponent::OnPreAwake(IScene* pScene) { return ISR_IGNORE; }

void SComponent::OnAwake() {}

void SComponent::OnRemoved() {}

void SComponent::OnPreUpdate() {}

void SComponent::OnPostUpdate() {}

void SComponent::OnPreProcessMessages() {}

void SComponent::OnPostProcessMessages() {}

MSGRESULT SComponent::OnMessageBegin(BYTE& msgType, LPVOID& param1, LPVOID& param2) { return MSGR_IGNORE; }

void SComponent::OnMessageEnd(BYTE msgType, LPVOID param1, LPVOID param2) {}

HRESULT _stdcall SComponent::QueryInterface(const IID& iid, void** ppvObject)
{
	if (iid == IID_ISComponent)
	{
		*ppvObject = static_cast<ISComponent*>(this);
	}
	else if (iid == IID_IVirtualBase)
	{
		*ppvObject = static_cast<IVirtualBase*>(this);
	}
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	static_cast<IUnknown*>(*ppvObject)->AddRef();
	return S_OK;
}

ULONG _stdcall SComponent::AddRef()
{
	return InterlockedIncrement(&mRef);
}

ULONG _stdcall SComponent::Release()
{
	ULONG val = InterlockedDecrement(&mRef);
	if (val == 0)
	{
		delete this;
	}
	return val;
}