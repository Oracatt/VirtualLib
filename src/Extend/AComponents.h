#pragma once
#include "../EasyDevelop/ActorCom.h"
#include "../Interface/IPhysicsManager.h"
#include "../Interface/IRenderable.h"
#include "../Interface/IRenderer.h"
#include "../Interface/IPhysicsActor.h"
#include "../Interface/ITextFormat.h"

class Transform :public AComponent
{
public:

	Vector3 location;

	Vector3 scale;

	Quaternion rotation;

	Transform();

	void Translate(const Vector3& displacement);

	void Rotate(const Euler& euler, bool selfCoord);

	Euler GetEulerAngles();

	void SetEulerAngles(const Euler& euler);

	XMMATRIX GetWorldMatrix();
};

class Transform2D :public AComponent
{
public:

	Vector2 location;

	Vector2 scale;

	float rotation;

	Transform2D();

	void Translate(const Vector2& displacement);

	void Rotate(float radians);
};


class RigidBody :public AComponent, public IPhysicsActor
{
public:

	RigidBody(bool dynamic = true);

	float GetMass();

	void SetMass(float mass);

	Vector3 GetVelocity();

	void SetVelocity(const Vector3& velocity);

	Vector3 GetAngularVelocity();

	void SetAngularVelocity(const Vector3& velocity);

	float GetVelocityDrag();

	void SetVelocityDrag(float drag);

	float GetAngularVelocityDrag();

	void SetAngularVelocityDrag(float drag);

	bool GetGravity();

	void SetGravity(bool enable);

	bool GetKinematic();

	void SetKinematic(bool enable);

	bool GetCCD();

	void SetCCD(bool enable);

	PxRigidDynamicLockFlags GetLockFlag();

	void SetLockFlag(PxRigidDynamicLockFlags flag);

	HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) override;

	ULONG _stdcall AddRef() override;

	ULONG _stdcall Release() override;

	/*以下这些必须在OnAwake()被调用后才可使用*/

	void AddForce(const Vector3& force, PxForceMode::Enum mode = PxForceMode::eFORCE);

	void AddTorque(const Vector3& torque, PxForceMode::Enum mode = PxForceMode::eFORCE);

	void ClearForce(PxForceMode::Enum mode = PxForceMode::eFORCE);

	void ClearTorque(PxForceMode::Enum mode = PxForceMode::eFORCE);

	PxRigidActor* GetPxTarget();

protected:

	float mass;

	PxVec3 velocity;

	PxVec3 angVelocity;

	float velocityDrag;

	float angVelocityDrag;

	bool gravityEnable;

	bool kinematicEnable;

	bool ccdEnable;

	PxRigidActor* pxRigid;

	PxRigidDynamic* pxRigidDynamic;

	bool dynamic;

	PxRigidDynamicLockFlags lockflag;

	PxConstant* pxVars;

	PxScene* pxScene;

private:

	ISRESULT OnPreAwake(IAContainer* actor) override;

	void OnAwake() override;

	void OnRemoved() override;

	void Synchronize(bool isPxToScene) override;

};

class Collider :public AComponent
{
public:

	ComPtr<RigidBody> rigidbody;

	Collider();

	PxMaterial* GetMaterial();

	void SetMaterial(PxMaterial* material);

	bool GetTrigger();

	void SetTrigger(bool enable);

	Vector3 GetLocalPosition();

	void SetLocalPosition(const Vector3& loc);

	Quaternion GetLocalRotation();

	void SetLocalRotation(const Quaternion& rot);

	/*以下这些必须在OnAwake()被调用后才可使用*/
	PxShape* GetPxTarget();

	virtual void OnColliderEnter(Collider* collider);

	virtual void OnColliderStay(Collider* collider);

	virtual void OnColliderExit(Collider* collider);

	virtual void OnTriggerEnter(Collider* collider);

	virtual void OnTriggerExit(Collider* collider);

protected:

	PxShape* pxShape;

	PxMaterial* material;

	bool isTrigger;

	Vector3 locpos;

	Quaternion locrot;

	void OnRemoved() override;

};

class SphereCollider :public Collider
{
public:

	SphereCollider();

	void SetRadius(float r);

	float GetRadius();

private:

	float radius;

	void OnAwake() override;

};

class BoxCollider :public Collider
{
public:

	BoxCollider();

	Vector3 GetHalfSize();

	void SetHalfSize(Vector3 halfsize);

private:

	PxVec3 halfsize;

	void OnAwake() override;
};

class CapsuleCollider :public Collider
{
public:

	CapsuleCollider();

	float GetRadius();

	void SetRadius(float radius);

	float GetHalfHeight();

	void SetHalfHeight(float halfheight);

private:

	float radius;

	float halfheight;

private:

