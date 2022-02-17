#include "../../EasyDevelop/GameObject.h"
#include "../../EasyDevelop/ActorCom.h"
#include "../../Extend/AComponents.h"
#include "../../Interface/IScene.h"
#include <algorithm>

bool sortfunc(const LPACOM& sc1, const LPACOM& sc2)
{
	return sc1->GetPriority() > sc2->GetPriority();
}

GameObject::GameObject() :mRef(0), thispointer(this), transform(AddComponent(new Transform)), transform2D(AddComponent(new Transform2D))
, scene(0), isValid(true), last(0), next(0), userdata(0), name("unnamed"), updateLevel(0) {}

IActor::~IActor() {}
IAContainer::~IAContainer() {};
GameObject::~GameObject() {}

string GameObject::GetName()
{
	return name;
}

void GameObject::SetName(const string& _name)
{
	name = _name;
}

IScene* GameObject::GetScene()
{
	return scene.Get();
}

void GameObject::SetScene(IScene* pScene)
{
	if (isValid)
		pScene->AttachActor(this);
	else
		scene.Reset();
}

bool GameObject::IsValid()
{
	return isValid;
}

void GameObject::Destroy()
{
	if (isValid)
	{
		isValid = false;
		if (scene)
		{
			OnDestroy();
			RemoveAllComponents();
			AddRef();
			thispointer.Reset();
			scene->WriteMessageStream(SMSG_DESTROYACTOR, static_cast<IActor*>(this), 0);
		}
		else
		{
			RemoveAllComponents();
			thispointer.Reset();
		}
	}
}

void* GameObject::GetUserData()
{
	return userdata;
}

void GameObject::SetUserData(void* pdata)
{
	userdata = pdata;
}

HRESULT GameObject::InsertBefore(IActor* actor)
{
	if (!scene)
		return ERR_POINTER;
	if (scene.Get() != actor->GetScene())
		return ERR_UNEXPECTED;
	if (!isValid || !actor->IsValid())
		return ERR_UNEXPECTED;
	return scene->WriteMessageStream(SMSG_INSERTBEFORE, static_cast<IActor*>(this), actor);
}

HRESULT GameObject::InsertAfter(IActor* actor)
{
	if (!scene)
		return ERR_POINTER;
	if (scene.Get() != actor->GetScene())
		return ERR_UNEXPECTED;
	if (!isValid || !actor->IsValid())
		return ERR_UNEXPECTED;
	return scene->WriteMessageStream(SMSG_INSERTAFTER, static_cast<IActor*>(this), actor);
}

HRESULT GameObject::SendToFirst()
{
	if (!scene)
		return ERR_POINTER;
	if (!isValid)
		return ERR_UNEXPECTED;
	return scene->WriteMessageStream(SMSG_SENDTOFIRST, static_cast<IActor*>(this), 0);
}

HRESULT GameObject::SendToLast()
{
	if (!scene)
		return ERR_POINTER;
	if (!isValid)
		return ERR_UNEXPECTED;
	return scene->WriteMessageStream(SMSG_SENDTOLAST, static_cast<IActor*>(this), 0);
}

UINT GameObject::GetUpdateLevel()
{
	return updateLevel;
}

void GameObject::SetUpdateLevel(UINT value)
{
	updateLevel = value;
}

void GameObject::OnUpdate() {}
void GameObject::OnDestroy() {}
void GameObject::OnCreated() {};

void GameObject::OnAttachedToScene(IScene* pScene) 
{
	scene = pScene;
	for (LPACOMITERATOR it = components.begin(); it != components.end(); it++)
		(*it)->OnAwake();
	OnCreated();
}

void GameObject::OnComponentAdd(IAComponent* pAcom)
{
	if (GetScene())
		pAcom->OnAwake();
}

void GameObject::RemoveAllComponents()
{
	AComponent* com;
	for (LPACOMITERATOR it = components.begin(); it != components.end(); it++)
	{
		if (*it)
		{
			com = static_cast<AComponent*>((*it).Get());
			com->isValid = false;
			com->enabled = false;
			com->OnRemoved();
			com->gameObject.Reset();
			com->thispointer.Reset();
		}
	}
	components.clear();
	sortedcoms.clear();
}

bool GameObject::HasComponent(IAComponent* com)
{
	if (com)
	{
		for (LPACOMITERATOR it = components.begin(); it != components.end(); it++)
		{
			if ((*it).Get() == com)
				return true;
		}
	}
	return false;
}

void GameObject::RemoveComponent(IAComponent* com)
{
	if (HasComponent(com))
		com->Destroy();
}

void GameObject::GetAllComponents(vector<LPACOM>* pArray, bool sort)
{
	if (sort)
		*pArray = sortedcoms;
	else
		*pArray = components;
}

void GameObject::GetAllComponentsReference(vector<LPACOM>** ppArray, bool sort)
{
	if (sort)
		*ppArray = &sortedcoms;
	else
		*ppArray = &components;
}

IActor* GameObject::GetLast()
{
	return last.Get();
}

IActor* GameObject::GetNext()
{
	return next.Get();
}

void GameObject::SetLast(IActor* actor)
{
	last = actor;
}

void GameObject::SetNext(IActor* actor)
{
	next = actor;
}

HRESULT _stdcall GameObject::QueryInterface(const IID& iid, void** ppvObject)
{
	if (iid == IID_IActor)
	{
		*ppvObject = static_cast<IActor*>(this);
	}
	else if (iid == IID_IAContainer)
	{
		*ppvObject = static_cast<IAContainer*>(this);
	}
	else if (iid == IID_IVirtualBase)
	{
		*ppvObject = static_cast<IVirtualBase*>(static_cast<IActor*>(this));
	}
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	static_cast<IUnknown*>(*ppvObject)->AddRef();
	return S_OK;
}

ULONG _stdcall GameObject::AddRef()
{
	return InterlockedIncrement(&mRef);
}

ULONG _stdcall GameObject::Release()
{
	ULONG val = InterlockedDecrement(&mRef);
	if (val == 0)
	{
		delete this;
	}
	return val;
}

void GameObject::SortComponents()
{
	sortedcoms.clear();
	sortedcoms = components;
	if (sortedcoms.size() > 1)
		sort(sortedcoms.begin(), sortedcoms.end(), sortfunc);
}