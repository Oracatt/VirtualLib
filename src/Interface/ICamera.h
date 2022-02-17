#pragma once
#include "../Common/EngineCommon.h"
#include "IVirtualBase.h"
#include "IAContainer.h"

MIDL_INTERFACE("7437872e-0405-45e3-a366-1858b488d2f8")
ICamera:public IVirtualBase
{
	virtual ~ICamera() = 0;

	virtual float GetFov() = 0;

	virtual void SetFov(float fovY) = 0;

	virtual Vector3 GetLocation() = 0;

	virtual Quaternion GetRotation() = 0;

	virtual CAMERA_TYPE GetType() = 0;

	virtual void SetType(CAMERA_TYPE type) = 0;

	virtual USize GetOrthogonalSize() = 0;

	virtual void SetOrthogonalSize(const USize& size) = 0;
};