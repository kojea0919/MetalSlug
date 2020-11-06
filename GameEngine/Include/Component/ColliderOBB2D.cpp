#include "ColliderOBB2D.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh.h"	
#include "../Resource/Material.h"
#include "Collision.h"
#include "ColliderSphere2D.h"
#include "ColliderRect.h"

CColliderOBB2D::CColliderOBB2D()
{
	m_eSceneComponentType = SCENECOMPONENT_TYPE::ST_2D;
	m_eSceneClassType = SCENECOMPONENT_CLASS_TYPE::COLLIDEROBB2D;
	m_eColliderType = COLLIDER_TYPE::OBB2D;

	m_tInfo.vLength = Vector3(30.f, 30.f, 0.f);

	m_b2D = true;
}

CColliderOBB2D::CColliderOBB2D(const CColliderOBB2D& com)
	: CCollider(com)
{
	m_tInfo = com.m_tInfo;
}

CColliderOBB2D::~CColliderOBB2D()
{
}

void CColliderOBB2D::SetExtent(float x, float y)
{
	m_tInfo.vLength.x = x / 2.f;
	m_tInfo.vLength.y = y / 2.f;

	SetWorldScale(m_tInfo.vLength.x * 2.f, m_tInfo.vLength.y * 2.f, 1.f);
}

void CColliderOBB2D::SetExtent(const Vector2& vExtent)
{
	m_tInfo.vLength.x = vExtent.x / 2.f;
	m_tInfo.vLength.y = vExtent.y / 2.f;

	SetWorldScale(m_tInfo.vLength.x * 2.f, m_tInfo.vLength.y * 2.f, 1.f);
}

bool CColliderOBB2D::Init()
{
	if (!CCollider::Init())
		return false;

	m_pDebugMesh = GET_SINGLE(CResourceManager)->FindMesh("ColliderRect");

	return true;
}

void CColliderOBB2D::Start()
{
	CCollider::Start();
	
	SetWorldScale(m_tInfo.vLength.x * 2.f, m_tInfo.vLength.y * 2.f, 1.f);
}

void CColliderOBB2D::Update(float fTime)
{
	CCollider::Update(fTime);
}

void CColliderOBB2D::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);

	m_tInfo.vCenter = GetWorldPos();

	m_tInfo.vAxis[AXIS::AXIS_X] = GetWorldAxis(AXIS::AXIS_X);
	m_tInfo.vAxis[AXIS::AXIS_Y] = GetWorldAxis(AXIS::AXIS_Y);
	m_tInfo.vAxis[AXIS::AXIS_Z] = GetWorldAxis(AXIS::AXIS_Z);

	Vector3	vPos[4] = {};

	vPos[0] = m_tInfo.vCenter - m_tInfo.vAxis[AXIS_X] * m_tInfo.vLength.x +
		m_tInfo.vAxis[AXIS_Y] * m_tInfo.vLength.y;
	vPos[1] = m_tInfo.vCenter + m_tInfo.vAxis[AXIS_X] * m_tInfo.vLength.x +
		m_tInfo.vAxis[AXIS_Y] * m_tInfo.vLength.y;

	vPos[2] = m_tInfo.vCenter - m_tInfo.vAxis[AXIS_X] * m_tInfo.vLength.x -
		m_tInfo.vAxis[AXIS_Y] * m_tInfo.vLength.y;
	vPos[3] = m_tInfo.vCenter + m_tInfo.vAxis[AXIS_X] * m_tInfo.vLength.x -
		m_tInfo.vAxis[AXIS_Y] * m_tInfo.vLength.y;

	m_vMin = vPos[0];
	m_vMax = vPos[0];

	for (int i = 1; i < 4; ++i)
	{
		if (m_vMin.x > vPos[i].x)
			m_vMin.x = vPos[i].x;

		if (m_vMin.y > vPos[i].y)
			m_vMin.y = vPos[i].y;

		if (m_vMax.x < vPos[i].x)
			m_vMax.x = vPos[i].x;

		if (m_vMax.y < vPos[i].y)
			m_vMax.y = vPos[i].y;
	}
}

void CColliderOBB2D::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderOBB2D::PrevRender(float fTime)
{
	CCollider::PrevRender(fTime);
}

void CColliderOBB2D::Render(float fTime)
{
	CCollider::Render(fTime);

	m_pMaterial->SetMaterial();

	m_pDebugMesh->Render(fTime);
}

void CColliderOBB2D::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

CColliderOBB2D* CColliderOBB2D::Clone()
{
	return nullptr;
}

bool CColliderOBB2D::Collision(CCollider* pCollider)
{
	switch (pCollider->GetColliderType())
	{
	case COLLIDER_TYPE::RECT:
		return CCollision::CollisionRectToOBB2D((CColliderRect*)pCollider, this);
	case COLLIDER_TYPE::SPHERE2D:
		return CCollision::CollisionSphere2DToOBB2D((CColliderSphere2D*)pCollider, this);
	case COLLIDER_TYPE::OBB2D:
		return CCollision::CollisionOBB2DToOBB2D(this, (CColliderOBB2D*)pCollider);
	case COLLIDER_TYPE::PIXEL:
		break;
	case COLLIDER_TYPE::LINE:
		break;
	}

	return false;
}

bool CColliderOBB2D::CollisionMouse(const Vector2& vMouse)
{
	Vector3	vLength = m_tInfo.vCenter - Vector3(vMouse.x, vMouse.y, 0.f);
	Vector3	vAxis = m_tInfo.vAxis[AXIS_X];

	float	fLength = abs(vLength.Dot(vAxis));

	if (fLength > m_tInfo.vLength.x)
		return false;

	vAxis = m_tInfo.vAxis[AXIS_Y];

	fLength = abs(vLength.Dot(vAxis));

	if (fLength > m_tInfo.vLength.y)
		return false;

	return true;
}

void CColliderOBB2D::Save(FILE* pFile)
{
	CCollider::Save(pFile);
}

void CColliderOBB2D::Load(FILE* pFile)
{
	CCollider::Load(pFile);

	m_pDebugMesh = GET_SINGLE(CResourceManager)->FindMesh("ColliderRect");
}
