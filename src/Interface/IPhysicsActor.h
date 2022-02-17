#pragma once
#include "../Common/EngineCommon.h"

MIDL_INTERFACE("58db42ad-9fb8-4f8d-97b9-6925e31e1cfb")
IPhysicsActor:public IUnknown
{
	virtual void Synchronize(bool isPxToScene) = 0;
};