#pragma once
#include "../../Interface/IPhysicsManager.h"

class ErrorCallback : public PxDefaultErrorCallback
{
public:

	class Engine* engine;

	void reportError(PxErrorCode::Enum e, const char* message, const char* file, int line) override;
};

class PhysicsManager :public IPhysicsManager
{
public:

	PhysicsManager(class Engine* engine);

	HRESULT InitPhysX(PhysXDesc& desc);

	void ReleasePhysX();

	PxConstant* GetConstant() override;

	PxMaterial* GetDefaultMaterial() override;

	void SetDefaultMaterial(PxMaterial* pMaterial) override;

protected:

	PxDefaultAllocator gAllocator;

	ErrorCallback gErrorCallback;

	PxConstant gConstant;

	PxCpuDispatcher* gDispatcher;

	PxMaterial* gMaterial;

	PxMaterial* gDefaultMaterial;

private:

	class Engine* engine;
};