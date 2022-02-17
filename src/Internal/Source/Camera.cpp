#include "../../EasyDevelop/Camera.h"
#include "../Include/Scene.h"

ICamera::~ICamera() {}

Camera::Camera(CAMERA_TYPE ctype, USize orthogonalSize, float f) :fov(f), type(ctype), oSize(orthogonalSize) 
{
	UpdateData();
}

ICamera* Camera::GetParent()
{
	return static_cast<ICamera*>(this);
}

void Camera::OnDestroy()
{
	if (scene && scene->GetMainCamera() == static_cast<ICamera*>(this))
		scene->SetMainCamera(0);
}

void Camera::OnCreated()
{
	if (!scene->GetMainCamera())
		scene->SetMainCamera(this);
}

float Camera::GetFov()
{
	return m_fov;
}

void Camera::SetFov(float fovY)
{
	fov = fovY;
}

Vector3 Camera::GetLocation()
{
	return m_location;
}

Quaternion Camera::GetRotation()
{
	return m_rotation;
}

CAMERA_TYPE Camera::GetType()
{
	return m_type;
}

void Camera::SetType(CAMERA_TYPE ctype)
{
	type = ctype;
}

HRESULT _stdcall Camera::QueryInterface(const IID& iid, void** ppvObject)
{
	if (iid == IID_IActor)
	{
		*ppvObject = static_cast<IActor*>(this);
	}
	else if (iid == IID_IAContainer)
	{
		*ppvObject = static_cast<IAContainer*>(this);
	}
	else if (iid == IID_ICamera)
	{
		*ppvObject = static_cast<ICamera*>(this);
	}
	else if (iid == IID_IVirtualBase)
	{
		*ppvObject = static_cast<IVirtualBase*>(static_cast<IActor*>(this));
	}
	else if (iid == IID_IRenderable)
	{
		*ppvObject = static_cast<IRenderable*>(this);
	}
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	static_cast<IUnknown*>(*ppvObject)->AddRef();
	return S_OK;
}

ULONG _stdcall Camera::AddRef()
{
	return GameObject::AddRef();
}

ULONG _stdcall Camera::Release()
{
	return GameObject::Release();
}

bool Camera::IsValid()
{
	return GameObject::IsValid();
}

void Camera::Destroy()
{
	GameObject::Destroy();
}

void* Camera::GetUserData()
{
	return GameObject::GetUserData();
}

void Camera::SetUserData(void* pdata)
{
	GameObject::SetUserData(pdata);
}

USize Camera::GetOrthogonalSize()
{
	return m_oSize;
}

void Camera::SetOrthogonalSize(const USize& size)
{
	oSize = size;
}

void Camera::OnFetchData()
{
	UpdateData();
}

void Camera::OnRender() {}

void Camera::OnRender2D() {}

void Camera::UpdateData()
{
	m_fov = fov;;
	m_type = type;
	m_oSize = oSize;
	m_location = transform->location;
	m_rotation = transform->rotation;
}