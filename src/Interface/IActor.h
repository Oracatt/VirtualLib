#pragma once
#include "../Common/EngineCommon.h"
#include "IVirtualBase.h"

struct IScene;

MIDL_INTERFACE("63bd739b-aaa1-4ee2-a571-da17863646ab")
IActor:public IVirtualBase
{

	virtual ~IActor() = 0;

	virtual string GetName() = 0;

	virtual void SetName(const string& name) = 0;

	virtual IScene* GetScene() = 0;

	virtual void SetScene(IScene* pScene) = 0;

	virtual HRESULT InsertBefore(IActor* actor) = 0;

	virtual HRESULT InsertAfter(IActor* actor) = 0;

	virtual HRESULT SendToFirst() = 0;

	virtual HRESULT SendToLast() = 0;

	virtual UINT GetUpdateLevel() = 0;

	virtual void SetUpdateLevel(UINT value) = 0;

	virtual void OnUpdate() = 0;

	virtual void OnDestroy() = 0;

	virtual void OnAttachedToScene(IScene* pScene) = 0;

	virtual IActor* GetLast() = 0;

	virtual IActor* GetNext() = 0;

	//一般情况下勿使用，除非你知道自己在做什么
	virtual void SetLast(IActor* actor) = 0;

	//一般情况下勿使用，除非你知道自己在做什么
	virtual void SetNext(IActor* actor) = 0;

};