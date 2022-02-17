#pragma once
#include "../../Interface/IScene.h"
#include "../../EasyDevelop/Camera.h"
#include "ManagedStream.h"
#include "Timer.h"
#include "PhysicsScene.h"

class Engine;

class Scene :public IScene, public PhysicsScene
{
	friend PhysicsScene;

public:

	Scene(Engine* engine);

	struct IVirtualEngine* GetEngine() override;

	void DetachActor(IActor* pActor) override;

	HRESULT WriteMessageStream(BYTE msgType, LPVOID param1, LPVOID param2) override;

	void ProcessMessages() override;

	bool IsValid() override;

	void Destroy() override;

	void* GetUserData() override;

	void SetUserData(void* pdata) override;

	IActor* GetFirstActor() override;

	IActor* GetLastActor() override;

	void SceneUpdate() override;

	bool HasComponent(ISComponent* com) override;

	void RemoveComponent(ISComponent* com) override;

	void GetAllComponents(vector<LPSCOM>* pArray, bool sort = true) override;

	void GetAllComponentsReference(vector<LPSCOM>** ppArray, bool sort = false) override;

	HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) override;

	ULONG _stdcall AddRef() override;

	ULONG _stdcall Release() override;

	void SortComponents() override;

	void SetMainCamera(ICamera* camera) override;

	ICamera* GetMainCamera() override;

	void Invoke(IDelegate* pDelegate) override;

private:

	ComPtr<ICamera> mainCamera = 0;

	ULONG mRef;

	MemoryStream msgStream1, msgStream2;

	MemoryStream* writingStream = &msgStream1;

	Engine* engine;

	bool isValid = true;

	LPACTOR firstActor = 0;

	LPACTOR lastActor = 0;

	vector<LPSCOM> components;

	vector<LPSCOM> sortedcoms;

	ComPtr<IScene> thispointer;

	vector<ComPtr<IDelegate>> sceneDelegates;

	void* userdata = 0;

	void RemoveFromActorList(LPACTOR actor);

	bool MessageBegin(BYTE& msgType, LPVOID& param1, LPVOID& param2, vector<LPSCOM>& pScoms);

	void MessageEnd(BYTE msgType, LPVOID param1, LPVOID param2, vector<LPSCOM>& pScoms);
};