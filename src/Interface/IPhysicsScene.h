#pragma once
#include "../Common/EngineCommon.h"

MIDL_INTERFACE("b1235a28-49f3-4bdd-9612-62dcfc3fc0f3")
IPhysicsScene:public IUnknown
{
	virtual PxScene* GetPxScene() = 0;
};