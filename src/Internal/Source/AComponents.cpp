#include "../../Extend/AComponents.h"
#include "../../Extend/LinearMath.h"
#include "../../EasyDevelop/GameObject.h"
#include "../../Interface/IVirtualEngine.h"
#include "../../Interface/IPhysicsScene.h"
#include "../Include/Renderer.h"

Transform::Transform() :
	location(0, 0, 0), scale(1, 1, 1), rotation(0, 0, 0, 1) {}

void Transform::Translate(const Vector3& displacement)
{
	location += displacement;
}

void Transform::Rotate(const Euler& euler, bool selfCoord)
{
	if (selfCoord)
	{
		rotation = QuaternionMultiply(QuaternionRotateRollPitchYaw(euler.x, euler.y, euler.z), rotation);
	}
	else
	{
		rotation = QuaternionMultiply(rotation, QuaternionRotateRollPitchYaw(euler.x, euler.y, euler.z));
	}
}

Euler Transform::GetEulerAngles()
{
	return EulerFromQuaternion(rotation);
}

void Transform::SetEulerAngles(const Euler& euler)
{
	rotation = QuaternionRotateRollPitchYaw(euler.pitch, euler.yaw, euler.roll);
}

XMMATRIX Transform::GetWorldMatrix()
{
	XMMATRIX matWorld = XMMatrixScaling(scale.x, scale.y, scale.z);
	matWorld *= XMMatrixRotationQuaternion(XMLoadFloat4(&rotation.f));
	matWorld *= XMMatrixTranslation(location.x, location.y, location.z);
	return matWorld;
}

Transform2D::Transform2D() :
	location(0, 0), scale(1, 1), rotation(0) {}

void Transform2D::Translate(const Vector2& displacement)
{
	location += displacement;
}

void Transform2D::Rotate(float radians)
{
	rotation += radians;
}

RigidBody::RigidBody(bool d) :
	dynamic(d), mass(1), velocity(0, 0, 0), angVelocity(0, 0, 0), velocityDrag(0), angVelocityDrag(0), gravityEnable(true),
	kinematicEnable(false), ccdEnable(false), pxRigid(0), pxRigidDynamic(0), lockflag(0), pxVars(0), pxScene(0) {}

ISRESULT RigidBody::OnPreAwake(IAContainer* actor)
{
	if (actor->GetComponent<RigidBody>())
	{
		IActor* pActor;
		actor->QueryInterface(IID_IActor, (void**)&pActor);
		pActor->GetScene()->GetEngine()->ReportError("An actor can only have one RigidBody component!");
		return ISR_SUPERCEDE;
	}
	return ISR_IGNORE;
}

void RigidBody::OnAwake()
{
	ComPtr<IPhysicsManager> pxManager;
	ComPtr<IVirtualEngine>(gameObject->GetScene()->GetEngine()).As(&pxManager);
	pxVars = pxManager->GetConstant();
	ComPtr<IPhysicsScene> pScene;
	ComPtr<IScene>(gameObject->GetScene()).As(&pScene);
	pxScene = pScene->GetPxScene();
	PxTransform localTm(*((PxVec3*)&gameObject->transform->location), *((PxQuat*)&gameObject->transform->rotation));
	if (dynamic)
	{
		pxRigid = pxVars->gPhysics->createRigidDynamic(localTm);
		pxRigidDynamic = (PxRigidDynamic*)pxRigid;
		pxRigidDynamic->setMass(mass);
		pxRigidDynamic->setLinearVelocity(velocity);
		pxRigidDynamic->setAngularVelocity(angVelocity);
		pxRigidDynamic->setLinearDamping(velocityDrag);
		pxRigidDynamic->setAngularDamping(angVelocityDrag);
		pxRigidDynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !gravityEnable);
		pxRigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, kinematicEnable);
		pxRigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, ccdEnable);
		pxRigidDynamic->setRigidDynamicLockFlags(lockflag);
	}
	else
	{
		pxRigid = pxVars->gPhysics->createRigidStatic(localTm);
	}
	vector<ComPtr<Collider>> colliders;
	gameObject->GetComponents<Collider>(&colliders);
	for (vector<ComPtr<Collider>>::iterator it = colliders.begin(); it != colliders.end(); it++)
	{
		if (!(*it)->rigidbody && (*it)->GetPxTarget())
		{
			(*it)->rigidbody = this;
			pxRigid->attachShape(*(*it)->GetPxTarget());
		}
	}
	pxScene->addActor(*pxRigid);
}

