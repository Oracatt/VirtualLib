#pragma once
#include <iostream>
#include <d3d11.h>
#include <D3D11SDKLayers.h>
#include <dxgi.h>
#include <D3D11Shader.h>
#include <DirectXMath.h>
#include "EngineDef.h"
#include <vector>
#include <corecrt_math_defines.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <d2d1_1.h>
#include <dwrite_3.h>
#include <functional>
#include "../PhysX/include/PxPhysicsAPI.h"
#include "../FXMaster/Include/d3dx11effect.h"
#include <d3d11_4.h>

using namespace std;
using namespace physx;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace D2D1;

struct IActor;
struct IAComponent;
struct ISComponent;

class EngineEventCallBack
{
public:

	virtual bool GetActive()
	{
		return active;
	}

	virtual void SetActive(bool active)
	{
		this->active = active;
	}

	virtual void OnEngineInitialized(struct IVirtualEngine* engine) = 0;

	virtual void OnEngineDestroy() = 0;

	virtual void OnStartRender() = 0;

	virtual void OnBufferResized(UINT width, UINT height) = 0;

	virtual void RecvShaderData(UINT type, LPVOID data1, LPVOID data2) = 0;

protected:

	bool active;

	EngineEventCallBack() :active(true) {}
};

class EngineErrorCallBack
{
public:

	virtual void ReportFatalError(LPCSTR msg, int errcode) = 0;

	virtual void ReportError(LPCSTR msg) = 0;
};

typedef struct _VECTOR2
{
	union
	{
		XMFLOAT2 f;
		struct
		{
			float x, y;
		};
	};

	inline _VECTOR2(XMVECTOR _v)
	{
		XMStoreFloat2(&f, _v);
	}

	inline _VECTOR2(float _x, float _y)
	{
		XMStoreFloat2(&f, XMVectorSet(_x, _y, 0, 0));
	}

	template <typename T>
	inline _VECTOR2(const T& _v)
	{
		XMStoreFloat2(&f, _v.GetXMVECTOR());
	}

	inline _VECTOR2()
	{
		XMStoreFloat2(&f, XMVectorSet(0, 0, 0, 0));
	}

	inline void Set(float _x, float _y)
	{
		XMStoreFloat2(&f, XMVectorSet(_x, _y, 0, 0));
	}

	inline XMVECTOR GetXMVECTOR() const
	{
		return XMLoadFloat2(&f);
	}

	inline _VECTOR2& operator =(const _VECTOR2& _v)
	{
		XMStoreFloat2(&f, XMLoadFloat2(&_v.f));
		return *this;
	}

	inline _VECTOR2 operator +(const _VECTOR2& _v) const
	{
		return XMVectorAdd(XMLoadFloat2(&f), XMLoadFloat2(&_v.f));
	}

	inline _VECTOR2 operator -(const _VECTOR2& _v) const
	{
		return XMVectorSubtract(XMLoadFloat2(&f), XMLoadFloat2(&_v.f));
	}

	inline _VECTOR2 operator *(const float _v) const
	{
		return XMVectorMultiply(XMLoadFloat2(&f), XMVectorSet(_v, _v, _v, _v));
	}

	inline _VECTOR2 operator /(const float _v) const
	{
		return XMVectorDivide(XMLoadFloat2(&f), XMVectorSet(_v, _v, _v, _v));
	}

	inline _VECTOR2 operator -() const
	{
		return _VECTOR2(-x, -y);
	}

	inline void operator +=(const _VECTOR2& _v)
	{
		XMStoreFloat2(&f, XMVectorAdd(XMLoadFloat2(&f), XMLoadFloat2(&_v.f)));
	}

	inline void operator -=(const _VECTOR2& _v)
	{
		XMStoreFloat2(&f, XMVectorSubtract(XMLoadFloat2(&f), XMLoadFloat2(&_v.f)));
	}

	inline void operator *=(const float _v)
	{
		XMStoreFloat2(&f, XMVectorMultiply(XMLoadFloat2(&f), XMVectorSet(_v, _v, _v, _v)));
	}

	inline void operator /=(const float _v)
	{
		XMStoreFloat2(&f, XMVectorDivide(XMLoadFloat2(&f), XMVectorSet(_v, _v, _v, _v)));
	}

	inline _VECTOR2 GetNormalized() const
	{
		return XMVector2Normalize(XMLoadFloat2(&f));
	}
	
	inline float GetLength() const
	{
		float length;
		XMStoreFloat(&length, XMVector2Length(XMLoadFloat2(&f)));
		return length;
	}

	inline float Dot(const _VECTOR2& _v) const
	{
		return _v.x * x + _v.y * y;
	}

}Vector2;

