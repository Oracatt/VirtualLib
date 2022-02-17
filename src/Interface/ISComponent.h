#pragma once
#include "../Common/EngineCommon.h"
#include "IScene.h"
#include "IVirtualBase.h"
#include "ManagedStream.h"

MIDL_INTERFACE("090c3c77-054d-4760-a870-e81f710426d0")
ISComponent:public IVirtualBase
{
	virtual ~ISComponent() = 0;

	virtual IScene* GetScene() = 0;

	virtual void SetScene(IScene* pScene ) = 0;

	virtual int GetPriority() = 0;

	virtual void SetPriority(int priority, bool autosort = true) = 0;

	virtual ISRESULT OnPreAwake(IScene* pScene) = 0;

	virtual void OnAwake() = 0;

	virtual void OnRemoved() = 0;

	virtual void OnPreUpdate() = 0;

	virtual void OnPostUpdate() = 0;

	virtual void OnPreProcessMessages() = 0;

	virtual void OnPostProcessMessages() = 0;

	virtual MSGRESULT OnMessageBegin(BYTE& msgType, LPVOID& param1, LPVOID& param2) = 0;

	virtual void OnMessageEnd(BYTE msgType, LPVOID param1, LPVOID param2) = 0;
};