void RigidBody::Synchronize(bool isPxToScene)
{
	if (isPxToScene)
	{
		if (dynamic)
		{
			PxTransform tm = pxRigid->getGlobalPose();
			gameObject->transform->location = *((Vector3*)&tm.p);
			gameObject->transform->rotation = *((Quaternion*)(&tm.q));
			velocity = pxRigidDynamic->getLinearVelocity();
			angVelocity = pxRigidDynamic->getAngularVelocity();
		}
	}
	else
	{
		PxTransform tm(*((PxVec3*)&gameObject->transform->location), *((PxQuat*)(&gameObject->transform->rotation)));
		pxRigid->setGlobalPose(tm);
		if (kinematicEnable && pxRigidDynamic)
			pxRigidDynamic->setKinematicTarget(tm);
	}
}

void RigidBody::OnRemoved()
{
	if (pxRigid)
	{
		pxScene->removeActor(*pxRigid);
		pxRigid->release();
		pxRigid = 0;
		pxRigidDynamic = 0;
	}
}

float RigidBody::GetMass()
{
	return mass;
}

void RigidBody::SetMass(float m)
{
	mass = m;
	if (pxRigidDynamic)
		pxRigidDynamic->setMass(mass);
}

Vector3 RigidBody::GetVelocity()
{
	return *((Vector3*)&velocity);
}

void RigidBody::SetVelocity(const Vector3& vec)
{
	velocity = *((PxVec3*)&vec);
	if (pxRigidDynamic)
		pxRigidDynamic->setLinearVelocity(velocity);
}

Vector3 RigidBody::GetAngularVelocity()
{
	return *((Vector3*)&angVelocity);
}

void RigidBody::SetAngularVelocity(const Vector3& vec)
{
	angVelocity = *((PxVec3*)&vec);
	if (pxRigidDynamic)
		pxRigidDynamic->setAngularVelocity(angVelocity);
}

float RigidBody::GetVelocityDrag()
{
	return velocityDrag;
}

void RigidBody::SetVelocityDrag(float drag)
{
	velocityDrag = drag;
	if (pxRigidDynamic)
		pxRigidDynamic->setLinearDamping(velocityDrag);
}

float RigidBody::GetAngularVelocityDrag()
{
	return angVelocityDrag;
}

void RigidBody::SetAngularVelocityDrag(float drag)
{
	angVelocityDrag = drag;
	if (pxRigidDynamic)
		pxRigidDynamic->setAngularDamping(angVelocityDrag);
}

PxRigidActor* RigidBody::GetPxTarget()
{
	return pxRigid;
}

void RigidBody::AddForce(const Vector3& force, PxForceMode::Enum mode)
{
	pxRigidDynamic->addForce(*((PxVec3*)&force), mode);
}

void RigidBody::AddTorque(const Vector3& torque, PxForceMode::Enum mode)
{
	pxRigidDynamic->addTorque(*((PxVec3*)&torque), mode);
}

void RigidBody::ClearForce(PxForceMode::Enum mode)
{
	pxRigidDynamic->clearForce(mode);
}

void RigidBody::ClearTorque(PxForceMode::Enum mode)
{
	pxRigidDynamic->clearTorque(mode);
}

bool RigidBody::GetGravity()
{
	return gravityEnable;
}

