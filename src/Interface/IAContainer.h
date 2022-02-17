#pragma once
#include "../Common/EngineCommon.h"
#include "IVirtualBase.h"

MIDL_INTERFACE("86d01e77-cea1-483d-a1b6-a2ccf0eb9c15")
IAContainer:public IVirtualBase
{
	virtual ~IAContainer() = 0;

	virtual void OnComponentAdd(IAComponent* pAcom) = 0;

	virtual bool HasComponent(IAComponent* com) = 0;

	virtual void RemoveComponent(IAComponent* com) = 0;

	virtual void RemoveAllComponents() = 0;

	virtual void GetAllComponents(vector<LPACOM>* pArray, bool sort = true) = 0;

	virtual void GetAllComponentsReference(vector<LPACOM>** ppArray, bool sort = false) = 0;

	virtual void SortComponents() = 0;

	template <class T>
	T* GetComponent()
	{
		vector<LPACOM>* pAcoms;
		GetAllComponentsReference(&pAcoms, true);
		for (LPACOMITERATOR it = pAcoms->begin(); it != pAcoms->end(); it++)
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
		vector<LPACOM>* pAcoms;
		GetAllComponentsReference(&pAcoms, true);
		for (LPACOMITERATOR it = pAcoms->begin(); it != pAcoms->end(); it++)
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
			IAComponent* acom = static_cast<IAComponent*>(com);
			if (acom->GetContainer())
				return 0;
			if (acom->OnPreAwake(this) == ISR_IGNORE)
			{
				vector<LPACOM>* pAcoms;
				GetAllComponentsReference(&pAcoms);
				pAcoms->emplace_back(acom);
				SortComponents();
				acom->SetContainer(this);
				OnComponentAdd(acom);
				return com;
			}
		}
		return 0;
	}
};