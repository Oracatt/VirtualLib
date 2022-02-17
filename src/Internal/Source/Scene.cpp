#include "../Include/Scene.h"
#include "../Include/Engine.h"
#include "../../EasyDevelop/GameObject.h"
#include "../../EasyDevelop/SceneCom.h"
#include <algorithm>

#define CR(a) if(FAILED(a)){return ERR_OUTOFMEMORY;}

bool sortfunc(const LPSCOM & sc1, const LPSCOM & sc2)
{
	return sc1->GetPriority() > sc2->GetPriority();
}

Scene::Scene(Engine* eng) :engine(eng), PhysicsScene(this)
{
	mRef = 0;
	thispointer = this;
}

IVirtualEngine* Scene::GetEngine()
{
	return static_cast<IVirtualEngine*>(engine);
}

void Scene::DetachActor(IActor* pActor)
{
	if (pActor->GetScene() == this)
		pActor->Destroy();
}

HRESULT Scene::WriteMessageStream(BYTE msgType, LPVOID param1, LPVOID param2)
{
	switch (msgType)
	{
	case SMSG_SCENEAWAKE:
	case SMSG_DESTROYSCENE:
		CR(writingStream->Write(&msgType, 0, sizeof(BYTE)));
		return SUCCEED_OPERATE;
	case SMSG_CHANGESCENE:
		CR(writingStream->Write(&msgType, 0, sizeof(BYTE)));
		CR(writingStream->Write(&param1, 0, sizeof(LPVOID)));
		return SUCCEED_OPERATE;
	case SMSG_ATTACHACTOR:
	case SMSG_DESTROYACTOR:
	case SMSG_SENDTOFIRST:
	case SMSG_SENDTOLAST:
		if (param1)
		{
			CR(writingStream->Write(&msgType, 0, sizeof(BYTE)));
			CR(writingStream->Write(&param1, 0, sizeof(LPVOID)));
			return SUCCEED_OPERATE;
		}
		return ERR_INVALIDARG;
	case SMSG_INSERTAFTER:
	case SMSG_INSERTBEFORE:
		if (param1 && param2)
		{
			CR(writingStream->Write(&msgType, 0, sizeof(BYTE)));
			CR(writingStream->Write(&param1, 0, sizeof(LPVOID)));
			CR(writingStream->Write(&param2, 0, sizeof(LPVOID)));
			return SUCCEED_OPERATE;
		}
		return ERR_INVALIDARG;
	default:
		CR(writingStream->Write(&msgType, 0, sizeof(BYTE)));
		CR(writingStream->Write(&param1, 0, sizeof(LPVOID)));
		CR(writingStream->Write(&param2, 0, sizeof(LPVOID)));
		return SUCCEED_OPERATE;
	}
	return ERR_INVALIDARG;
}