void RigidBody::SetGravity(bool enable)
{
	gravityEnable = enable;
	if (pxRigid)
		pxRigid->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !gravityEnable);
}

bool RigidBody::GetKinematic()
{
	return kinematicEnable;
}

void RigidBody::SetKinematic(bool enable)
{
	kinematicEnable = enable;
	if (pxRigidDynamic)
		pxRigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, kinematicEnable);
}

bool RigidBody::GetCCD()
{
	return ccdEnable;
}

void RigidBody::SetCCD(bool enable)
{
	ccdEnable = enable;
	if (pxRigidDynamic)
		pxRigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, ccdEnable);
}

PxRigidDynamicLockFlags RigidBody::GetLockFlag()
{
	return lockflag;
}

void RigidBody::SetLockFlag(PxRigidDynamicLockFlags flag)
{
	lockflag = flag;
	if (pxRigidDynamic)
		pxRigidDynamic->setRigidDynamicLockFlags(lockflag);
}

HRESULT _stdcall RigidBody::QueryInterface(const IID& iid, void** ppvObject)
{
	if (iid == IID_IAComponent)
	{
		*ppvObject = static_cast<IAComponent*>(this);
	}
	else if (iid == IID_IPhysicsActor)
	{
		*ppvObject = static_cast<IPhysicsActor*>(this);
	}
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	static_cast<IUnknown*>(*ppvObject)->AddRef();
	return S_OK;
}

ULONG _stdcall RigidBody::AddRef()
{
	return AComponent::AddRef();
}

ULONG _stdcall RigidBody::Release()
{
	return AComponent::Release();
}

Collider::Collider() :
	pxShape(0), material(0), isTrigger(false), locpos(0, 0, 0), locrot(0, 0, 0, 1) {}

void Collider::OnRemoved()
{
	if (pxShape)
	{
		pxShape->userData = 0;
		if (rigidbody && rigidbody->IsValid())
		{
			rigidbody->GetPxTarget()->detachShape(*pxShape);
		}
		else
		{
			pxShape->release();
		}
	}
	pxShape = 0;
	rigidbody.Reset();
}

bool Collider::GetTrigger()
{
	return isTrigger;
}

void Collider::SetTrigger(bool enable)
{
	isTrigger = enable;
	if (pxShape)
	{
		pxShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !enable);
		pxShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, enable);
	}
}

PxMaterial* Collider::GetMaterial()
{
	return material;
}

void Collider::SetMaterial(PxMaterial* mat)
{
	material = mat;
	if (pxShape)
		pxShape->setMaterials(&material, 1);
}

Vector3 Collider::GetLocalPosition()
{
	return locpos;
}

void Collider::SetLocalPosition(const Vector3& loc)
{
	locpos = loc;
	if (pxShape)
		pxShape->setLocalPose(PxTransform(*((PxVec3*)&locpos), *((PxQuat*)&locrot)));
}

Quaternion Collider::GetLocalRotation()
{
	return locrot;
}

void Collider::SetLocalRotation(const Quaternion& rot)
{
	locrot = rot;
	if (pxShape)
		pxShape->setLocalPose(PxTransform(*((PxVec3*)&locpos), *((PxQuat*)&locrot)));
}

PxShape* Collider::GetPxTarget()
{
	return pxShape;
}

void Collider::OnColliderEnter(Collider* collider) {};
void Collider::OnColliderStay(Collider* collider) {};
void Collider::OnColliderExit(Collider* collider) {};
void Collider::OnTriggerEnter(Collider* collider) {};
void Collider::OnTriggerExit(Collider* collider) {};

SphereCollider::SphereCollider() :
	radius(1) {}

