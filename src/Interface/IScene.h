#pragma once
#include "../Common/EngineCommon.h"
#include "IVirtualBase.h"
#include "IActor.h"
#include "IDelegate.h"

struct ICamera;

MIDL_INTERFACE("50cc9f37-82af-f15c-7a65-9d84269de88a")
IScene :public IVirtualBase
{
	virtual struct IVirtualEngine* GetEngine() = 0;

	virtual void DetachActor(IActor* pActor) = 0;

	virtual HRESULT WriteMessageStream(BYTE msgType, LPVOID param1, LPVOID param2) = 0;

	virtual void ProcessMessages() = 0;

	virtual IActor* GetFirstActor() = 0;

	virtual IActor* GetLastActor() = 0;

	virtual void SceneUpdate() = 0;

	virtual bool HasComponent(ISComponent* com) = 0;

	virtual void RemoveComponent(ISComponent* com) = 0;

	virtual void GetAllComponents(vector<LPSCOM>* pArray, bool sort = true) = 0;

	virtual void GetAllComponentsReference(vector<LPSCOM>** ppArray, bool sort = false) = 0;

	virtual void SortComponents() = 0;

	virtual void SetMainCamera(ICamera* cam) = 0;

	virtual ICamera* GetMainCamera() = 0;

	virtual void Invoke(IDelegate* pDelegate) = 0;

	template <class T>
	T* AttachActor(T* pActor)
	{
		if (!IsValid())
		{
			pActor->Destroy();
			return 0;
		}
		if (!pActor->IsValid())
			return 0;
		if (pActor->GetScene())
			return 0;
		WriteMessageStream(SMSG_ATTACHACTOR, pActor, 0);
		static_cast<IActor*>(pActor)->OnAttachedToScene(this);
		return pActor;
	}

	template <class T>
	T* GetComponent()
	{
		vector<LPSCOM>* pScoms;
		GetAllComponentsReference(&pScoms, true);
		for (LPSCOMITERATOR it = pScoms->begin(); it != pScoms->end(); it++)
		{
			if (*it)
			{
				if (dynamic_cast<T*>((*it).Get()))
					return static_cast<T*>((*it).Get());
			}
		}
		return 0;
	}

	template <class T>
	void GetComponents(vector<ComPtr<T>>* pArray)
	{
		pArray->clear();
		vector<LPSCOM>* pScoms;
		GetAllComponentsReference(&pScoms, true);
		for (LPSCOMITERATOR it = pScoms->begin(); it != pScoms->end(); it++)
		{
			if (*it)
			{
				if (dynamic_cast<T*>((*it).Get()))
					pArray->emplace_back(static_cast<T*>((*it).Get()));
			}
		}
	}

	template <class T>
	T* AddComponent(T* com)
	{
		if (com && com->IsValid())
		{
			if (!IsValid())
			{
				com->Destroy();
				return 0;
			}
			ISComponent* scom = static_cast<ISComponent*>(com);
			if (scom->GetScene())
				return  0;
			if (scom->OnPreAwake(this) == ISR_IGNORE)
			{
				vector<LPSCOM>* pScoms;
				GetAllComponentsReference(&pScoms);
				pScoms->emplace_back(scom);
				SortComponents();
				scom->SetScene(this);
				scom->OnAwake();
				return com;
			}
		}
		return 0;
	}
};