void Scene::ProcessMessages()
{
	IScene* nextScene = 0;
	bool changeIsSignalled = false;
	bool destroyIsSignalled = false;
	vector<LPSCOM> coms;
	GetAllComponents(&coms);
	for (LPSCOMITERATOR it = coms.begin(); it != coms.end(); it++)
	{
		(*it)->OnPreProcessMessages();
	}
	while (msgStream1.GetLength() > 0 || msgStream2.GetLength() > 0|| !sceneDelegates.empty())
	{
		vector<ComPtr<IDelegate>> delTemp = sceneDelegates;
		sceneDelegates.clear();
		for (auto e : delTemp)
			e->Invoke();

		MemoryStream* msgStream = writingStream;
		writingStream = ((writingStream == &msgStream1) ? &msgStream2 : &msgStream1);
		msgStream->SetPosition(0);
		IActor* a1, * a2;
		IScene* s1;
		LPVOID nullp1 = 0, nullp2 = 0;
		LPACTOR actor1, actor2;
		BYTE msgType;
		bool notify = true;
		while (msgStream->GetPosition() < msgStream->GetLength())
		{
			msgStream->Read(&msgType, 0, sizeof(BYTE));
			nullp1 = 0;
			nullp2 = 0;
			notify = (~msgType) & SMSG_NONOTIFY;
			msgType &= ~SMSG_NONOTIFY;
			switch (msgType)
			{
			case SMSG_ATTACHACTOR:
				msgStream->Read(&a1, 0, sizeof(LPVOID));
				if (notify && !MessageBegin(msgType, (LPVOID&)a1, nullp1, coms))
					break;
				actor1 = a1;
				if (!firstActor)
				{
					firstActor = actor1;
					lastActor = actor1;
				}
				else
				{
					lastActor->SetNext(actor1.Get());
					actor1->SetLast(lastActor.Get());
					lastActor = actor1;
				}
				if (destroyIsSignalled)
					actor1->Destroy();
				MessageEnd(msgType, (LPVOID)a1, nullp1, coms);
				break;
			case SMSG_DESTROYACTOR:
				msgStream->Read(&a1, 0, sizeof(LPVOID));
				if (notify && !MessageBegin(msgType, (LPVOID&)a1, nullp1, coms))
					break;
				actor1 = a1;
				RemoveFromActorList(actor1);
				actor1->SetScene(0);
				actor1->Release();
				MessageEnd(msgType, (LPVOID)a1, nullp1, coms);
				break;
			case SMSG_INSERTAFTER:
				msgStream->Read(&a1, 0, sizeof(LPVOID));
				msgStream->Read(&a2, 0, sizeof(LPVOID));
				if (notify && !MessageBegin(msgType, (LPVOID&)a1, (LPVOID&)a2, coms))
					break;
				actor1 = a1;
				actor2 = a2;
				RemoveFromActorList(actor1);
				if (lastActor == actor2)
				{
					actor2->SetNext(actor1.Get());
					actor1->SetLast(actor2.Get());
					actor1->SetNext(0);
					lastActor = actor1;
				}
				else
				{
					actor1->SetLast(actor2.Get());
					actor1->SetNext(actor2->GetNext());
					actor2->GetNext()->SetLast(actor1.Get());
					actor2->SetNext(actor1.Get());
				}
				MessageEnd(msgType, (LPVOID)a1, (LPVOID)a2, coms);
				break;
			case SMSG_INSERTBEFORE:
				msgStream->Read(&a1, 0, sizeof(LPVOID));
				msgStream->Read(&a2, 0, sizeof(LPVOID));
				if (notify && !MessageBegin(msgType, (LPVOID&)a1, (LPVOID&)a2, coms))
					break;
				actor1 = a1;
				actor2 = a2;
				RemoveFromActorList(actor1);
				if (firstActor == actor2)
				{
					actor2->SetLast(actor1.Get());
					actor1->SetNext(actor2.Get());
					actor1->SetLast(0);
					firstActor = actor1;
				}
				else
				{
					actor1->SetNext(actor2.Get());
					actor1->SetLast(actor2->GetLast());
					actor2->GetLast()->SetNext(actor1.Get());
					actor2->SetLast(actor1.Get());
				}
				MessageEnd(msgType, (LPVOID)a1, (LPVOID)a2, coms);
				break;
			case SMSG_SENDTOFIRST:
				msgStream->Read(&a1, 0, sizeof(LPVOID));
				if (notify && !MessageBegin(msgType, (LPVOID&)a1, nullp1, coms))
					break;
				actor1 = a1;
				if (!firstActor)
					break;
				if (firstActor == actor1)
					break;
				else
				{
					RemoveFromActorList(actor1);
					firstActor->SetLast(actor1.Get());
					actor1->SetNext(firstActor.Get());
					actor1->SetLast(0);
					firstActor = actor1;
				}
				MessageEnd(msgType, (LPVOID)a1, nullp1, coms);
				break;
			case SMSG_SENDTOLAST:
				msgStream->Read(&a1, 0, sizeof(LPVOID));
				if (notify && !MessageBegin(msgType, (LPVOID&)a1, nullp1, coms))
					break;
				actor1 = a1;
				if (!firstActor)
					break;
				if (lastActor == actor1)
					break;
				else
				{
					RemoveFromActorList(actor1.Get());
					lastActor->SetNext(actor1.Get());
					actor1->SetLast(lastActor.Get());
					actor1->SetNext(0);
					lastActor = actor1;
				}
				MessageEnd(msgType, (LPVOID)a1, nullp1, coms);
				break;
			case SMSG_DESTROYSCENE:
				if (notify && !MessageBegin(msgType, nullp1, nullp2, coms))
					break;
				if (!destroyIsSignalled)
					destroyIsSignalled = true;
				MessageEnd(msgType, nullp2, nullp2, coms);
				break;
			case SMSG_CHANGESCENE:
				msgStream->Read(&nextScene, 0, sizeof(LPVOID));
				if (notify && !MessageBegin(msgType, (LPVOID&)nextScene, nullp1, coms))
					break;
				changeIsSignalled = true;
				MessageEnd(msgType, (LPVOID)nextScene, nullp1, coms);
				break;
			case SMSG_SCENEAWAKE:
				if (notify && !MessageBegin(msgType, nullp1, nullp2, coms))
					break;
				MessageEnd(msgType, nullp1, nullp2, coms);
				break;
			default:
				msgStream->Read(&a1, 0, sizeof(LPVOID));
				msgStream->Read(&a2, 0, sizeof(LPVOID));
				if (notify && !MessageBegin(msgType, (LPVOID&)a1, (LPVOID&)a2, coms))
					break;
				MessageEnd(msgType, (LPVOID)a1, (LPVOID)a2, coms);
				break;
			}
		}
		msgStream->SetLength(0);
	}
	for (LPSCOMITERATOR it = coms.begin(); it != coms.end(); it++)
	{
		(*it)->OnPostProcessMessages();
	}
	Engine* etemp = engine;
	if (destroyIsSignalled)
	{
		SComponent* com;
		for (LPSCOMITERATOR i = components.begin(); i != components.end(); i++)
		{
			if (*i)
			{
				com = static_cast<SComponent*>((*i).Get());
				com->isValid = false;
				com->OnRemoved();
				com->scene.Reset();
				com->thispointer.Reset();
			}
		}
		components.clear();
		sortedcoms.clear();
		ReleasePxScene();
		for (vector<ComPtr<IScene>>::iterator it = etemp->mScenes.begin(); it != etemp->mScenes.end();)
		{
			if ((*it).Get() == this)
				it = etemp->mScenes.erase(it);
			else
				it++;
		}
		mainCamera.Reset();
		firstActor.Reset();
		lastActor.Reset();
		sceneDelegates.clear();
		thispointer.Reset();
		if (etemp->GetNowScene() == this)
			etemp->nowScene.Reset();
	}
	if (changeIsSignalled)
	{
		etemp->nowScene = nextScene;
		if (nextScene)
			nextScene->WriteMessageStream(SMSG_SCENEAWAKE, 0, 0);
	}
}

