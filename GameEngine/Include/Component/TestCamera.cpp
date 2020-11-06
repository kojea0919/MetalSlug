#include "TestCamera.h"
#include "../Scene/Scene.h"
#include "../GameObject.h"

CTestCamera::CTestCamera()
{
}

CTestCamera::CTestCamera(const CTestCamera& cam)
	: CCamera(cam)
{
}

CTestCamera::~CTestCamera()
{
}

void CTestCamera::SetNewComponent(CSceneComponent* pNewCom)
{
	if (m_pParent)
	{
		//CGameObject* pParentObject = m_pParent->GetGameObject();

		//pParentObject->SetCamera(nullptr);

		//SAFE_RELEASE(pParentObject);

		//부모의 입장에서 나를 제거
		m_pParent->DeleteChild(this);
	}

	//부모를 Update
	m_pParent = pNewCom;

	pNewCom->AddChild(this);
}

void CTestCamera::BackToPlayer()
{
	CGameObject* pPlayer = m_pScene->GetGameMode()->GetPlayer();

	CSceneComponent* pRootCom = pPlayer->GetRootComponent();

	m_pParent = nullptr;

	SetNewComponent(pRootCom);

	SAFE_RELEASE(pRootCom);
	SAFE_RELEASE(pPlayer);
}

bool CTestCamera::Init()
{
	if (!CCamera::Init())
		return false;

	return true;
}

void CTestCamera::Start()
{
	CCamera::Start();
}

void CTestCamera::Update(float fTime)
{
	CCamera::Update(fTime);
}

void CTestCamera::PostUpdate(float fTime)
{
	CCamera::PostUpdate(fTime);
}

void CTestCamera::Collision(float fTime)
{
	CCamera::Collision(fTime);
}

void CTestCamera::PrevRender(float fTime)
{
	CCamera::PrevRender(fTime);
}

void CTestCamera::Render(float fTime)
{
	CCamera::Render(fTime);
}

void CTestCamera::PostRender(float fTime)
{
	CCamera::PostRender(fTime);
}

CTestCamera* CTestCamera::Clone()
{
	return new CTestCamera(*this);
}
