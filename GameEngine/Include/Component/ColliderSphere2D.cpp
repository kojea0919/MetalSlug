#include "ColliderSphere2D.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh.h"
#include "../Resource/Material.h"
#include "Collision.h"
#include "ColliderRect.h"
#include "ColliderOBB2D.h"
#include "ColliderPixel.h"

CColliderSphere2D::CColliderSphere2D()
{
    m_eSceneComponentType = SCENECOMPONENT_TYPE::ST_2D;
    m_eSceneClassType = SCENECOMPONENT_CLASS_TYPE::COLLIDERSPHERE2D;
    m_eColliderType = COLLIDER_TYPE::SPHERE2D;

    m_tInfo.fRadius = 30.f;

    m_b2D = true;
}

CColliderSphere2D::CColliderSphere2D(const CColliderSphere2D& com)
    : CCollider(com)
{
    m_tInfo = com.m_tInfo;
}

CColliderSphere2D::~CColliderSphere2D()
{
}

void CColliderSphere2D::SetRadius(float fRadius)
{
    m_tInfo.fRadius = fRadius;

    SetWorldScale(fRadius, fRadius, 1.f);
}

bool CColliderSphere2D::Init()
{
    if (!CCollider::Init())
        return false;

    m_pDebugMesh = GET_SINGLE(CResourceManager)->FindMesh("ColliderSphere2D");

    return true;
}

void CColliderSphere2D::Start()
{
    CCollider::Start();

    SetWorldScale(m_tInfo.fRadius, m_tInfo.fRadius, 1.f);
}

void CColliderSphere2D::Update(float fTime)
{
    CCollider::Update(fTime);
}

void CColliderSphere2D::PostUpdate(float fTime)
{
    CCollider::PostUpdate(fTime);

    m_tInfo.vCenter = GetWorldPos();

    m_vMin = m_tInfo.vCenter - Vector3(m_tInfo.fRadius, m_tInfo.fRadius, 0.f);
    m_vMax = m_tInfo.vCenter + Vector3(m_tInfo.fRadius, m_tInfo.fRadius, 0.f);
}

void CColliderSphere2D::Collision(float fTime)
{
    CCollider::Collision(fTime);
}

void CColliderSphere2D::PrevRender(float fTime)
{
    CCollider::PrevRender(fTime);
}

void CColliderSphere2D::Render(float fTime)
{
    CCollider::Render(fTime);

    m_pDebugMesh->Render(fTime);
}

void CColliderSphere2D::PostRender(float fTime)
{
    CCollider::PostRender(fTime);
}

CColliderSphere2D* CColliderSphere2D::Clone()
{
    return nullptr;
}

bool CColliderSphere2D::Collision(CCollider* pCollider)
{
    switch (pCollider->GetColliderType())
    {
    case COLLIDER_TYPE::RECT:
        return CCollision::CollisionRectToSphere2D((CColliderRect*)pCollider, this);
        break;
    case COLLIDER_TYPE::SPHERE2D:
        return CCollision::CollisionSphere2DToSphere2D(this, (CColliderSphere2D*)pCollider);
    case COLLIDER_TYPE::OBB2D:
        return CCollision::CollisionSphere2DToOBB2D(this, (CColliderOBB2D*)pCollider);
    case COLLIDER_TYPE::PIXEL:
        return CCollision::CollisionSphere2DToPixel(this, (CColliderPixel*)pCollider);
    case COLLIDER_TYPE::LINE:
        break;
    }

    return false;
}

bool CColliderSphere2D::CollisionMouse(const Vector2& vMouse)
{
    Vector3	vDist = m_tInfo.vCenter - Vector3(vMouse.x, vMouse.y, 0.f);

    return vDist.Length() <= m_tInfo.fRadius;
}

void CColliderSphere2D::Save(FILE* pFile)
{
    CCollider::Save(pFile);
}

void CColliderSphere2D::Load(FILE* pFile)
{
    CCollider::Load(pFile);
}
