#include "TestObj.h"
#include "Component/Mesh2DComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Material.h"
#include "Component/ColliderPixel.h"
#include "Component/ColliderRect.h"

CTestObj::CTestObj()
	: m_pMesh(nullptr),m_pBody(nullptr)
{
}

CTestObj::CTestObj(const CTestObj& bullet)
	: CGameObject(bullet)
{
}

CTestObj::~CTestObj()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pBody);
}

bool CTestObj::Init()
{
	m_pMesh = CreateComponent<CMesh2DComponent>("Mesh");
	m_pBody = CreateComponent<CColliderPixel>("Body");

	SetRootComponent(m_pMesh);

	m_pMesh->SetRelativeScale(200.f, 200.f, 1.f);
	m_pMesh->SetRelativePos(500.f, 700.f, 0.f);
	m_pMesh->SetPivot(0.5f, 0.5f, 0.f);

	//m_pBody->SetExtent(200.f, 100.f);
	//m_pBody->SetRadius(60.f);
	m_pBody->SetPixel(TEXT("PixelCollision.png"));
	m_pBody->SetIgnoreColor(255, 0, 255);
	m_pBody->SetCollisionProfile("Monster");

	m_pBody->SetCallback<CTestObj>(Collision_State::Begin, this,
		&CTestObj::CollisionBegin);
	m_pBody->SetCallback<CTestObj>(Collision_State::End, this,
		&CTestObj::CollisionEnd);

	m_pMesh->AddChild(m_pBody);

	CMaterial* pMaterial = m_pScene->GetResourceManager()->FindMaterial("TestPixelMtrl");

	m_pMesh->SetMaterial(pMaterial);

	SAFE_RELEASE(pMaterial);


	return true;
}

void CTestObj::Start()
{
	CGameObject::Start();
}

void CTestObj::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CTestObj::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CTestObj::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CTestObj::PrevRender(float fTime)
{
	CGameObject::PrevRender(fTime);
}

void CTestObj::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CTestObj::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

void CTestObj::CollisionBegin(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CTestObj::CollisionEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
}
