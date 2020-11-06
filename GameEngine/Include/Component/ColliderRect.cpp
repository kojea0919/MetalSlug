#include "ColliderRect.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh.h"
#include "../Resource/Material.h"
#include "Collision.h"
#include "ColliderSphere2D.h"
#include "ColliderOBB2D.h"
#include "ColliderPixel.h"

CColliderRect::CColliderRect()
    : m_vExtent(Vector2(50.f,50.f))
{
    m_eSceneComponentType = SCENECOMPONENT_TYPE::ST_2D;
    m_eSceneClassType = SCENECOMPONENT_CLASS_TYPE::COLLIDERRECT;
    m_eColliderType = COLLIDER_TYPE::RECT;

    m_b2D = true;
}

CColliderRect::CColliderRect(const CColliderRect& com)
    : CCollider(com)
{
    m_vExtent = com.m_vExtent;
    m_tInfo = com.m_tInfo;
}

CColliderRect::~CColliderRect()
{
}

void CColliderRect::SetExtent(float x, float y)
{
    m_vExtent.x = x;
    m_vExtent.y = y;

    SetWorldScale(m_vExtent.x, m_vExtent.y, 1.f);
}

void CColliderRect::SetExtent(const Vector2& vExtent)
{
    m_vExtent = vExtent;
    SetWorldScale(m_vExtent.x, m_vExtent.y, 1.f);
}

bool CColliderRect::Init()
{
    if (!CCollider::Init())
        return false;

    m_pDebugMesh = GET_SINGLE(CResourceManager)->FindMesh("ColliderRect");

    SetInheritRotX(false);
    SetInheritRotY(false);
    SetInheritRotZ(false);

    return true;
}

void CColliderRect::Start()
{
    CCollider::Start();

    SetWorldScale(m_vExtent.x, m_vExtent.y, 1.f);
}

void CColliderRect::Update(float fTime)
{
    CCollider::Update(fTime);
}

void CColliderRect::PostUpdate(float fTime)
{
    CCollider::PostUpdate(fTime);

    m_tInfo.fL = GetWorldPos().x - m_vExtent.x / 2.f;
    m_tInfo.fR = GetWorldPos().x + m_vExtent.x / 2.f;
    m_tInfo.fT = GetWorldPos().y + m_vExtent.y / 2.f;
    m_tInfo.fB = GetWorldPos().y - m_vExtent.y / 2.f;

    //Min,Max Setting
    m_vMin = Vector3(m_tInfo.fL, m_tInfo.fB, 0.f);
    m_vMax = Vector3(m_tInfo.fR, m_tInfo.fT, 0.f);
}

void CColliderRect::Collision(float fTime)
{
    CCollider::Collision(fTime);
}

void CColliderRect::PrevRender(float fTime)
{
    CCollider::PrevRender(fTime);
}

void CColliderRect::Render(float fTime)
{
    CCollider::Render(fTime);

    m_pMaterial->SetMaterial();

    m_pDebugMesh->Render(fTime);
}

void CColliderRect::PostRender(float fTime)
{
    CCollider::PostRender(fTime);
}

CColliderRect* CColliderRect::Clone()
{
    return nullptr;
}

bool CColliderRect::Collision(CCollider* pCollider)
{
    switch (pCollider->GetColliderType())
    {
    case COLLIDER_TYPE::RECT:
        return CCollision::CollisionRectToRect(this, (CColliderRect*)pCollider);
    case COLLIDER_TYPE::SPHERE2D:
        return CCollision::CollisionRectToSphere2D(this, (CColliderSphere2D*)pCollider);
    case COLLIDER_TYPE::OBB2D:
        return CCollision::CollisionRectToOBB2D(this, (CColliderOBB2D*)pCollider);
    case COLLIDER_TYPE::PIXEL:
        return CCollision::CollisionRectToPixel(this, (CColliderPixel*)pCollider);
    case COLLIDER_TYPE::LINE:
        break;
    }

    return false;
}

bool CColliderRect::CollisionMouse(const Vector2& vMouse)
{
    if (m_tInfo.fL > vMouse.x)
        return false;

    else if (m_tInfo.fR < vMouse.x)
        return false;

    else if (m_tInfo.fB > vMouse.y)
        return false;

    else if (m_tInfo.fT < vMouse.y)
        return false;

    return true;
}

void CColliderRect::Save(FILE* pFile)
{
    CCollider::Save(pFile);
}

void CColliderRect::Load(FILE* pFile)
{
    CCollider::Load(pFile);
}