typedef struct _VECTOR3
{
	union
	{
		XMFLOAT3 f;
		struct
		{
			float x, y, z;
		};
		struct
		{
			float pitch, yaw, roll;
		};
	};

	inline _VECTOR3(XMVECTOR _v)
	{
		XMStoreFloat3(&f, _v);
	}

	inline _VECTOR3(float _x, float _y, float _z)
	{
		XMStoreFloat3(&f, XMVectorSet(_x, _y, _z, 0));
	}

	template <typename T>
	inline _VECTOR3(const T& _v)
	{
		XMStoreFloat3(&f, _v.GetXMVECTOR());
	}

	inline _VECTOR3()
	{
		XMStoreFloat3(&f, XMVectorSet(0, 0, 0, 0));
	}

	inline void Set(float _x, float _y, float _z)
	{
		XMStoreFloat3(&f, XMVectorSet(_x, _y, _z, 0));
	}

	inline XMVECTOR GetXMVECTOR() const
	{
		return XMLoadFloat3(&f);
	}

	inline _VECTOR3& operator =(const _VECTOR3& _v)
	{
		XMStoreFloat3(&f, XMLoadFloat3(&_v.f));
		return *this;
	}

	inline _VECTOR3 operator +(const _VECTOR3& _v) const
	{
		return XMVectorAdd(XMLoadFloat3(&f), XMLoadFloat3(&_v.f));
	}

	inline _VECTOR3 operator -(const _VECTOR3& _v) const
	{
		return XMVectorSubtract(XMLoadFloat3(&f), XMLoadFloat3(&_v.f));
	}

	inline _VECTOR3 operator *(const float _v) const
	{
		return XMVectorMultiply(XMLoadFloat3(&f), XMVectorSet(_v, _v, _v, _v));
	}

	inline _VECTOR3 operator /(const float _v) const
	{
		return XMVectorDivide(XMLoadFloat3(&f), XMVectorSet(_v, _v, _v, _v));
	}

	inline _VECTOR3 operator -() const
	{
		return _VECTOR3(-x, -y, -z);
	}

	inline void operator +=(const _VECTOR3& _v)
	{
		XMStoreFloat3(&f, XMVectorAdd(XMLoadFloat3(&f), XMLoadFloat3(&_v.f)));
	}

	inline void operator -=(const _VECTOR3& _v)
	{
		XMStoreFloat3(&f, XMVectorSubtract(XMLoadFloat3(&f), XMLoadFloat3(&_v.f)));
	}

	inline void operator *=(const float _v)
	{
		XMStoreFloat3(&f, XMVectorMultiply(XMLoadFloat3(&f), XMVectorSet(_v, _v, _v, _v)));
	}

	inline void operator /=(const float _v)
	{
		XMStoreFloat3(&f, XMVectorDivide(XMLoadFloat3(&f), XMVectorSet(_v, _v, _v, _v)));
	}

	inline _VECTOR3 GetNormalized() const
	{
		return XMVector3Normalize(XMLoadFloat3(&f));
	}
	
	inline float GetLength() const
	{
		float length;
		XMStoreFloat(&length, XMVector3Length(XMLoadFloat3(&f)));
		return length;
	}

	inline float Dot(const _VECTOR3& _v) const
	{
		return _v.x * x + _v.y * y + _v.z * z;
	}

	inline _VECTOR3 Cross(const _VECTOR3& _v) const
	{
		return XMVector3Cross(XMLoadFloat3(&f), XMLoadFloat3(&_v.f));
	}

}Vector3, Euler;

