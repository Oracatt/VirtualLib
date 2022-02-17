#pragma once
#include "../Common/EngineCommon.h"
#include "../Interface/ICamera.h"

inline Quaternion QuaternionRotateAxis(float radians, const Vector3& axis)
{
	Quaternion quat;
	XMStoreFloat4(&quat.f, XMQuaternionRotationAxis(XMLoadFloat3(&axis.f), radians));
	return quat;
}

inline Quaternion QuaternionRotateRollPitchYaw(float pitch, float yaw, float roll)
{
	Quaternion quat;
	XMStoreFloat4(&quat.f, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
	return quat;
}

inline Quaternion QuaternionSlerp(const Quaternion& quat1,const Quaternion& quat2,float t)
{
	Quaternion quat;
	XMStoreFloat4(&quat.f, XMQuaternionSlerp(XMLoadFloat4(&quat1.f), XMLoadFloat4(&quat1.f), t));
	return quat;
}

inline Quaternion QuaternionMultiply(const Quaternion& quat1, const Quaternion& quat2)
{
	Quaternion quat;
	XMStoreFloat4(&quat.f, XMQuaternionMultiply(XMLoadFloat4(&quat1.f), XMLoadFloat4(&quat2.f)));
	return quat;
}

inline Euler EulerFromQuaternion(const Quaternion& quat)
{
	Euler euler;
	float sx = 2 * (quat.w * quat.x - quat.y * quat.z);
	if (sx < -1.0)
		sx = -1.0;
	if (sx > 1.0)
		sx = 1.0;
	euler.roll = atan2(2 * (quat.w * quat.z + quat.x * quat.y), 1 - 2 * (quat.z * quat.z + quat.x * quat.x));
	euler.pitch = asin(sx);
	euler.yaw = atan2(2 * (quat.w * quat.y + quat.z * quat.x), 1 - 2 * (quat.x * quat.x + quat.y * quat.y));
	return euler;
}

inline XMMATRIX GetWorldMatrixByTransform(const Vector3& location, const Quaternion& rotation, const Vector3& scale)
{
	XMMATRIX matWorld = XMMatrixScaling(scale.x, scale.y, scale.z);
	matWorld *= XMMatrixRotationQuaternion(XMLoadFloat4(&rotation.f));
	matWorld *= XMMatrixTranslation(location.x, location.y, location.z);
	return matWorld;
}

inline Vector3 WorldToScreen(const Vector3& world, ICamera* camera, const USize& screenSize)
{
	Vector3 target;
	Vector3 location = camera->GetLocation();
	Quaternion rotation = camera->GetRotation();
	if (camera->GetType() == CAMERA_TYPE_PROJECTIVE)
	{
		XMMATRIX matvp = XMMatrixLookAtLH(location.GetXMVECTOR(), (location + Vector3{ 0,0,1 }).GetXMVECTOR(), XMVectorSet(0, 1, 0, 0)) *
			XMMatrixTranspose(XMMatrixRotationQuaternion(rotation.GetXMVECTOR())) *
			XMMatrixPerspectiveFovLH(camera->GetFov(), screenSize.width / (float)screenSize.height, 0.1, 1000);
		target = XMVector3TransformCoord(world.GetXMVECTOR(), matvp);
		target.x *= screenSize.width * 0.5f;
		target.y *= screenSize.height * 0.5f;
	}
	else
	{
		XMMATRIX matvp = XMMatrixLookAtLH(location.GetXMVECTOR(), (location + Vector3{ 0,0,1 }).GetXMVECTOR(), XMVectorSet(0, 1, 0, 0)) *
			XMMatrixTranspose(XMMatrixRotationQuaternion(rotation.GetXMVECTOR())) *
			XMMatrixOrthographicLH(screenSize.width, screenSize.height, 0.1, 1000);
		target = XMVector3TransformCoord(world.GetXMVECTOR(), matvp);
		target.x *= screenSize.width * 0.5f;
		target.y *= screenSize.height * 0.5f;
	}
	return target;
}

inline Vector3 ScreenToWorld(const Vector2& screen, float depth, ICamera* camera, const USize& screenSize)
{
	Vector3 target;
	Vector3 location = camera->GetLocation();
	Quaternion rotation = camera->GetRotation();
	Vector3 temp(screen);
	temp.x = temp.x * 2 / screenSize.width;
	temp.y = temp.y * 2 / screenSize.height;
	temp.z = depth;
	if (camera->GetType() == CAMERA_TYPE_PROJECTIVE)
	{
		XMMATRIX matvp = XMMatrixLookAtLH(location.GetXMVECTOR(), (location + Vector3{ 0,0,1 }).GetXMVECTOR(), XMVectorSet(0, 1, 0, 0)) *
			XMMatrixTranspose(XMMatrixRotationQuaternion(rotation.GetXMVECTOR())) *
			XMMatrixPerspectiveFovLH(camera->GetFov(), screenSize.width / (float)screenSize.height, 0.1, 1000);
		target = XMVector3TransformCoord(temp.GetXMVECTOR(), XMMatrixInverse(&XMMatrixDeterminant(matvp), matvp));
	}
	else
	{
		XMMATRIX matvp = XMMatrixLookAtLH(location.GetXMVECTOR(), (location + Vector3{ 0,0,1 }).GetXMVECTOR(), XMVectorSet(0, 1, 0, 0)) *
			XMMatrixTranspose(XMMatrixRotationQuaternion(rotation.GetXMVECTOR())) *
			XMMatrixOrthographicLH(screenSize.width, screenSize.height, 0.1, 1000);
		target = XMVector3TransformCoord(temp.GetXMVECTOR(), XMMatrixInverse(&XMMatrixDeterminant(matvp), matvp));
	}
	return target;
}