bool Scene::IsValid()
{
	return isValid;
}

void Scene::Destroy()
{
	if (isValid)
	{
		isValid = false;
		for (LPACTOR act = firstActor; act; act = act->GetNext())
			act->Destroy();
		WriteMessageStream(SMSG_DESTROYSCENE, 0, 0);
	}
}

void* Scene::GetUserData()
{
	return userdata;
}

void Scene::SetUserData(void* pdata)
{
	userdata = pdata;
}

IActor* Scene::GetFirstActor()
{
	return firstActor.Get();
}

IActor* Scene::GetLastActor()
{
	return lastActor.Get();
}

void Scene::SceneUpdate()
{
	vector<LPSCOM> coms;
	GetAllComponents(&coms);
	for (LPSCOMITERATOR it = coms.begin(); it != coms.end(); it++)
	{
		(*it)->OnPreUpdate();
	}
	gScene->simulate(engine->GetDeltaTime());
	gScene->fetchResults(true);
	Synchronize(true);
	int timeStopLevel = engine->GetTimeStopLevel();
	LPACTOR gp = firstActor;
	vector<LPACOM> acoms;
	ComPtr<IAContainer> gContainer;
	for (; gp; gp = gp->GetNext())
	{
		if (!gp->IsValid())
			continue;
		if (gp->GetUpdateLevel() >= timeStopLevel)
			gp->OnUpdate();
		gp.As(&gContainer);
		if (!gContainer)
			continue;
		acoms.clear();
		gContainer->GetAllComponents(&acoms);
		for (LPACOMITERATOR it = acoms.begin(); it != acoms.end(); it++)
		{
			if (!(*it)->GetActive() || !(*it)->IsValid())
				continue;
			if ((*it)->GetUpdateLevel() < timeStopLevel)
				continue;
			if (!(*it)->IsStarted())
			{
				(*it)->SetStart();
				(*it)->OnStart();
			}
			(*it)->OnUpdate();
		}
	}
	gp = firstActor;
	for (; gp; gp = gp->GetNext())
	{
		if (!gp->IsValid())
			continue;
		gp.As(&gContainer);
		if (!gContainer)
			continue;
		acoms.clear();
		gContainer->GetAllComponents(&acoms);
		for (LPACOMITERATOR it = acoms.begin(); it != acoms.end(); it++)
		{
			if (!(*it)->GetActive() || !(*it)->IsValid())
				continue;
			if ((*it)->GetUpdateLevel() < timeStopLevel)
				continue;
			(*it)->OnLateUpdate();
		}
	}
	Synchronize(false);
	for (LPSCOMITERATOR it = coms.begin(); it != coms.end(); it++)
	{
		(*it)->OnPostUpdate();
	}
}