void SphereCollider::OnAwake()
{
	ComPtr<IPhysicsManager> pxManager;
	ComPtr<IVirtualEngine>(gameObject->GetScene()->GetEngine()).As(&pxManager);
	PxConstant* pxVars = pxManager->GetConstant();
	material = pxManager->GetDefaultMaterial();
	pxShape = pxVars->gPhysics->createShape(PxSphereGeometry(radius), *material);
	pxShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
	pxShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
	pxShape->setLocalPose(PxTransform(*((PxVec3*)&locpos), *((PxQuat*)&locrot)));
	PxFilterData data;
	data.word0 = 1;
	pxShape->setSimulationFilterData(data);
	pxShape->userData = (Collider*)this;
	ComPtr<RigidBody> rigid = 0;
	if (!rigidbody && (rigid = gameObject->GetComponent<RigidBody>()))
	{
		if (rigid->IsValid() && rigid->GetPxTarget())
		{
			rigidbody = rigid;
			rigid->GetPxTarget()->attachShape(*pxShape);
		}
	}
}

void SphereCollider::SetRadius(float r)
{
	radius = r;
	if (pxShape)
		pxShape->setGeometry(PxSphereGeometry(radius));
}

float SphereCollider::GetRadius()
{
	return radius;
}

BoxCollider::BoxCollider() :
	halfsize(1, 1, 1) {}

void BoxCollider::OnAwake()
{
	ComPtr<IPhysicsManager> pxManager;
	ComPtr<IVirtualEngine>(gameObject->GetScene()->GetEngine()).As(&pxManager);
	PxConstant* pxVars = pxManager->GetConstant();
	material = pxManager->GetDefaultMaterial();
	pxShape = pxVars->gPhysics->createShape(PxBoxGeometry(halfsize.x, halfsize.y, halfsize.z), *material);
	pxShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
	pxShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
	pxShape->setLocalPose(PxTransform(*((PxVec3*)&locpos), *((PxQuat*)&locrot)));
	PxFilterData data;
	data.word0 = 1;
	pxShape->setSimulationFilterData(data);
	pxShape->userData = (Collider*)this;
	ComPtr<RigidBody> rigid = 0;
	if (!rigidbody && (rigid = gameObject->GetComponent<RigidBody>()))
	{
		if (rigid->IsValid() && rigid->GetPxTarget())
		{
			rigidbody = rigid;
			rigid->GetPxTarget()->attachShape(*pxShape);
		}
	}
}

Vector3 BoxCollider::GetHalfSize()
{
	return *((Vector3*)&halfsize);
}

void BoxCollider::SetHalfSize(Vector3 s)
{
	halfsize = *((PxVec3*)&s);
	if (pxShape)
		pxShape->setGeometry(PxBoxGeometry(halfsize));
}

CapsuleCollider::CapsuleCollider() :
	radius(1), halfheight(0) {}

void CapsuleCollider::OnAwake()
{
	ComPtr<IPhysicsManager> pxManager;
	ComPtr<IVirtualEngine>(gameObject->GetScene()->GetEngine()).As(&pxManager);
	PxConstant* pxVars = pxManager->GetConstant();
	material = pxManager->GetDefaultMaterial();
	pxShape = pxVars->gPhysics->createShape(PxCapsuleGeometry(radius, halfheight), *material);
	pxShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
	pxShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
	pxShape->setLocalPose(PxTransform(*((PxVec3*)&locpos), *((PxQuat*)&locrot)));
	PxFilterData data;
	data.word0 = 1;
	pxShape->setSimulationFilterData(data);
	pxShape->userData = (Collider*)this;
	ComPtr<RigidBody> rigid = 0;
	if (!rigidbody && (rigid = gameObject->GetComponent<RigidBody>()))
	{
		if (rigid->IsValid() && rigid->GetPxTarget())
		{
			rigidbody = rigid;
			rigid->GetPxTarget()->attachShape(*pxShape);
		}
	}
}

float CapsuleCollider::GetRadius()
{
	return radius;
}

void CapsuleCollider::SetRadius(float r)
{
	radius = r;
	if (pxShape)
		pxShape->setGeometry(PxCapsuleGeometry(radius, halfheight));
}

