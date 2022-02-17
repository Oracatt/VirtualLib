#pragma once
#include "../Common/EngineCommon.h"
#include "IVirtualBase.h"
#include "IAContainer.h"

MIDL_INTERFACE("7437872e-0405-45e3-a366-1858b488d2f8")
IAComponent:public IVirtualBase
{
	virtual ~IAComponent() = 0;

	virtual IAContainer* GetContainer() = 0;

	virtual void SetContainer(IAContainer* pContainer) = 0;

	virtual bool IsStarted() = 0;

	virtual void SetStart() = 0;

	virtual bool GetActive() = 0;

	virtual void SetActive(bool active) = 0;

	virtual int GetPriority() = 0;

	virtual void SetPriority(int priority, bool autosort = true) = 0;

	virtual UINT GetUpdateLevel() = 0;

	virtual void SetUpdateLevel(UINT value) = 0;

	virtual void OnUpdate() = 0;

	virtual void OnLateUpdate() = 0;

	virtual ISRESULT OnPreAwake(IAContainer* actor) = 0;

	virtual void OnAwake() = 0;

	virtual void OnStart() = 0;

	virtual void OnRemoved() = 0;
};