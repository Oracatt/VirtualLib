#pragma once
#include "../Common/EngineCommon.h"

MIDL_INTERFACE("16aad442-c7c3-4400-af6a-96f4d13f711d")
IPhysicsManager:public IUnknown
{
	virtual PxConstant* GetConstant() = 0;

	virtual PxMaterial* GetDefaultMaterial() = 0;

	virtual void SetDefaultMaterial(PxMaterial* pMaterial) = 0;
};