float CapsuleCollider::GetHalfHeight()
{
	return halfheight;
}

void CapsuleCollider::SetHalfHeight(float h)
{
	halfheight = h;
	if (pxShape)
		pxShape->setGeometry(PxCapsuleGeometry(radius, halfheight));
}


TriangleMeshCollider::TriangleMeshCollider(PxTriangleMeshDesc des) :
	meshDesc(des), meshpt(0), refer(false) {}

TriangleMeshCollider::TriangleMeshCollider(PxTriangleMesh* mesh) :
	meshDesc(), meshpt(mesh), refer(true) {}

void TriangleMeshCollider::OnAwake()
{
	ComPtr<IPhysicsManager> pxManager;
	ComPtr<IVirtualEngine>(gameObject->GetScene()->GetEngine()).As(&pxManager);
	PxConstant* pxVars = pxManager->GetConstant();
	material = pxManager->GetDefaultMaterial();
	if (!refer)
	{
		meshpt = pxVars->gCooking->createTriangleMesh(meshDesc, pxVars->gPhysics->getPhysicsInsertionCallback());
	}
	pxShape = pxVars->gPhysics->createShape(PxTriangleMeshGeometry(meshpt), *material);
	pxShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
	pxShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
	pxShape->setLocalPose(PxTransform(*((PxVec3*)&locpos), *((PxQuat*)&locrot)));
	PxFilterData data;
	data.word0 = 1;
	pxShape->setSimulationFilterData(data);
	pxShape->userData = (Collider*)this;
	ComPtr<RigidBody> rigid = 0;
	if (!rigidbody && (rigid = gameObject->GetComponent<RigidBody>()))
	{
		if (rigid->IsValid() && rigid->GetPxTarget())
		{
			rigidbody = rigid;
			rigid->GetPxTarget()->attachShape(*pxShape);
		}
	}
}

void TriangleMeshCollider::OnRemoved()
{
	__super::OnRemoved();
	if (meshpt)
	{
		if (!refer)
			meshpt->release();
		meshpt = 0;
	}
}

PxTriangleMesh* TriangleMeshCollider::GetMesh()
{
	return meshpt;
}

ConvexMeshCollider::ConvexMeshCollider(PxConvexMeshDesc des) :
	meshDesc(des), meshpt(0), refer(false) {}

ConvexMeshCollider::ConvexMeshCollider(PxConvexMesh* mesh) :
	meshDesc(), meshpt(mesh), refer(true) {}

void ConvexMeshCollider::OnAwake()
{
	ComPtr<IPhysicsManager> pxManager;
	ComPtr<IVirtualEngine>(gameObject->GetScene()->GetEngine()).As(&pxManager);
	PxConstant* pxVars = pxManager->GetConstant();
	material = pxManager->GetDefaultMaterial();
	if (!refer)
	{
		meshpt = pxVars->gCooking->createConvexMesh(meshDesc, pxVars->gPhysics->getPhysicsInsertionCallback());
	}
	pxShape = pxVars->gPhysics->createShape(PxConvexMeshGeometry(meshpt), *material);
	pxShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
	pxShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
	pxShape->setLocalPose(PxTransform(*((PxVec3*)&locpos), *((PxQuat*)&locrot)));
	PxFilterData data;
	data.word0 = 1;
	pxShape->setSimulationFilterData(data);
	pxShape->userData = (Collider*)this;
	ComPtr<RigidBody> rigid = 0;
	if (!rigidbody && (rigid = gameObject->GetComponent<RigidBody>()))
	{
		if (rigid->IsValid() && rigid->GetPxTarget())
		{
			rigidbody = rigid;
			rigid->GetPxTarget()->attachShape(*pxShape);
		}
	}
}

void ConvexMeshCollider::OnRemoved()
{
	__super::OnRemoved();
	if (meshpt)
	{
		if (!refer)
			meshpt->release();
		meshpt = 0;
	}
}

