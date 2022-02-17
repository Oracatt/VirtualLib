#pragma once
#include "../Interface/IActor.h"
#include "../Interface/IAContainer.h"
#include "../Extend/AComponents.h"
#include "EasyDef.h"
#include <vector>

class GameObject :public IActor,public IAContainer
{
	friend class AComponent;

public:

	GameObject();

	~GameObject() override;

	string GetName() override;

	void SetName(const string& name) override;

	IScene* GetScene() override;

	void SetScene(IScene* pScene) override;

	bool IsValid() override;

	void Destroy() override;

	void* GetUserData() override;

	void SetUserData(void* pdata) override;

	HRESULT InsertBefore(IActor* pActor) override;

	HRESULT InsertAfter(IActor* pActor) override;

	HRESULT SendToFirst() override;

	HRESULT SendToLast() override;

	UINT GetUpdateLevel() override;

	void SetUpdateLevel(UINT value) override;

	void OnUpdate() override;

	void OnDestroy() override;

	virtual void OnCreated();

	bool HasComponent(IAComponent* com) override;

	void RemoveComponent(IAComponent* com) override;

	void GetAllComponents(vector<LPACOM>* pArray, bool sort = true) override;

	void GetAllComponentsReference(vector<LPACOM>** ppArray, bool sort = false) override;

	IActor* GetLast() override;

	IActor* GetNext() override;

	void SetLast(IActor* actor) override;

	void SetNext(IActor* actor) override;

	HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) override;

	ULONG _stdcall AddRef() override;

	ULONG _stdcall Release() override;

	void SortComponents() override;

private:

	ULONG mRef;

protected:

	vector<LPACOM> components;

	vector<LPACOM> sortedcoms;

	LPACTOR thispointer;

	ComPtr<IScene> scene;

	UINT updateLevel;

private:

	bool isValid;

	string name;

	LPACTOR last;

	LPACTOR next;

	void* userdata;

	void OnComponentAdd(IAComponent* pAcom) final;

	void OnAttachedToScene(IScene* pScene) final;

	void RemoveAllComponents() final;

public:

	const ComPtr<Transform> transform;

	const ComPtr<Transform2D> transform2D;
};