#include "Bullet.h"
#include "Component/Mesh2DComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Material.h"
#include "Component/Camera.h"
#include "Component/TestCamera.h"
#include "Component/SpriteComponent.h"

CBullet::CBullet() :
	m_pMesh(nullptr)
{
}

CBullet::CBullet(const CBullet& bullet) :
	CGameObject(bullet)
{
}

CBullet::~CBullet()
{
	SAFE_RELEASE(m_pMesh);
}

bool CBullet::Init()
{
	m_pMesh = CreateComponent<CMesh2DComponent>("Bullet");

	SetRootComponent(m_pMesh);

	m_pMesh->SetRelativeScale(150.f, 80.f, 1.f);
	m_pMesh->SetRelativePos(0.f, 0.f, 0.f);
	m_pMesh->SetPivot(0.5f, 0.5f, 0.f);

	CMaterial* pMaterial = m_pMesh->GetMaterial();

	pMaterial->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.f);

	SAFE_RELEASE(pMaterial);

	m_fDistance = 600.f;

	m_pMesh->SetMaterial("BulletMtrl");

	return true;
}

void CBullet::Start()
{
	CGameObject::Start();
}

void CBullet::Update(float fTime)
{
	CGameObject::Update(fTime);

	/*float	fSpeed = 500.f * fTime;

	AddWorldPos(GetWorldAxis(AXIS_X) * fSpeed);

	m_fDistance -= fSpeed;

	if (m_fDistance <= 0.f)
		Destroy();*/
}

void CBullet::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);
}

void CBullet::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CBullet::PrevRender(float fTime)
{
	CGameObject::PrevRender(fTime);
}

void CBullet::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CBullet::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}