PxConvexMesh* ConvexMeshCollider::GetMesh()
{
	return meshpt;
}

Plane::Plane(float x, float y, float z, float n) :
	pl(x, y, z, n), pxRigid(0), material(0) {}

void Plane::OnAwake()
{
	ComPtr<IPhysicsManager> pxManager;
	ComPtr<IVirtualEngine>(gameObject->GetScene()->GetEngine()).As(&pxManager);
	PxConstant* pxVars = pxManager->GetConstant();
	material = pxManager->GetDefaultMaterial();
	pxRigid = PxCreatePlane(*pxVars->gPhysics, PxPlane(pl.x, pl.y, pl.z, pl.w), *material);
	ComPtr<IPhysicsScene> pScene;
	ComPtr<IScene>(gameObject->GetScene()).As(&pScene);
	pScene->GetPxScene()->addActor(*pxRigid);
}

void Plane::OnRemoved()
{
	if (pxRigid)
	{
		pxRigid->release();
		pxRigid = 0;
	}
}

PxRigidStatic* Plane::GetPxTarget()
{
	return pxRigid;
}

PxMaterial* Plane::GetMaterial()
{
	return material;
}

void Plane::SetMaterial(PxMaterial* mat)
{
	material = mat;
	if (pxRigid)
	{
		PxShape* pxShape;
		pxRigid->getShapes(&pxShape, 1);
		pxShape->setMaterials(&material, 1);
	}
}

void Plane::SetTransform(float x, float y, float z, float n)
{
	PxTransform tm = PxTransformFromPlaneEquation(PxPlane(x, y, z, n));
	pxRigid->setGlobalPose(tm);
}