typedef struct _VECTOR4
{
	union
	{
		XMFLOAT4 f;
		struct
		{
			float x, y, z, w;
		};
		struct
		{
			float r, g, b, a;
		};
	};

	inline _VECTOR4(XMVECTOR _v)
	{
		XMStoreFloat4(&f, _v);
	}

	inline _VECTOR4(float _x, float _y, float _z, float _w)
	{
		XMStoreFloat4(&f, XMVectorSet(_x, _y, _z, _w));
	}
	
	template <typename T>
	inline _VECTOR4(const T& _v)
	{
		XMStoreFloat4(&f, _v.GetXMVECTOR());
	}

	inline _VECTOR4()
	{
		XMStoreFloat4(&f, XMVectorSet(0, 0, 0, 1));
	}

	inline void Set(float _x, float _y, float _z, float _w)
	{
		XMStoreFloat4(&f, XMVectorSet(_x, _y, _z, _w));
	}

	inline XMVECTOR GetXMVECTOR() const
	{
		return XMLoadFloat4(&f);
	}

	inline _VECTOR4& operator =(const _VECTOR4& _v)
	{
		XMStoreFloat4(&f, XMLoadFloat4(&_v.f));
		return *this;
	}

	inline _VECTOR4 operator +(const _VECTOR4& _v) const
	{
		return XMVectorAdd(XMLoadFloat4(&f), XMLoadFloat4(&_v.f));
	}

	inline _VECTOR4 operator -(const _VECTOR4& _v) const
	{
		return XMVectorSubtract(XMLoadFloat4(&f), XMLoadFloat4(&_v.f));
	}

	inline _VECTOR4 operator *(const float _v) const
	{
		return XMVectorMultiply(XMLoadFloat4(&f), XMVectorSet(_v, _v, _v, _v));
	}

	inline _VECTOR4 operator /(const float _v) const
	{
		return XMVectorDivide(XMLoadFloat4(&f), XMVectorSet(_v, _v, _v, _v));
	}

	inline _VECTOR4 operator -() const
	{
		return _VECTOR4(-x, -y, -z, -w);
	}

	inline void operator +=(const _VECTOR4& _v)
	{
		XMStoreFloat4(&f, XMVectorAdd(XMLoadFloat4(&f), XMLoadFloat4(&_v.f)));
	}

	inline void operator -=(const _VECTOR4& _v)
	{
		XMStoreFloat4(&f, XMVectorSubtract(XMLoadFloat4(&f), XMLoadFloat4(&_v.f)));
	}

	inline void operator *=(const float _v)
	{
		XMStoreFloat4(&f, XMVectorMultiply(XMLoadFloat4(&f), XMVectorSet(_v, _v, _v, _v)));
	}

	inline void operator /=(const float _v)
	{
		XMStoreFloat4(&f, XMVectorDivide(XMLoadFloat4(&f), XMVectorSet(_v, _v, _v, _v)));
	}

	inline _VECTOR4 GetNormalized() const
	{
		return XMVector4Normalize(XMLoadFloat4(&f));
	}
	
	inline float GetLength() const
	{
		float length;
		XMStoreFloat(&length, XMVector4Length(XMLoadFloat4(&f)));
		return length;
	}

	inline float Dot(const _VECTOR4& _v) const
	{
		return _v.x * x + _v.y * y + _v.z * z + _v.w * w;
	}

}Vector4, Color, Quaternion;

typedef struct _RectangleF
{
	float x,y,width,height;
}RectangleF;

struct VideoDesc
{
	UINT bufferWidth;

	UINT bufferHeight;

	bool msaaEnabled;

	UINT multiSampleCount;
	
	bool windowed;
	
	UINT refreshRate;

	bool vSync;

	bool fixedAspect;

	bool altEnter;
};

struct PhysXDesc
{
	PxTolerancesScale tolerancesScale;
	
	PxCpuDispatcher* cpuDispatcher;
};

struct InputDesc
{
	DWORD keyboardLevel;
	
	DWORD mouseLevel;
};

struct EngineAttributes
{
	bool autoClear = true;
	
	Color backColor = { 0,0,0,1 };
	
	bool lockDeltaTime = false;
};

struct EngineDescription
{
	VideoDesc videoDesc;

	PhysXDesc physxDesc;

	InputDesc inputDesc;

	HINSTANCE hInstance;

	HWND outputWindow;

	LPCSTR windowName;

	LPCSTR className;

	EngineErrorCallBack* errorCallBack = 0;

	EngineAttributes attributes;
};

struct RenderTargetDesc
{
	UINT width;

	UINT height;

	bool support2D;

	bool msaaEnabled;

	UINT multiSampleCount;

	bool hasDepth;
};

struct PxConstant
{
	PxFoundation* gFoundation;

	PxPhysics* gPhysics;

	PxCooking* gCooking;
};

struct SceneDesc
{
	PxSceneDesc pxSceneDesc;
};

struct Light
{
	Color ambient;

	Color diffuse;

	Color specular;

	Vector3 position;

	float range;

	Vector3 direction;

	float spot;

	LIGHTTYPE type;

	float att0;

	float att1;

	float att2;

	Light() { ZeroMemory(this, sizeof(Light)); }

private:

	float pad;
};

struct Material
{
	Color ambient;

	Color diffuse;

	Color specular;

	float power;

	Material() { ZeroMemory(this, sizeof(Material)); }

private:

	Vector3 pad;
};

struct USize
{
	UINT width;
	
	UINT height;
};

struct DynamicAudioDesc
{
	UINT playBegin;

	UINT playEnd;

	UINT loopCount;

	UINT loopBegin;

	UINT loopEnd;
};