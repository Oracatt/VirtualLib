#include "../Include/PhysicsScene.h"
#include "../Include/Scene.h"
#include "../Include/Engine.h"
#include "../../Extend/AComponents.h"
#include "../../EasyDevelop/GameObject.h"

PhysicsScene::PhysicsScene(Scene* pScene)
{
	mScene = pScene;
}

PxFilterFlags contactReportFilterShader(PxFilterObjectAttributes attributes0,
	PxFilterData filterData0,
	PxFilterObjectAttributes attributes1,
	PxFilterData filterData1,
	PxPairFlags& pairFlags,
	const void* constantBlock,
	PxU32 constantBlockSize)
{
	if (filterData0.word0 & filterData1.word0)
	{
		if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST | PxPairFlag::eNOTIFY_TOUCH_CCD | PxPairFlag::eDETECT_CCD_CONTACT;
			return PxFilterFlag::eNOTIFY;
		}
		else
		{
			pairFlags = PxPairFlag::eDETECT_CCD_CONTACT | PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST | PxPairFlag::eNOTIFY_TOUCH_CCD | PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
			return PxFilterFlag::eNOTIFY;
		}
	}
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT | PxPairFlag::eDETECT_CCD_CONTACT;
		return PxFilterFlag::eDEFAULT;
	}
	pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eDETECT_CCD_CONTACT;
	return PxFilterFlag::eDEFAULT;
}

class SimulationEventCallback :public PxSimulationEventCallback
{
public:
	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override {};

	void onWake(PxActor** actors, PxU32 count) override {};

	void onSleep(PxActor** actors, PxU32 count) override {};

	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override
	{
		for (int i = 0; i < nbPairs; i++)
		{
			Collider* c0 = (Collider*)pairs[i].shapes[0]->userData;
			Collider* c1 = (Collider*)pairs[i].shapes[1]->userData;
			if (c0 && c1)
			{
				if ((pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_FOUND))
				{
					c0->OnColliderEnter(c1);
					c1->OnColliderEnter(c0);
				}
				if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
				{
					c0->OnColliderStay(c1);
					c1->OnColliderStay(c0);
				}
				if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_LOST)
				{
					c0->OnColliderExit(c1);
					c1->OnColliderExit(c0);
				}
			}
		}
	};

	void onTrigger(PxTriggerPair* pairs, PxU32 count) override
	{
		for (int i = 0; i < count; i++)
		{
			Collider* c0 = (Collider*)pairs[i].triggerShape->userData;
			Collider* c1 = (Collider*)pairs[i].otherShape->userData;
			if (c0 && c1)
			{
				if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
				{
					c0->OnTriggerEnter(c1);
					c1->OnTriggerEnter(c0);
				}
				if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST)
				{
					c0->OnTriggerExit(c1);
					c1->OnTriggerExit(c0);
				}
			}
		}
	};

	void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override {};
};

class SimulationFilterCallback :public PxSimulationFilterCallback
{
public:
	PxFilterFlags pairFound(PxU32 pairID,
		PxFilterObjectAttributes attributes0, PxFilterData filterData0, const PxActor* a0, const PxShape* s0,
		PxFilterObjectAttributes attributes1, PxFilterData filterData1, const PxActor* a1, const PxShape* s1,
		PxPairFlags& pairFlags) override {
		return PxFilterFlag::eDEFAULT;
	};
	void pairLost(PxU32 pairID,
		PxFilterObjectAttributes attributes0,
		PxFilterData filterData0,
		PxFilterObjectAttributes attributes1,
		PxFilterData filterData1,
		bool objectRemoved) override {};
	bool	statusChange(PxU32& pairID, PxPairFlags& pairFlags, PxFilterFlags& filterFlags) override {
		return false;
	};
};

SimulationFilterCallback sfc;
SimulationEventCallback sec;

HRESULT PhysicsScene::InitPxScene(const PxSceneDesc* pDesc)
{
	if (!pDesc)
	{
		PhysXDesc des = mScene->engine->mDesc.physxDesc;
		PxSceneDesc sceneDesc(des.tolerancesScale);
		sceneDesc.gravity = PxVec3(0.0f, des.tolerancesScale.speed * -1, 0.0f);
		sceneDesc.flags |= PxSceneFlag::eENABLE_CCD | PxSceneFlag::eENABLE_STABILIZATION;
		sceneDesc.cpuDispatcher = mScene->engine->gDispatcher;
		sceneDesc.filterShader = contactReportFilterShader;
		sceneDesc.ccdThreshold = 100;
		sceneDesc.filterCallback = &sfc;
		gScene = mScene->engine->gConstant.gPhysics->createScene(sceneDesc);
		if (!gScene)
			return ERR_INVALIDARG;
	}
	else
	{
		gScene = mScene->engine->gConstant.gPhysics->createScene(*pDesc);
		if (!gScene)
			return ERR_INVALIDARG;
	}
	gScene->setSimulationEventCallback(&sec);
	return SUCCEED_OPERATE;
}



PxScene* PhysicsScene::GetPxScene()
{
	return gScene;
}

void PhysicsScene::ReleasePxScene()
{
	if (gScene)
	{
		gScene->release();
		gScene = 0;
	}
}

void PhysicsScene::Synchronize(bool isPxToScene)
{
	LPACTOR gp = mScene->firstActor;
	vector<LPACOM> acoms;
	ComPtr<IPhysicsActor> pActor;
	ComPtr<IAContainer> gContainer;
	for (; gp; gp = gp->GetNext())
	{
		gp.As(&gContainer);
		if (!gContainer)
			continue;
		acoms.clear();
		gContainer->GetAllComponents(&acoms);
		for (LPACOMITERATOR it = acoms.begin(); it != acoms.end(); it++)
		{
			(*it).As(&pActor);
			if (pActor)
				pActor->Synchronize(isPxToScene);
		}
	}
}