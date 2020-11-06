#include "Monster.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Material.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderRect.h"

CMonster::CMonster()
    : m_pMesh(nullptr)
{
}

CMonster::CMonster(const CMonster& monster)
    : CGameObject(monster)
{
}

CMonster::~CMonster()
{
    SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pBody);
}

bool CMonster::Init()
{
	m_pMesh = CreateComponent<CSpriteComponent>("Mesh");
	m_pBody = CreateComponent<CColliderRect>("Body");

	SetRootComponent(m_pMesh);

	m_pMesh->SetRelativeScale(45.f, 60.f, 1.f);
	m_pMesh->SetRelativePos(0.f, 0.f, 0.f);
	m_pMesh->SetPivot(0.5f, 0.f, 0.f);

	m_pMesh->AddSpriteInfo("Idle", "MonsterIdle",
		true);
	m_pMesh->AddSpriteInfo("Run", "MonsterRun", true);
	m_pMesh->AddSpriteInfo("Attack", "MonsterAttack", false);

	m_pBody->SetExtent(200.f, 100.f);

	m_pBody->SetCallback<CMonster>(Collision_State::Begin, this,
		&CMonster::CollisionBegin);
	m_pBody->SetCallback<CMonster>(Collision_State::End, this,
		&CMonster::CollisionEnd);

	m_pMesh->AddChild(m_pBody);

	CMaterial* pMaterial = m_pMesh->GetMaterial();

	//pMaterial->SetDiffuseColor(0.7f, 0.7f, 0.7f, 1.f);

	SAFE_RELEASE(pMaterial);

	return true;
}

void CMonster::Start()
{
	CGameObject::Start();
}

void CMonster::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CMonster::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CMonster::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CMonster::PrevRender(float fTime)
{
	CGameObject::PrevRender(fTime);
}

void CMonster::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CMonster::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

void CMonster::CollisionBegin(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CMonster::CollisionEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
}
