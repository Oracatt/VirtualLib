#pragma once
#include "GameObject.h"
#include "../Interface/ICamera.h"
#include "../Interface/IRenderable.h"

class Camera :public GameObject, public ICamera ,public IRenderable
{
public:

	float fov;

	CAMERA_TYPE type;

	USize oSize;

	Camera(CAMERA_TYPE type, USize orthogonalSize = { 0,0 }, float fov = M_PI / 4);

	ICamera* GetParent();

	void OnDestroy() override;

	void OnCreated() override;

	float GetFov() override;

	void SetFov(float fovY) override;

	Vector3 GetLocation() override;

	Quaternion GetRotation() override;

	CAMERA_TYPE GetType() override;

	void SetType(CAMERA_TYPE type) override;

	HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) override;

	ULONG _stdcall AddRef() override;

	ULONG _stdcall Release() override;

	bool IsValid() override;

	void Destroy() override;

	void* GetUserData() override;

	void SetUserData(void* pdata) override;

	USize GetOrthogonalSize() override;

	void SetOrthogonalSize(const USize& size) override;

	void OnFetchData() override;

	void OnRender() override;

	void OnRender2D() override;

	void UpdateData();

private:

	float m_fov;

	CAMERA_TYPE m_type;

	USize m_oSize;

	Vector3 m_location;

	Quaternion m_rotation;
};