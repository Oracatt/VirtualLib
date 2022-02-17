#pragma once
#include "../Interface/IAComponent.h"
#include "EasyDef.h"

class AComponent :public IAComponent
{
	friend class GameObject;

public:

	AComponent();

	~AComponent() override;

	IAContainer* GetContainer() override;

	void SetContainer(IAContainer* pContainer) override;

	bool IsStarted() override;

	void SetStart() override;

	bool GetActive() override;

	void SetActive(bool active) override;

	int GetPriority() override;

	void SetPriority(int priority, bool autosort = true) override;

	UINT GetUpdateLevel() override;

	void SetUpdateLevel(UINT value) override;

	bool IsValid() override;

	void Destroy() override;

	void* GetUserData() override;

	void SetUserData(void* pdata) override;

	void OnUpdate() override;

	void OnLateUpdate() override;

	ISRESULT OnPreAwake(IAContainer* actor) override;

	void OnAwake() override;

	void OnStart() override;

	void OnRemoved() override;
		
	HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) override;

	ULONG _stdcall AddRef() override;

	ULONG _stdcall Release() override;

	GameObject* GetGameObject();

private:

	ULONG mRef;

protected:

	LPGOBJ gameObject;

	LPACOM thispointer;

	bool enabled;

	UINT updateLevel;

private:

	bool isValid;

	bool started;

	//ִ��˳���ȱȽ�IActor˳�򣨿�ǰ����ִ�У�����αȽ�priority��ֵ�����ִ�У������Ƚ�IAComponent���˳�򣨿�ǰ����ִ�У�
	int priority;

	void* userdata;

};