	void OnAwake() override;

};

class TriangleMeshCollider :public Collider
{
public:

	TriangleMeshCollider(PxTriangleMeshDesc des);

	TriangleMeshCollider(PxTriangleMesh* mesh);

	PxTriangleMesh* GetMesh();

protected:

	PxTriangleMesh* meshpt;

	PxTriangleMeshDesc meshDesc;

	bool refer;

private:

	void OnAwake() override;

	void OnRemoved() override;

};

class ConvexMeshCollider :public Collider
{
public:

	ConvexMeshCollider(PxConvexMeshDesc des);

	ConvexMeshCollider(PxConvexMesh* mesh);

	PxConvexMesh* GetMesh();

protected:

	PxConvexMesh* meshpt;

	PxConvexMeshDesc meshDesc;

	bool refer;

private:

	void OnAwake() override;

	void OnRemoved() override;

};

class Plane :public AComponent
{
public:

	Plane(float x, float y, float z, float n);

	PxRigidStatic* GetPxTarget();

	PxMaterial* GetMaterial();

	void SetMaterial(PxMaterial* material);

	void SetTransform(float x, float y, float z, float n);

protected:

	Vector4 pl;

	PxRigidStatic* pxRigid;

	PxMaterial* material;

	void OnAwake() override;

	void OnRemoved() override;

};

class RenderCom :public AComponent, public IRenderable
{
public:

	HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) override;

	ULONG _stdcall AddRef() override;

	ULONG _stdcall Release() override;

	void OnFetchData() override;

	void OnRender() override;

	void OnRender2D() override;

};

class SpriteRender :public RenderCom
{
public:

	SpriteRender();

	void SetTexture(ITexture* tex);

	ITexture* GetTexture();

	XMMATRIX GetTextureTransform();

	void SetTextureTransform(const XMMATRIX& matrix);

	XMMATRIX GetLocalTransform();

	void SetLocalTransform(const XMMATRIX& matrix);

	Material GetMaterial();

	void SetMaterial(const Material& material);

	void SetSourceRect(const RectangleF& rect);

protected:

	Material material;

	XMMATRIX texTransform;

	XMMATRIX localTransform;

	ComPtr<ITexture> texture;

	ComPtr<IRenderer> renderer;

	ComPtr<ID3D11Device> pDevice;

	ComPtr<ID3D11DeviceContext> pContext;

	ComPtr<IDXGISwapChain> pSwapChain;

protected:

	ComPtr<ITexture> mTexture;

	Material mMaterial;

	XMMATRIX transform;

	XMMATRIX mTexTransform;

	XMMATRIX mlocalTransform;

	bool needUpdate;

	void OnAwake() override;

	void OnFetchData() override;

	void OnRender() override;
};

class TextRender2D :public RenderCom
{
public:

	TextRender2D(bool renderOnCanvas = true);

	ITextFormat* GetTextFormat();

	void SetTextFormat(ITextFormat* pFormat);

	wstring GetText();

	void SetText(const wstring& str);

	bool GetOutlineEnabled();

	void SetOutlineEnabled(bool enable);

	bool GetBodyEnabled();

	void SetBodyEnabled(bool enable);

	float GetOutlineWidth();

	void SetOutlineWidth(float width);

	ID2D1Brush* GetBodyBrush();

	void SetBodyBrush(ID2D1Brush* brush);

	ID2D1Brush* GetOutlineBrush();

	void SetOutlineBrush(ID2D1Brush* brush);

	D2D1_RECT_F GetRect();

	void SetRect(const D2D1_RECT_F& rect);

protected:

	UINT align;

	bool outlineEnabled;

	bool bodyEnabled;

	float outlineWidth;

	wstring text;

	D2D1_RECT_F rect;

	ComPtr<IRenderer> renderer;

	ComPtr<IDWriteFactory> pDWriteFactory;

	ComPtr<IDWriteTextLayout> pDWriteTextLayout;

	ComPtr<ITextFormat> pFormat;

	ComPtr<ID2D1Brush> pBodyBrush;

	ComPtr<ID2D1Brush> pOutlineBrush;

	bool copyText;

	bool needUpdate;

protected:

	Vector2 location;

	Vector2 scale;

	float rotation;

	bool mOutlineEnabled;

	bool mBodyEnabled;

	float mOutlineWidth;

	wstring mText;

	D2D1_RECT_F mRect;

	ComPtr<ITextFormat> mFormat;

	ComPtr<ID2D1Brush> mBodyBrush;

	ComPtr<ID2D1Brush> mOutlineBrush;

	ComPtr<ID3D11Multithread> mProtector;

	bool renderOnCanvas;

	void OnAwake() override;

	void OnFetchData() override;

	void OnRender2D() override;

	void OnRender() override;

};