bool Scene::HasComponent(ISComponent* com)
{
	if (com)
	{
		for (LPSCOMITERATOR it = components.begin(); it != components.end(); it++)
		{
			if ((*it).Get() == com)
				return true;
		}
	}
	return false;
}

void Scene::RemoveComponent(ISComponent* com)
{
	if (HasComponent(com))
		com->Destroy();
}

void Scene::GetAllComponents(vector<LPSCOM>* pArray,bool sort)
{
	if (sort)
		*pArray = sortedcoms;
	else
		*pArray = components;
}

void Scene::GetAllComponentsReference(vector<LPSCOM>** ppArray, bool sort)
{
	if (sort)
		*ppArray = &sortedcoms;
	else
		*ppArray = &components;
}

HRESULT _stdcall Scene::QueryInterface(const IID& iid, void** ppvObject)
{
	if (iid == IID_IScene)
	{
		*ppvObject = static_cast<IScene*>(this);
	}
	else if (iid == IID_IPhysicsScene)
	{
		*ppvObject = static_cast<IPhysicsScene*>(this);
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

ULONG _stdcall Scene::AddRef()
{
	return InterlockedIncrement(&mRef);
}

ULONG _stdcall Scene::Release()
{
	ULONG val = InterlockedDecrement(&mRef);
	if (val == 0)
	{
		delete this;
	}
	return val;
}

void Scene::RemoveFromActorList(LPACTOR actor)
{
	if (firstActor == actor && lastActor == actor)
	{
		firstActor.Reset();
		lastActor.Reset();
	}
	else if (firstActor == actor)
	{
		actor->GetNext()->SetLast(0);
		firstActor = actor->GetNext();
	}
	else if (lastActor == actor)
	{
		actor->GetLast()->SetNext(0);
		lastActor = actor->GetLast();
	}
	else
	{
		actor->GetLast()->SetNext(actor->GetNext());
		actor->GetNext()->SetLast(actor->GetLast());
	}
	actor->SetLast(0);
	actor->SetNext(0);
}

bool Scene::MessageBegin(BYTE& msgType, LPVOID& param1, LPVOID& param2, vector<LPSCOM>& pScoms)
{
	for (LPSCOMITERATOR it = pScoms.begin(); it != pScoms.end(); it++)
	{
		BYTE tmsgType = msgType;
		LPVOID ta1 = param1;
		LPVOID ta2 = param2;
		MSGRESULT mr = (*it)->OnMessageBegin(tmsgType, ta1, ta2);
		switch (mr)
		{
		case MSGR_OVERLAP:
			msgType = tmsgType;
			param1 = ta1;
			param2 = ta2;
			break;
		case MSGR_SUPERCEDE:
			return false;
		default:
			break;
		}
	}
	return true;
}

void Scene::MessageEnd(BYTE msgType, LPVOID param1, LPVOID param2, vector<LPSCOM>& pScoms)
{
	for (LPSCOMITERATOR it = pScoms.begin(); it != pScoms.end(); it++)
	{
		(*it)->OnMessageEnd(msgType, param1, param2);
	}
}

void Scene::SortComponents()
{
	sortedcoms.clear();
	sortedcoms = components;
	if (sortedcoms.size() > 1)
		sort(sortedcoms.begin(), sortedcoms.end(), sortfunc);
}

ICamera* Scene::GetMainCamera()
{
	return mainCamera.Get();
}

void Scene::SetMainCamera(ICamera* camera)
{
	mainCamera = camera;
}

void Scene::Invoke(IDelegate* pDelegate)
{
	if (IsValid())
	{
		WaitForSingleObject(engine->sceneDelegateMutex, INFINITE);
		sceneDelegates.emplace_back(pDelegate);
		ReleaseMutex(engine->sceneDelegateMutex);
	}
	else
	{
		pDelegate->AddRef();
		pDelegate->Release();
	}
}