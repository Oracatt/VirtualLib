#pragma once
#include "../../Interface/IPhysicsScene.h"

class PhysicsScene :public IPhysicsScene
{
public:

	PhysicsScene(struct Scene* pScene);

	HRESULT InitPxScene(const PxSceneDesc* pDesc);

	void ReleasePxScene();

	PxScene* GetPxScene() override;

protected:

	PxScene* gScene = 0;

	void Synchronize(bool isPxToScene);

private:

	struct Scene* mScene;
};