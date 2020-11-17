#include "Collider.h"
#include "../Resource/Mesh.h"
#include "../Resource/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/SceneCollision.h"
#include "../Scene/Scene.h"
#include "../CollisionManager.h"

CCollider::CCollider()
    : m_pDebugMesh(nullptr),
    m_bUI(false),m_b2D(false),m_pProfile(nullptr),
    m_bMouseCollision(false),m_bCollisionEnable(true)
{
}

CCollider::CCollider(const CCollider& com)
    : CPrimitiveComponent(com)
{
    m_bUI = com.m_bUI;
    m_pDebugMesh = com.m_pDebugMesh;
    m_pProfile = com.m_pProfile;
    m_PrevCollisionList.clear();
    m_SectionList.clear();
    m_CheckSectionList.clear();
    m_bCurrentFrameSectionCheck = false;
    m_bMouseCollision = false;

    if (m_pDebugMesh)
        m_pDebugMesh->AddRef();
}

CCollider::~CCollider()
{
    auto iter = m_PrevCollisionList.begin();
    auto iterEnd = m_PrevCollisionList.end();

    for (; iter != iterEnd; ++iter)
    {
        //상대 Collider에서 제거
        (*iter)->DeletePrevCollider(this);

        //떨어졌을 때 Callback함수 호출
        //----------------------------------------------
        (*iter)->CallCollisionCallback(Collision_State::End,
            this, 0.f);
        CallCollisionCallback(Collision_State::End, *iter, 0.f);
        //----------------------------------------------
    }

    SAFE_RELEASE(m_pDebugMesh);
}

bool CCollider::Init()
{
    if (!CPrimitiveComponent::Init())
        return false;

    //Collider Material Setting
    CMaterial* pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("Collider");

    SetMaterial(pMaterial);

    SAFE_RELEASE(pMaterial);

    m_pProfile = GET_SINGLE(CCollisionManager)->FindProfile("Static");

    return true;
}

void CCollider::Start()
{
    CPrimitiveComponent::Start();
}

void CCollider::Update(float fTime)
{
    CPrimitiveComponent::Update(fTime);
}

void CCollider::PostUpdate(float fTime)
{
    CPrimitiveComponent::PostUpdate(fTime);
}

void CCollider::Collision(float fTime)
{
    CPrimitiveComponent::Collision(fTime);

    //CollisionManager에 해당 Collider등록
    m_pScene->GetCollisionManager()->AddCollider(this);
}

void CCollider::PrevRender(float fTime)
{
    CPrimitiveComponent::PrevRender(fTime);
}

void CCollider::Render(float fTime)
{
    if (m_PrevCollisionList.empty() && !m_bMouseCollision)
        m_pMaterial->SetDiffuseColor(0.f, 1.f, 0.f, 1.f);

    //충돌한 충돌체가 있거나 마우스 충돌을 한 경우 
    else
        m_pMaterial->SetDiffuseColor(1.f, 0.f, 0.f, 1.f);

    CPrimitiveComponent::Render(fTime);
}

void CCollider::PostRender(float fTime)
{
    CPrimitiveComponent::PostRender(fTime);
}

CCollider* CCollider::Clone()
{
    return nullptr;
}

void CCollider::Save(FILE* pFile)
{
    CPrimitiveComponent::Save(pFile);
}

void CCollider::Load(FILE* pFile)
{
    CPrimitiveComponent::Load(pFile);

    m_pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("Collider");

    m_pProfile = GET_SINGLE(CCollisionManager)->FindProfile("Static");
}

void CCollider::SetCollisionProfile(const string& strName)
{
    m_pProfile = GET_SINGLE(CCollisionManager)->FindProfile(strName);
}

void CCollider::CheckPrevColliderSection(float fTime)
{
    auto PrevCollideriter = m_PrevCollisionList.begin();
    auto PrevCollideriterEnd = m_PrevCollisionList.end();

    //이전 프레임에서 충돌중이었던 Collider 순회
    //------------------------------------------
    for (; PrevCollideriter != PrevCollideriterEnd;)
    {
        //현재 Collider와 해당 Collider가
        //같이있는 Section이 있는지 Check
        //-------------------------------
        auto Sectioniter = m_SectionList.begin();
        auto SectioniterEnd = m_SectionList.end();

        bool bCheck = false;

        for (; Sectioniter != SectioniterEnd; ++Sectioniter)
        {
            auto TargetSectioniter = (*PrevCollideriter)->m_SectionList.begin();
            auto TargetSectioniterEnd = (*PrevCollideriter)->m_SectionList.end();

            for (; TargetSectioniter != TargetSectioniterEnd; ++TargetSectioniter)
            {
                if (*Sectioniter == *TargetSectioniter)
                {
                    bCheck = true;
                    break;
                }
            }

            if (bCheck)
                break;
        }
        //-------------------------------

        //같이있는 Section이 없는 경우 떨어지는 처리
        //-------------------------------
        if (!bCheck)
        {
            CallCollisionCallback(Collision_State::End, *PrevCollideriter, fTime);
            (*PrevCollideriter)->CallCollisionCallback(Collision_State::End,
                this, fTime);

            (*PrevCollideriter)->DeletePrevCollider(this);
            PrevCollideriter = m_PrevCollisionList.erase(PrevCollideriter);
            PrevCollideriterEnd = m_PrevCollisionList.end();

            continue;
        }
        //-------------------------------

        ++PrevCollideriter;
    }
    //------------------------------------------

}

void CCollider::AddPrevCollider(CCollider* pCollider)
{
    m_PrevCollisionList.push_back(pCollider);
}

void CCollider::DeletePrevCollider(CCollider* pCollider)
{
    auto iter = m_PrevCollisionList.begin();
    auto iterEnd = m_PrevCollisionList.end();

    for (; iter != iterEnd; ++iter)
    {
        if (*iter == pCollider)
        {
            m_PrevCollisionList.erase(iter);
            return;
        }
    }
}

bool CCollider::EmptyPrevCollision()
{
    return m_PrevCollisionList.empty();
}

bool CCollider::CheckPrevCollision(CCollider* pCollider)
{
    auto iter = m_PrevCollisionList.begin();
    auto iterEnd = m_PrevCollisionList.end();

    for (; iter != iterEnd; ++iter)
    {
        if (*iter == pCollider)
            return true;
    }

    return false;
}

void CCollider::CallCollisionCallback(Collision_State eState, CCollider* pDest, float fTime)
{
    auto iter = m_Callback[(int)eState].begin();
    auto iterEnd = m_Callback[(int)eState].end();

    for (; iter != iterEnd; ++iter)
    {
        (*iter)(this, pDest, fTime);
    }
}

void CCollider::CallMouseCollisionCallback(Collision_State eState, const Vector2& vMousePos, float fTime)
{
    auto iter = m_MouseCallback[(int)eState].begin();
    auto iterEnd = m_MouseCallback[(int)eState].end();

    for (; iter != iterEnd; ++iter)
    {
        (*iter)(this, vMousePos, fTime);
    }
}

void CCollider::AddCurrentCollision(CCollider* pCollider)
{
    if (!CheckCurrentCollision(pCollider))
        m_CurrentCollisionList.push_back(pCollider);
}

bool CCollider::CheckCurrentCollision(CCollider* pCollider)
{
    auto iter = m_CurrentCollisionList.begin();
    auto iterEnd = m_CurrentCollisionList.end();

    for (; iter != iterEnd; ++iter)
    {
        if (*iter == pCollider)
            return true;
    }

    return false;
}
