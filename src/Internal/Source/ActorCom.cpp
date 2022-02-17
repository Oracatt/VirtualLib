#include "../../EasyDevelop/ActorCom.h"
#include "../../EasyDevelop/GameObject.h"

AComponent::AComponent() :mRef(0), gameObject(0), thispointer(this), enabled(true), updateLevel(0), isValid(true), started(false)
, priority(100), userdata(0) {}

IAComponent::~IAComponent() {}
AComponent::~AComponent() {}

IAContainer* AComponent::GetContainer()
{
	return static_cast<IAContainer*>(gameObject.Get());
}

void AComponent::SetContainer(IAContainer* actor)
{
	if (actor->HasComponent(this))
		gameObject = static_cast<GameObject*>(actor);
	else
		actor->AddComponent(this);
}

bool AComponent::IsStarted()
{
	return started;
}

void AComponent::SetStart()
{
	started = true;
}

bool AComponent::GetActive()
{
	return enabled;
}

void AComponent::SetActive(bool active)
{
	enabled = active;
}

int AComponent::GetPriority()
{
	return priority;
}

void AComponent::SetPriority(int p, bool autosort)
{
	priority = p;
	if (autosort)
		gameObject->SortComponents();
}

UINT AComponent::GetUpdateLevel()
{
	return updateLevel;
}

void AComponent::SetUpdateLevel(UINT value)
{
	updateLevel = value;
}

bool AComponent::IsValid()
{
	return isValid;
}

void AComponent::Destroy()
{
	if (isValid)
	{
		isValid = false;
		enabled = false;
		OnRemoved();
		if (gameObject)
		{
			vector<LPACOM>* pAcoms;
			gameObject->GetAllComponentsReference(&pAcoms);
			for (LPACOMITERATOR it = pAcoms->begin(); it != pAcoms->end();)
			{
				if (*it == thispointer)
					it = pAcoms->erase(it);
				else
					it++;
			}
			gameObject->SortComponents();
			gameObject.Reset();
		}
		thispointer.Reset();
	}
}

void* AComponent::GetUserData()
{
	return userdata;
}

void AComponent::SetUserData(void* pdata)
{
	userdata = pdata;
}

void AComponent::OnUpdate() {};

void AComponent::OnLateUpdate() {};

ISRESULT AComponent::OnPreAwake(IAContainer* actor) { return ISR_IGNORE; };

void AComponent::OnAwake() {};

void AComponent::OnStart() {};

void AComponent::OnRemoved() {};

HRESULT _stdcall AComponent::QueryInterface(const IID& iid, void** ppvObject)
{
	if (iid == IID_IAComponent)
	{
		*ppvObject = static_cast<IAComponent*>(this);
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

ULONG _stdcall AComponent::AddRef()
{
	return InterlockedIncrement(&mRef);
}

ULONG _stdcall AComponent::Release()
{
	ULONG val = InterlockedDecrement(&mRef);
	if (val == 0)
	{
		delete this;
	}
	return val;
}

GameObject* AComponent::GetGameObject()
{
	return gameObject.Get();
}