HRESULT _stdcall RenderCom::QueryInterface(const IID& iid, void** ppvObject)
{
	if (iid == IID_IAComponent)
	{
		*ppvObject = static_cast<IAComponent*>(this);
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

ULONG _stdcall RenderCom::AddRef()
{
	return AComponent::AddRef();
}

ULONG _stdcall RenderCom::Release()
{
	return AComponent::Release();
}

void RenderCom::OnRender() {}

void RenderCom::OnRender2D() {}

void RenderCom::OnFetchData() {}

SpriteRender::SpriteRender() :
	texTransform(XMMatrixIdentity()), localTransform(XMMatrixIdentity()), transform(), mTexTransform(),
	mlocalTransform(), needUpdate(true) {}

void SpriteRender::OnAwake()
{
	ComPtr<IVirtualEngine>(gameObject->GetScene()->GetEngine()).As(&renderer);
	renderer->GetPointer(IID_ID3D11Device, (void**)&pDevice);
	renderer->GetPointer(IID_ID3D11DeviceContext, (void**)&pContext);
	renderer->GetPointer(IID_IDXGISwapChain, (void**)&pSwapChain);
}

void SpriteRender::OnFetchData()
{
	if (needUpdate)
	{
		needUpdate = false;
		mTexture = texture;
		mMaterial = material;
		mTexTransform = texTransform;
		mlocalTransform = localTransform;
	}
	transform = gameObject->transform->GetWorldMatrix();
}

void SpriteRender::OnRender()
{
	UINT stride = 32;
	UINT offset = 0;

	pContext->IASetVertexBuffers(0, 1, static_cast<Renderer*>(renderer.Get())->rectVertexBuffer.GetAddressOf(), &stride, &offset);

	renderer->SetWorldMatrix(mlocalTransform * transform);
	renderer->SetTextureMatrix(mTexTransform);
	if (mTexture)
		renderer->SendShaderData(SD_TEXTURE, mTexture->GetShaderResourceView(), 0);
	else
		renderer->SendShaderData(SD_TEXTURE, 0, 0);
	renderer->SendShaderData(SD_MATERIAL, &mMaterial, 0);
	renderer->StartRender();
	pContext->Draw(6, 0);
}

void SpriteRender::SetTexture(ITexture* tex)
{
	texture = tex;
	needUpdate = true;
}

ITexture* SpriteRender::GetTexture()
{
	return texture.Get();
}

XMMATRIX SpriteRender::GetTextureTransform()
{
	return texTransform;
}

void SpriteRender::SetTextureTransform(const XMMATRIX& matrix)
{
	texTransform = matrix;
	needUpdate = true;
}

XMMATRIX SpriteRender::GetLocalTransform()
{
	return localTransform;
}

void SpriteRender::SetLocalTransform(const XMMATRIX& matrix)
{
	localTransform = matrix;
	needUpdate = true;
}

Material SpriteRender::GetMaterial()
{
	return material;
}

void SpriteRender::SetMaterial(const Material& mat)
{
	material = mat;
	needUpdate = true;
}

void SpriteRender::SetSourceRect(const RectangleF& rect)
{
	texTransform = XMMatrixIdentity();
	texTransform *= XMMatrixScaling(rect.width / 1, rect.height / 1, 1);
	texTransform *= XMMatrixTranslation(rect.x, rect.y, 0);
	needUpdate = true;
}

TextRender2D::TextRender2D(bool _renderOnCanvas) :
	align(ALIGNMENT_TYPE_LEFT | ALIGNMENT_TYPE_TOP), outlineEnabled(false), bodyEnabled(true),
	outlineWidth(1), rect({ 0,0,100,100 }), copyText(false), needUpdate(true), location(0, 0), scale(1, 1),
	rotation(0), mOutlineEnabled(false), mBodyEnabled(true), mOutlineWidth(1), mRect({ 0,0,100,100 }),
	renderOnCanvas(_renderOnCanvas) {}

ITextFormat* TextRender2D::GetTextFormat()
{
	return pFormat.Get();
}

void TextRender2D::SetTextFormat(ITextFormat* format)
{
	pFormat = format;
	needUpdate = true;
}

wstring TextRender2D::GetText()
{
	return text;
}

void TextRender2D::SetText(const wstring& str)
{
	text = str;
	copyText = true;
}

bool TextRender2D::GetOutlineEnabled()
{
	return outlineEnabled;
}

void TextRender2D::SetOutlineEnabled(bool enable)
{
	outlineEnabled = enable;
	needUpdate = true;
}

bool TextRender2D::GetBodyEnabled()
{
	return bodyEnabled;
}

void TextRender2D::SetBodyEnabled(bool enable)
{
	bodyEnabled = enable;
	needUpdate = true;
}

float TextRender2D::GetOutlineWidth()
{
	return outlineWidth;
}

void TextRender2D::SetOutlineWidth(float width)
{
	outlineWidth = width;
	needUpdate = true;
}

ID2D1Brush* TextRender2D::GetBodyBrush()
{
	return pBodyBrush.Get();
}

void TextRender2D::SetBodyBrush(ID2D1Brush* brush)
{
	pBodyBrush = brush;
	needUpdate = true;
}

ID2D1Brush* TextRender2D::GetOutlineBrush()
{
	return pOutlineBrush.Get();
}

void TextRender2D::SetOutlineBrush(ID2D1Brush* brush)
{
	pOutlineBrush = brush;
	needUpdate = true;
}

D2D1_RECT_F TextRender2D::GetRect()
{
	return rect;
}

void TextRender2D::SetRect(const D2D1_RECT_F& r)
{
	rect = r;
	needUpdate = true;
}

void TextRender2D::OnAwake()
{
	ComPtr<IVirtualEngine>(gameObject->GetScene()->GetEngine()).As(&renderer);
	ComPtr<ID3D11Device> pDevice;
	renderer->GetPointer(__uuidof(ID3D11Device), &pDevice);
	pDevice.As(&mProtector);
	renderer->GetPointer(__uuidof(IDWriteFactory), (void**)&pDWriteFactory);
	if (!pBodyBrush)
		pBodyBrush = static_cast<Renderer*>(renderer.Get())->pBodyBrush;
	if (!pOutlineBrush)
		pOutlineBrush = static_cast<Renderer*>(renderer.Get())->pOutlineBrush;
	if (!pFormat)
		pFormat = static_cast<Renderer*>(renderer.Get())->pFormat;
	needUpdate = true;
}

void TextRender2D::OnFetchData()
{
	if (copyText)
	{
		copyText = false;
		mText = text;
		pDWriteFactory->CreateTextLayout(mText.c_str(), mText.length(), pFormat->GetDWriteTextFormat(), rect.right - rect.left, rect.bottom - rect.top, &pDWriteTextLayout);
	}
	if (needUpdate)
	{
		needUpdate = false;
		mRect = rect;
		mFormat = pFormat;
		mBodyBrush = pBodyBrush;
		mOutlineBrush = pOutlineBrush;
		mOutlineEnabled = outlineEnabled;
		mOutlineWidth = outlineWidth;
		mBodyEnabled = bodyEnabled;
	}
	location = gameObject->transform2D->location;
	scale = gameObject->transform2D->scale;
	rotation = gameObject->transform2D->rotation;
}

void TextRender2D::OnRender2D()
{
	if (renderOnCanvas)
	{
		if (pDWriteTextLayout)
		{
			ComPtr<IRenderTarget> pRT = renderer->GetRenderTarget();
			if (pRT->Is2DSupported())
			{
				ComPtr<ID2D1RenderTarget> pD2DRenderTarget = pRT->GetD2DRenderTarget();
				CustomTextRenderer* textRenderer = static_cast<Renderer*>(renderer.Get())->pCustomTextRenderer.Get();
				textRenderer->rect = mRect;
				textRenderer->location = location;
				textRenderer->scale = scale;
				textRenderer->rotation = rotation;
				textRenderer->m_pTextBodyBrush = mBodyBrush;
				textRenderer->m_pTextOutlineBrush = mOutlineBrush;
				textRenderer->mStrokeWidth = outlineWidth;
				textRenderer->outlineEnabled = outlineEnabled;
				textRenderer->bodyEnabled = bodyEnabled;
				textRenderer->m_pRenderTarget = pD2DRenderTarget;
				mProtector->Enter();
				pD2DRenderTarget->BeginDraw();
				pDWriteTextLayout->Draw(0, textRenderer, 0, 0);
				pD2DRenderTarget->EndDraw();
				mProtector->Leave();
			}
		}
	}
}

void TextRender2D::OnRender()
{
	if (!renderOnCanvas)
	{
		if (pDWriteTextLayout)
		{
			ComPtr<IRenderTarget> pRT = renderer->GetRenderTarget();
			if (pRT->Is2DSupported())
			{
				ComPtr<ID2D1RenderTarget> pD2DRenderTarget = pRT->GetD2DRenderTarget();
				CustomTextRenderer* textRenderer = static_cast<Renderer*>(renderer.Get())->pCustomTextRenderer.Get();
				textRenderer->rect = mRect;
				textRenderer->location = location;
				textRenderer->scale = scale;
				textRenderer->rotation = rotation;
				textRenderer->m_pTextBodyBrush = mBodyBrush;
				textRenderer->m_pTextOutlineBrush = mOutlineBrush;
				textRenderer->mStrokeWidth = outlineWidth;
				textRenderer->outlineEnabled = outlineEnabled;
				textRenderer->bodyEnabled = bodyEnabled;
				textRenderer->m_pRenderTarget = pD2DRenderTarget;
				mProtector->Enter();
				pD2DRenderTarget->BeginDraw();
				pDWriteTextLayout->Draw(0, textRenderer, 0, 0);
				pD2DRenderTarget->EndDraw();
				mProtector->Leave();
			}
		}
	}
}