#pragma once
#include "../Interface/ISComponent.h"

class SComponent :public ISComponent
{
	friend class Scene;

public:

	SComponent();

	~SComponent() override;

	IScene* GetScene() override;

	void SetScene(IScene* pScene) override;

	int GetPriority() override;

	void SetPriority(int priority, bool autosort = true) override;

	bool IsValid() override;

	void Destroy() override;

	void* GetUserData() override;

	void SetUserData(void* pdata) override;

	ISRESULT OnPreAwake(IScene* pScene) override;

	void OnAwake() override;

	void OnRemoved() override;

	void OnPreUpdate() override;

	void OnPostUpdate() override;

	void OnPreProcessMessages() override;

	void OnPostProcessMessages() override;

	MSGRESULT OnMessageBegin(BYTE& msgType, LPVOID& param1, LPVOID& param2) override;

	void OnMessageEnd(BYTE msgType, LPVOID param1, LPVOID param2) override;

	HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) override;

	ULONG _stdcall AddRef() override;

	ULONG _stdcall Release() override;

private:

	ULONG mRef;

protected:

	ComPtr<IScene> scene;

	LPSCOM thispointer;

private:

	bool isValid;

	int priority;

	void* userdata;
};