#include "../Include/PhysicsManager.h"
#include "../Include/Engine.h"

void ErrorCallback::reportError(PxErrorCode::Enum e, const char* message, const char* file, int line)
{
	const char* errorCode = NULL;

	switch (e)
	{
	case PxErrorCode::eNO_ERROR:
		errorCode = "no error";
		break;
	case PxErrorCode::eINVALID_PARAMETER:
		errorCode = "invalid parameter";
		break;
	case PxErrorCode::eINVALID_OPERATION:
		errorCode = "invalid operation";
		break;
	case PxErrorCode::eOUT_OF_MEMORY:
		errorCode = "out of memory";
		break;
	case PxErrorCode::eDEBUG_INFO:
		errorCode = "info";
		break;
	case PxErrorCode::eDEBUG_WARNING:
		errorCode = "warning";
		break;
	case PxErrorCode::ePERF_WARNING:
		errorCode = "performance warning";
		break;
	case PxErrorCode::eABORT:
		errorCode = "abort";
		break;
	case PxErrorCode::eINTERNAL_ERROR:
		errorCode = "internal error";
		break;
	case PxErrorCode::eMASK_ALL:
		errorCode = "unknown error";
		break;
	}

	if (errorCode)
	{
		char buffer[1024];
		sprintf_s(buffer, "%s (%d) : %s : %s\n", file, line, errorCode, message);
		engine->ReportError(buffer);
		if (e == PxErrorCode::eABORT)
		{
			engine->ReportFatalError("PhysXÄÚ²¿´íÎó£¡", FE_PHYSXINTERNAL);
		}
	}
}

PhysicsManager::PhysicsManager(Engine* eng) : engine(eng), gConstant({ 0,0,0 }), gMaterial(0), gDefaultMaterial(0)
{
	gErrorCallback.engine = eng;
}

HRESULT PhysicsManager::InitPhysX(PhysXDesc& desc)
{
	gConstant.gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	gConstant.gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gConstant.gFoundation, desc.tolerancesScale, false, 0);
	if (!gConstant.gPhysics)
	{
		gConstant.gFoundation->release();
		gConstant.gFoundation = 0;
		return ERR_INITPHYSX;
	}
	PxCookingParams cookingParams(gConstant.gPhysics->getTolerancesScale());
	cookingParams.midphaseDesc.setToDefault(PxMeshMidPhase::eBVH33);
	cookingParams.meshPreprocessParams = PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
	gConstant.gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gConstant.gFoundation, cookingParams);
	if (!desc.cpuDispatcher)
		gDispatcher = PxDefaultCpuDispatcherCreate(0);
	else
		gDispatcher = desc.cpuDispatcher;
	gDefaultMaterial = gMaterial = gConstant.gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	return SUCCEED_OPERATE;
}

void PhysicsManager::ReleasePhysX()
{
	if (gMaterial)
	{
		gMaterial->release();
		gMaterial = 0;
	}
	if (gConstant.gCooking)
	{
		gConstant.gCooking->release();
		gConstant.gCooking = 0;
	}
	if (gConstant.gPhysics)
	{
		gConstant.gPhysics->release();
		gConstant.gPhysics = 0;
	}
	if (gConstant.gFoundation)
	{
		gConstant.gFoundation->release();
		gConstant.gFoundation = 0;
	}
}

PxConstant* PhysicsManager::GetConstant()
{
	return &gConstant;
}

PxMaterial* PhysicsManager::GetDefaultMaterial()
{
	return gDefaultMaterial;
}

void PhysicsManager::SetDefaultMaterial(PxMaterial* pMaterial)
{
	gDefaultMaterial = pMaterial;
}