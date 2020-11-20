#include "Player.h"
#include "Component/Mesh2DComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Material.h"
#include "Input.h"
#include "Bullet.h"
#include "Component/Camera.h"
#include "Component/TestCamera.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderRect.h"
#include "Component/ColliderOBB2D.h"
#include "Component/ColliderPixel.h"
#include "Component/ColliderSphere2D.h"
#include "PathManager.h"
#include "../UI/Inventory.h"


CPlayer::CPlayer()
	:m_pLowerMesh(nullptr), m_pUpperMesh(nullptr),
	m_pCamera(nullptr), m_pBody(nullptr), m_fColorChangeTime(1.f),
	m_fColorChangeTimer(0.f), m_fPrevVelocity(0.f),
	m_bIsRightDir(true), m_bPrevRightDir(true),
	m_bIsShooting(false), m_bRecvFireInput(false),
	m_bIsAimUp(false),m_eUpperAnimState(PLAYER_ANIMSTATE::PS_IDLE),
	m_eLowerAnimState(PLAYER_ANIMSTATE::PS_IDLE),
	m_bIsStand(true),m_bIsCanFire(true),
	m_bRecvDownInput(false),m_eCurWeaponState(WEAPON_STATE::WS_HEAVY),
	m_pInventory(nullptr)
{
}

CPlayer::CPlayer(const CPlayer& player)	:
	CGameObject(player)
{
}

CPlayer::~CPlayer()
{
	SAFE_RELEASE(m_pLowerMesh);
	SAFE_RELEASE(m_pUpperMesh);
	SAFE_RELEASE(m_pBody);
	SAFE_RELEASE(m_pCamera);
}

bool CPlayer::LoadPlayerAnimList()
{
	//Lower
	//-------------------------------------------------------------------------

	//AnimList���� ��� Setting
	//-----------------------------------------------
	char strFullPath[MAX_PATH] = {};
	const char* pPath = GET_SINGLE(CPathManager)->FindPathMultibyte(ANI_PATH);
	if (!pPath)
		return false;

	strcpy_s(strFullPath, pPath);
	strcat_s(strFullPath, df_PLAYER_LOWERANIM_FILENAME);
	//-----------------------------------------------

	FILE* pFile = nullptr;
	fopen_s(&pFile, strFullPath, "rt");
	if (!pFile)
		return false;

	//�ִϸ��̼� �� Setting
	//-------------------
	int iSize = 0;
	fscanf_s(pFile, "%d\n", &iSize);
	//-------------------

	//�ִϸ��̼� Setting
	//-------------------------------
	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		char strAnimName[MAX_PATH] = {};
		fscanf_s(pFile, "%s\n", strAnimName, MAX_PATH);

		m_pLowerMesh->AddSpriteInfo(strAnimName, strAnimName);
	}
	//-------------------------------

	fclose(pFile);

	//-------------------------------------------------------------------------


	memset(strFullPath, 0, MAX_PATH);

	//Upper
	//-------------------------------------------------------------------------
	strcpy_s(strFullPath, pPath);
	strcat_s(strFullPath, df_PLAYER_UPPERANIM_FILENAME);

	pFile = nullptr;
	fopen_s(&pFile, strFullPath, "rt");
	if (!pFile)
		return false;

	//�ִϸ��̼� �� Setting
	//-------------------
	iSize = 0;
	fscanf_s(pFile, "%d\n", &iSize);
	//-------------------

	//�ִϸ��̼� Setting
	//-------------------------------
	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		char strAnimName[MAX_PATH] = {};
		fscanf_s(pFile, "%s\n", strAnimName, MAX_PATH);

		m_pUpperMesh->AddSpriteInfo(strAnimName, strAnimName);
	}
	//-------------------------------

	fclose(pFile);

	//-------------------------------------------------------------------------


	return true;
}
void CPlayer::UpdateMoveState(float fTime)
{
	float fCurVelocity = m_pLowerMesh->GetCurrentVelocity();
	

	//���� ���� -> �����̴� ����
	//---------------------------------	
	if (m_fPrevVelocity == 0 && fCurVelocity > 0)
	{
		//���ִ� ���� �⺻ ������
		if(m_bIsStand)
			SetMoveStartState();
		//�ɾ��ִ� ���� ���� ������
		else
			SetSitMoveState();

	}
	//---------------------------------

	//���� ����
	//---------------------------------
	else if (m_fPrevVelocity > 0 && fCurVelocity == 0)
	{
		//���ִ� ���
		if (m_bIsStand)
			SetIdleState();
		//�ɾ��ִ� ���
		else
		{
			//�÷��̾ �ɾƼ� ���� ��ų� ��ź�� ���� ��쿡�� �ش� �ִϸ��̼��� play
			if (!m_bIsShooting && m_eLowerAnimState != PLAYER_ANIMSTATE::PS_SITTHROWBOMB)
				SetSitIdleState();
		}
	}
	//---------------------------------
}

//void CPlayer::UpdateAttackState(float fTime)
//{
//	//���� �⺻ �������� ��� �⺻���� Setting
//	//---------------------------------
//	if (m_bRecvFireInput)
//	{
//		if (m_bIsAimUp)
//			SetNormalUpShotState();
//		else if (!m_bIsStand)
//			SetSitShotState();
//		else
//			SetNormalMidShotState();
//	}
//	//---------------------------------
//}

void CPlayer::UpdateAnimation(float fTime)
{
	bool bIsNormal = true;
	if (m_eCurWeaponState == WEAPON_STATE::WS_NORMAL)
		bIsNormal = true;
	else
		bIsNormal = false;

	if (m_bIsRightDir)
	{
		switch (m_eLowerAnimState)
		{
		case PLAYER_ANIMSTATE::PS_IDLE:
				m_pLowerMesh->ChangeSprite("PlayerRightLowerIdle");
			break;
		case PLAYER_ANIMSTATE::PS_MOVESTART:
			m_pLowerMesh->ChangeSprite("PlayerRightLowerMoveStart");
			break;
		case PLAYER_ANIMSTATE::PS_MOVE:
			m_pLowerMesh->ChangeSprite("PlayerRightLowerMove");
			break;
		case PLAYER_ANIMSTATE::PS_SITSTART:
			if(bIsNormal)
				m_pLowerMesh->ChangeSprite("PlayerRightSitStart");
			else
				m_pLowerMesh->ChangeSprite("PlayerRightSitStart_H");
			break;
		case PLAYER_ANIMSTATE::PS_SITIDLE:
			if(bIsNormal)
				m_pLowerMesh->ChangeSprite("PlayerRightSitIdle");
			else
				m_pLowerMesh->ChangeSprite("PlayerRightSitIdle_H");
			break;
		case PLAYER_ANIMSTATE::PS_UP:
			if(bIsNormal)
				m_pLowerMesh->ChangeSprite("PlayerRightUpStart");
			else
				m_pLowerMesh->ChangeSprite("PlayerRightUpStart_H");
			break;
		case PLAYER_ANIMSTATE::PS_SITMOVE:
			if(bIsNormal)
				m_pLowerMesh->ChangeSprite("PlayerRightSitMove");
			else
				m_pLowerMesh->ChangeSprite("PlayerRightSitMove_H");
			break;
		case PLAYER_ANIMSTATE::PS_SITSHOT:
			if(bIsNormal)
				m_pLowerMesh->ChangeSprite("PlayerRightSitShot");
			else
				m_pLowerMesh->ChangeSprite("PlayerRightSitShot_H");
			break;
		case PLAYER_ANIMSTATE::PS_SITSHOTEND:
			if(bIsNormal)
				m_pLowerMesh->ChangeSprite("PlayerRightSitShotEnd");
			else
				m_pLowerMesh->ChangeSprite("PlayerRightSitShotEnd_H");
			break;
		case PLAYER_ANIMSTATE::PS_SITTHROWBOMB:
			if(bIsNormal)
				m_pLowerMesh->ChangeSprite("PlayerRightSitThrowBomb");
			else
				m_pLowerMesh->ChangeSprite("PlayerRightSitThrowBomb_H");
			break;
		default:
			break;
		}

		switch (m_eUpperAnimState)
		{
		case PLAYER_ANIMSTATE::PS_IDLE:
			m_pUpperMesh->Enable(true);

			if(bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerRightUpperIdle");
			else
				m_pUpperMesh->ChangeSprite("PlayerRightUpperIdle_H");
			break;
		case PLAYER_ANIMSTATE::PS_MOVESTART:
			m_pUpperMesh->Enable(true);

			if(bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerRightUpperMoveStart");
			else
				m_pUpperMesh->ChangeSprite("PlayerRightUpperMoveStart_H");
			break;
		case PLAYER_ANIMSTATE::PS_MOVE:
			m_pUpperMesh->Enable(true);

			if (bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerRightUpperMove");
			else
				m_pUpperMesh->ChangeSprite("PlayerRightUpperMove_H");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALSHOT:
			m_pUpperMesh->Enable(true);

			if(bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerRightNormalMidShot");
			else
				m_pUpperMesh->ChangeSprite("PlayerRightHeavyMidShot");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALSHOTEND:
			m_pUpperMesh->Enable(true);

			if(bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerRightNormalMidShotEnd");
			else
				m_pUpperMesh->ChangeSprite("PlayerRightHeavyMidShotEnd");
			break;
		case PLAYER_ANIMSTATE::PS_AIMUPSTART:
			m_pUpperMesh->Enable(true);

			if(bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerRightAimUpStart");
			else
			{
				if(m_bIsShooting)
					m_pUpperMesh->ChangeSprite("PlayerRightAimUpStartHeavyShot");
				else
					m_pUpperMesh->ChangeSprite("PlayerRightAimUpStart_H");
			}
			break;
		case PLAYER_ANIMSTATE::PS_ANIMUPIDLE:
			m_pUpperMesh->Enable(true);

			if(bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerRightAimUpIdle");
			else
				m_pUpperMesh->ChangeSprite("PlayerRightAimUpIdle_H");
			break;
		case PLAYER_ANIMSTATE::PS_AIMDOWN:
			m_pUpperMesh->Enable(true);

			if(bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerRightAimDown");
			else
			{
				if(m_bIsShooting)
					m_pUpperMesh->ChangeSprite("PlayerRightAimDownHeavyShot");
				else
					m_pUpperMesh->ChangeSprite("PlayerRightAimDown_H");
			}
			break;
		case PLAYER_ANIMSTATE::PS_NORMALUPSHOT:
			m_pUpperMesh->Enable(true);

			if(bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerRightNormalUpShot");
			else
				m_pUpperMesh->ChangeSprite("PlayerRightHeavyUpShot");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALUPSHOTEND:
			m_pUpperMesh->Enable(true);

			if(bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerRightNormalUpShotEnd");
			else
				m_pUpperMesh->ChangeSprite("PlayerRightHeavyUpShotEnd");
			break;
		case PLAYER_ANIMSTATE::PS_SITSTART:
		case PLAYER_ANIMSTATE::PS_SITIDLE:
		case PLAYER_ANIMSTATE::PS_UP:
		case PLAYER_ANIMSTATE::PS_SITSHOT:
		case PLAYER_ANIMSTATE::PS_SITSHOTEND:
			m_pUpperMesh->Enable(false);
			break;
		case PLAYER_ANIMSTATE::PS_THROWBOMB:
			m_pUpperMesh->Enable(true);
			
			if(bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerRightThrowBomb");
			else
				m_pUpperMesh->ChangeSprite("PlayerRightThrowBomb_H");
			break;
		case PLAYER_ANIMSTATE::PS_SITTHROWBOMB:
			m_pUpperMesh->Enable(false);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (m_eLowerAnimState)
		{
		case PLAYER_ANIMSTATE::PS_IDLE:
			m_pLowerMesh->ChangeSprite("PlayerLeftLowerIdle");
			break;
		case PLAYER_ANIMSTATE::PS_MOVESTART:
			m_pLowerMesh->ChangeSprite("PlayerLeftLowerMoveStart");
			break;
		case PLAYER_ANIMSTATE::PS_MOVE:
			m_pLowerMesh->ChangeSprite("PlayerLeftLowerMove");
			break;
		case PLAYER_ANIMSTATE::PS_SITSTART:
			if(bIsNormal)
				m_pLowerMesh->ChangeSprite("PlayerLeftSitStart");
			else
				m_pLowerMesh->ChangeSprite("PlayerLeftSitStart_H");
			break;
		case PLAYER_ANIMSTATE::PS_SITIDLE:
			if(bIsNormal)
				m_pLowerMesh->ChangeSprite("PlayerLeftSitIdle");
			else
				m_pLowerMesh->ChangeSprite("PlayerLeftSitIdle_H");
			break;
		case PLAYER_ANIMSTATE::PS_UP:
			if(bIsNormal)
				m_pLowerMesh->ChangeSprite("PlayerLeftUpStart");
			else
				m_pLowerMesh->ChangeSprite("PlayerLeftUpStart_H");
			break;
		case PLAYER_ANIMSTATE::PS_SITMOVE:
			if(bIsNormal)
				m_pLowerMesh->ChangeSprite("PlayerLeftSitMove");
			else
				m_pLowerMesh->ChangeSprite("PlayerLeftSitMove_H");
			break;
		case PLAYER_ANIMSTATE::PS_SITSHOT:
			if(bIsNormal)
				m_pLowerMesh->ChangeSprite("PlayerLeftSitShot");
			else
				m_pLowerMesh->ChangeSprite("PlayerLeftSitShot_H");
			break;
		case PLAYER_ANIMSTATE::PS_SITSHOTEND:
			if(bIsNormal)
				m_pLowerMesh->ChangeSprite("PlayerLeftSitShotEnd");
			else
				m_pLowerMesh->ChangeSprite("PlayerLeftSitShotEnd_H");
			break;
		case PLAYER_ANIMSTATE::PS_SITTHROWBOMB:
			if(bIsNormal)
				m_pLowerMesh->ChangeSprite("PlayerLeftSitThrowBomb");
			else
				m_pLowerMesh->ChangeSprite("PlayerLeftSitThrowBomb_H");
			break;
		default:
			break;
		}

		switch (m_eUpperAnimState)
		{
		case PLAYER_ANIMSTATE::PS_IDLE:
			m_pUpperMesh->Enable(true);

			if(bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerLeftUpperIdle");
			else
				m_pUpperMesh->ChangeSprite("PlayerLeftUpperIdle_H");
			break;
		case PLAYER_ANIMSTATE::PS_MOVESTART:
			m_pUpperMesh->Enable(true);

			if (bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerLeftUpperMoveStart");
			else
				m_pUpperMesh->ChangeSprite("PlayerLeftUpperMoveStart_H");
			break;
		case PLAYER_ANIMSTATE::PS_MOVE:
			m_pUpperMesh->Enable(true);

			if (bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerLeftUpperMove");
			else
				m_pUpperMesh->ChangeSprite("PlayerLeftUpperMove_H");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALSHOT:
			m_pUpperMesh->Enable(true);

			if(bIsNormal)	
				m_pUpperMesh->ChangeSprite("PlayerLeftNormalMidShot");
			else
				m_pUpperMesh->ChangeSprite("PlayerLeftHeavyMidShot");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALSHOTEND:
			m_pUpperMesh->Enable(true);

			if(bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerLeftNormalMidShotEnd");
			else
				m_pUpperMesh->ChangeSprite("PlayerLeftHeavyMidShotEnd");
			break;
		case PLAYER_ANIMSTATE::PS_AIMUPSTART:
			m_pUpperMesh->Enable(true);

			if(bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerLeftAimUpStart");
			else
			{
				if(m_bIsShooting)
					m_pUpperMesh->ChangeSprite("PlayerLeftAimUpStartHeavyShot");
				else
					m_pUpperMesh->ChangeSprite("PlayerLeftAimUpStart_H");
			}
			break;
		case PLAYER_ANIMSTATE::PS_ANIMUPIDLE:
			m_pUpperMesh->Enable(true);

			if (bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerLeftAimUpIdle");
			else
				m_pUpperMesh->ChangeSprite("PlayerLeftAimUpIdle_H");
			break;
		case PLAYER_ANIMSTATE::PS_AIMDOWN:
			m_pUpperMesh->Enable(true);

			if (bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerLeftAimDown");
			else
			{
				if(m_bIsShooting)
					m_pUpperMesh->ChangeSprite("PlayerLeftAimDownHeavyShot");
				else
					m_pUpperMesh->ChangeSprite("PlayerLeftAimDown_H");
			}
			break;
		case PLAYER_ANIMSTATE::PS_NORMALUPSHOT:
			m_pUpperMesh->Enable(true);

			if(bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerLeftNormalUpShot");
			else
				m_pUpperMesh->ChangeSprite("PlayerLeftHeavyUpShot");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALUPSHOTEND:
			m_pUpperMesh->Enable(true);

			if(bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerLeftNormalUpShotEnd");
			else
				m_pUpperMesh->ChangeSprite("PlayerLeftHeavyUpShotEnd");
			break;
		case PLAYER_ANIMSTATE::PS_SITSTART:
		case PLAYER_ANIMSTATE::PS_SITIDLE:
		case PLAYER_ANIMSTATE::PS_UP:
		case PLAYER_ANIMSTATE::PS_SITSHOT:
			m_pUpperMesh->Enable(false);
			break;
		case PLAYER_ANIMSTATE::PS_THROWBOMB:
			m_pUpperMesh->Enable(true);

			if(bIsNormal)
				m_pUpperMesh->ChangeSprite("PlayerLeftThrowBomb");
			else
				m_pUpperMesh->ChangeSprite("PlayerLeftThrowBomb_H");
			break;
		case PLAYER_ANIMSTATE::PS_SITTHROWBOMB:
			m_pUpperMesh->Enable(false);
			break;
		default:
			break;
		}
	}
}


bool CPlayer::Init()
{
	//�� Component ����
	//---------------------------------
	m_pLowerMesh = CreateComponent<CSpriteComponent>("Lower");
	m_pUpperMesh = CreateComponent<CSpriteComponent>("Upper");
	//m_pBody = CreateComponent<CColliderOBB2D>("Collider");
	//m_pBody = CreateComponent<CColliderRect>("Collider");
	m_pBody = CreateComponent<CColliderSphere2D>("Collider");
	m_pCamera = CreateComponent<CCamera>("Camera");
	//---------------------------------

	//���� ���� Setting
	//---------------------------------
	SetRootComponent(m_pLowerMesh);
	m_pLowerMesh->AddChild(m_pBody);
	m_pLowerMesh->AddChild(m_pUpperMesh);
	m_pLowerMesh->AddChild(m_pCamera);
	//---------------------------------

	//����� Material Setting
	//---------------------------------
	CMaterial* pMaterial = m_pLowerMesh->GetMaterial();

	SAFE_RELEASE(pMaterial);
	//---------------------------------

	//Transform���� Setting
	//---------------------------------
	m_pLowerMesh->SetWorldScale(1.f, 1.f, 1.f);
	m_pLowerMesh->SetScale(2.f);
	m_pLowerMesh->SetRelativePos(0.f, 0.f, 0.f);
	m_pLowerMesh->InitVelocity();
	m_pLowerMesh->SetPivot(0.5f, 0.5f, 0.f);
	m_pLowerMesh->ChangeSprite("PlayerRightLowerIdle");

	m_pUpperMesh->SetInheritScale(false);
	m_pUpperMesh->SetScale(2.f);
	m_pUpperMesh->SetPivot(0.5f, 0.5f, 0.f);
	m_pUpperMesh->SetUseParentZValue(true);
	m_pUpperMesh->SetRender_Priority(Render_Priority::RP_MID);
	m_pUpperMesh->ChangeSprite("PlayerRightUpperIdle");


	m_pCamera->SetCameraType(CAMERA_TYPE::CAM2D);
	m_pCamera->SetInheritScale(false);
	m_pCamera->SetInheritRotX(false);
	m_pCamera->SetInheritRotY(false);
	m_pCamera->SetInheritRotZ(false);
	m_pCamera->SetPivot(0.5f, 0.5f, 0.f);
	m_pCamera->SetRelativePos(0.f, 0.f, -10.f);

	m_pBody->SetInheritScale(false);
	//---------------------------------

	//Sprite���� Setting
	if (!LoadPlayerAnimList())
		return false;


	AddBindAxis("MoveSide", this, &CPlayer::MoveSide);
	AddBindAxis("RotationZ", this, &CPlayer::RotationZ);
	AddBindAxis("Scroll", this, &CPlayer::InventoryScroll);
	
	AddBindAction("Fire", KT_DOWN, this, &CPlayer::Fire);
	AddBindAction("AimUp",KT_DOWN, this, &CPlayer::AimUp);
	AddBindAction("AimUp", KT_UP, this, &CPlayer::AimDown);
	AddBindAction("Down", KT_DOWN, this, &CPlayer::Down);
	AddBindAction("Down", KT_UP, this, &CPlayer::Up);
	AddBindAction("Bomb", KT_DOWN, this, &CPlayer::ThrowBomb);


	return true;
}

void CPlayer::Start()
{
	CGameObject::Start();
}

void CPlayer::Update(float fTime)
{
	CGameObject::Update(fTime);

	UpdateMoveState(fTime);

	//UpdateAttackState(fTime);

	UpdateAnimation(fTime);
}

void CPlayer::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);

	//���� �ӵ� ����
	m_fPrevVelocity = m_pLowerMesh->GetCurrentVelocity();
}

void CPlayer::Collision(float fTime)
{
	CGameObject::Collision(fTime);
}

void CPlayer::PrevRender(float fTime)
{
	CGameObject::PrevRender(fTime);
}

void CPlayer::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void CPlayer::PostRender(float fTime)
{
	CGameObject::PostRender(fTime);
}

void CPlayer::CallAnimation2DNotify(const string& strName)
{
	if (strName == "Attack")
	{
		// �Ѿ��� �߻��Ѵ�.
		CBullet* pBullet = m_pScene->CreateObject<CBullet>("Bullet");

		pBullet->SetRelativeRotation(GetRelativeRot());

		Vector3	vPos = GetWorldPos() + GetWorldAxis(AXIS_Y) *
			((GetWorldScale().y * GetPivot().y) +
				((1.f - pBullet->GetPivot().y) * pBullet->GetWorldScale().y));

		pBullet->SetWorldPos(vPos);

		SAFE_RELEASE(pBullet);
	}

	else if (strName == "MoveStartEnd")
	{
		MoveStartEndProc();
	}

	//else if (strName == "MoveEnd")
	//{
	//	MoveEndProc();
	//}

	else if (strName == "ShotEnd")
	{
		AttackEndProc();
	}

	else if (strName == "GotoIdle")
	{
		GotoIdleProc();
	}

	else if (strName == "AimUp")
	{
		AimUpProc();
	}
}
void CPlayer::AimUp(float fTime)
{
	//aimup���°� �ƴϰ� ���ִ� ��� aimup ���·� Setting
	//-------------------------------------------------
	if (!m_bIsAimUp && m_bIsStand)
	{
		m_bIsAimUp = true;
		SetAimUpState();
	}
	//-------------------------------------------------
}

void CPlayer::AimDown(float fTime)
{
	//aimup�����̰� ���ִ� ��� aimdown ���·� Setting
	//-------------------------------------------------
	if (m_bIsAimUp && m_bIsStand)
	{
		m_bIsAimUp = false;
		SetAimDownState();
	}
	//-------------------------------------------------
}

void CPlayer::MoveSide(float fScale, float fTime)
{
	if (fScale != 0)
	{
		//���� ������ Check
		//---------------------------------------
		if (fScale < 0)
		{
			m_bPrevRightDir = m_bIsRightDir;
			m_bIsRightDir = false;
		}
		else
		{
			m_bPrevRightDir = m_bIsRightDir;
			m_bIsRightDir = true;
		}
		//---------------------------------------

		//�ɾƼ� �ѽ�� ��� & ��ź�� ������ ��� �̵� �Ұ���
		if (!m_bIsStand && (m_bIsShooting || m_eLowerAnimState == PLAYER_ANIMSTATE::PS_SITTHROWBOMB ||
			m_eLowerAnimState == PLAYER_ANIMSTATE::PS_SITSHOTEND))
			return;
		
		AddRelativePos(GetWorldAxis(AXIS_X) * 300.f * fScale * fTime);
	}
}

void CPlayer::RotationZ(float fScale, float fTime)
{
	AddRelativeRotationZ(180.f * fScale * fTime);
}

void CPlayer::Fire(float fTime)
{
	//�̹� ���� ���� ���� pass
	if (m_bIsShooting)
	{
		m_bRecvFireInput = true;
		return;
	}

	m_bRecvFireInput = false;
	m_bIsShooting = true;
	
	//���� �⺻ �������� ��� �⺻���� Setting
	//---------------------------------
	if (m_bIsAimUp)
		SetNormalUpShotState();
	else if (!m_bIsStand)
		SetSitShotState();
	else
		SetNormalMidShotState();
	//---------------------------------


	CBullet* pBullet = m_pScene->CreateObject<CBullet>("Bullet");

	pBullet->SetRelativeRotation(GetRelativeRot());

	Vector3	vPos = GetWorldPos() + GetWorldAxis(AXIS_Y) *
		((GetWorldScale().y * GetPivot().y) +
			((1.f - pBullet->GetPivot().y) * pBullet->GetWorldScale().y));

	pBullet->SetWorldPos(vPos);

	SAFE_RELEASE(pBullet);
}

void CPlayer::AttackEnd()
{
	
}

void CPlayer::Down(float fTime)
{
	//���� �������� �ִϸ��̼��� Jump�� ��쿡�� aim�� �Ʒ�������
	//-----------------------------------------------
	//-----------------------------------------------

	if(!m_bIsStand && m_bRecvDownInput)
		return;

	m_bRecvDownInput = true;
	m_bIsStand = false;
	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_SITSTART;
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_SITSTART;
	//1. ���� ���� �������� ��� 
	if (m_eUpperAnimState == PLAYER_ANIMSTATE::PS_NORMALSHOT)
	{
		return;
		//1-1 ����Ű�� ���� ��� �ɾƼ� �������� ��ȯ
		if (m_bRecvFireInput)
		{
			m_bIsStand = false;
			m_eLowerAnimState = PLAYER_ANIMSTATE::PS_SITSHOT;
			m_eUpperAnimState = PLAYER_ANIMSTATE::PS_SITSHOT;
		}
		//1-2 ����Ű�� �ȴ��� ��� ���� ������ ������ �ɴ� ���·� ��ȯ
		//AttackEnd���� ó��
		else
		{
			return;
		}
	}

	else
	{
		m_bIsStand = false;
		SetSitStartState();
	}
}

void CPlayer::Up(float fTime)
{
	if (m_bIsStand && !m_bRecvDownInput)
		return;

	m_bRecvDownInput = false;

	m_bIsStand = true;
	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_UP;
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_UP;

	//1. �ɾƼ� �������� ���
	if (m_eLowerAnimState == PLAYER_ANIMSTATE::PS_SITSHOT)
	{
		return;

		//1-1 ����Ű�� ���� ��� ���� �������� ��ȯ
		if (m_bRecvFireInput)
		{
			m_bIsStand = true;
			m_eUpperAnimState = PLAYER_ANIMSTATE::PS_NORMALSHOT;
			m_eLowerAnimState = PLAYER_ANIMSTATE::PS_IDLE;
		}

		//1-2 ����Ű�� �ȴ��� ��� �ɾƼ� ������ ������ ���ִ� ���·� ��ȯ
		//AttackEnd���� ó��
		else
		{
			return;
		}
		return;
	}

	else
	{
		m_bIsStand = true;
		
		SetUpStartState();
	}

}

void CPlayer::ThrowBomb(float fTime)
{
	if(m_bIsStand)
		m_eUpperAnimState = PLAYER_ANIMSTATE::PS_THROWBOMB;
	else
	{
		m_eLowerAnimState = PLAYER_ANIMSTATE::PS_SITTHROWBOMB;
		m_eUpperAnimState = PLAYER_ANIMSTATE::PS_SITTHROWBOMB;
	}
}

void CPlayer::InventoryScroll(float fScale, float fTime)
{
	if (fScale != 0)
	{
		if (!m_pInventory)
			return;

		if (fScale < 0.f)
		{
			m_pInventory->ScrollDown();
		}
		else
		{
			m_pInventory->ScrollUp();
		}
	}
}

void CPlayer::MoveStartEndProc()
{
	//MoveStart�ִϸ��̼��� ������ �ش� ���������� �ӵ��� Check�ؼ�
	//MoveEnd�ִϸ��̼��� ������� Move�ִϸ��̼��� ������� ����
	//------------------------------------------------------------

	float fCurVelocity = m_pLowerMesh->GetCurrentVelocity();

	//idle���·� �ٲٱ�
	if (fCurVelocity == 0)
	{
		SetIdleState();
	}

	//�����̴� �ִϸ��̼� ���
	else if (fCurVelocity > 0)
	{
		SetMoveState();
	}

	//------------------------------------------------------------
}

//void CPlayer::MoveEndProc()
//{
//	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_IDLE;
//	if (m_bIsRightDir)
//	{
//		m_pLowerMesh->ChangeSprite("PlayerRightLowerIdle");
//
//		if (m_eUpperAnimState == PLAYER_ANIMSTATE::PS_MOVE)
//		{
//			m_eUpperAnimState = PLAYER_ANIMSTATE::PS_IDLE;
//			m_pUpperMesh->ChangeSprite("PlayerRightUpperIdle");
//		}
//	}
//	else
//	{
//		m_pLowerMesh->ChangeSprite("PlayerLeftLowerIdle");
//
//		if (m_eUpperAnimState == PLAYER_ANIMSTATE::PS_MOVE)
//		{
//			m_eUpperAnimState = PLAYER_ANIMSTATE::PS_IDLE;
//			m_pUpperMesh->ChangeSprite("PlayerLeftUpperIdle");
//		}
//	}
//}

void CPlayer::AttackEndProc()
{
	m_bIsShooting = false;

	//����Ű�� �ȴ������� ���� ��
	//-----------------------------------------------
	if (!m_bRecvFireInput)
	{
		if (m_bIsAimUp)
			SetNormalUpShotEndState();
		//�ɾ��ִ� ���
		else if (!m_bIsStand)
		{
			//�ɴ� Ű�� �� ��� ���� �ִϸ��̼� ���
			if (!m_bRecvDownInput)
			{
				m_bIsStand = true;
				SetUpStartState();
			}
			else
				SetSitShotEndState();
		}
		//���ִ� ���
		else if (m_bIsStand)
		{
			//�ɴ� Ű�� ���� ���� �ɴ� �ִϸ��̼� ���
			if (m_bRecvDownInput)
			{
				m_bIsStand = false;
				SetSitStartState();
			}
			else
				SetNormalMidShotEndState();
		}
		else
			SetNormalMidShotEndState();
	}
	//-----------------------------------------------

	//����Ű�� �������� ��� ����
	//-----------------------------------------------
	else
	{
		m_bIsShooting = true;
		if (m_bIsAimUp)
			SetNormalUpShotState();
		else if (!m_bIsStand)
		{
			SetSitShotState();
		}
		else if (m_bIsStand)
		{
			SetNormalMidShotState();
		}
		else
			SetNormalMidShotEndState();
	}
	//-----------------------------------------------

	m_bRecvFireInput = false;
}

void CPlayer::GotoIdleProc()
{
	m_bIsShooting = false;

	if (m_pLowerMesh->GetCurrentVelocity() > 0)
	{
		if(m_bIsStand)
			SetMoveState();
		else
			SetSitMoveState();
	}
	else
	{
		if (m_bIsAimUp)
			SetAimUpIdleState();
		else if (m_eLowerAnimState == PLAYER_ANIMSTATE::PS_SITSTART || !m_bIsStand)
		{
			SetSitIdleState();
		}
		else
		{
			SetIdleState();
		}
	}
}

void CPlayer::AimUpProc()
{
	//��� ������ ������ AimUpIdle ���·�
	if (m_bIsAimUp)
	{
		SetAimUpIdleState();
	}
	//�߰��� Ű�� ���� AimDown���·�
	else
	{
		SetAimDownState();
	}
}

void CPlayer::SetIdleState()
{
	m_bIsShooting = false;

	//��ü ���´� �ٷ� IDLE
	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_IDLE;

	//��ü�� MOVE������ ��쿡�� IDLE��(�ٸ� ������ ���� �׳� ����)
	if (m_eUpperAnimState == PLAYER_ANIMSTATE::PS_MOVE ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_MOVESTART ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_AIMDOWN ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_NORMALSHOTEND ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_UP ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_THROWBOMB ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_SITTHROWBOMB)
	{
		m_eUpperAnimState = PLAYER_ANIMSTATE::PS_IDLE;
	}
}

void CPlayer::SetMoveStartState()
{
	m_bIsShooting = false;

	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_MOVESTART;

	if (m_eUpperAnimState == PLAYER_ANIMSTATE::PS_IDLE ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_MOVE ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_NORMALSHOTEND ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_UP ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_THROWBOMB ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_SITTHROWBOMB)
	{
		m_eUpperAnimState = PLAYER_ANIMSTATE::PS_MOVESTART;
	}
}

void CPlayer::SetMoveState()
{
	m_bIsShooting = false;

	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_MOVE;

	//���� ��� ��ź�� ������ �� �ٽ� �����̴� ���·� ���ư����ϴ� ��� ȣ��
	if (m_eUpperAnimState == PLAYER_ANIMSTATE::PS_MOVESTART ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_AIMDOWN ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_NORMALSHOTEND||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_UP ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_THROWBOMB ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_SITTHROWBOMB)
		m_eUpperAnimState = PLAYER_ANIMSTATE::PS_MOVE;
}

void CPlayer::SetNormalMidShotState()
{
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_NORMALSHOT;

	float fCurVelocity = m_pLowerMesh->GetCurrentVelocity();

	//���� ���� -> �����̴� ����
	//---------------------------------	
	if (m_fPrevVelocity == 0 && fCurVelocity > 0)
	{
		m_eLowerAnimState = PLAYER_ANIMSTATE::PS_MOVESTART;
	}
	//---------------------------------

	//���� ����
	//---------------------------------
	else if (fCurVelocity == 0)
	{
		m_eLowerAnimState = PLAYER_ANIMSTATE::PS_IDLE;
	}
	//---------------------------------
}

void CPlayer::SetNormalMidShotEndState()
{
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_NORMALSHOTEND;
}

void CPlayer::SetAimUpState()
{
	//�Ϲ� ������ ��쿡�� ������ �����.
	if (m_eCurWeaponState == WEAPON_STATE::WS_NORMAL)
		m_bIsShooting = false;
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_AIMUPSTART;
}

void CPlayer::SetAimUpIdleState()
{
	m_bIsShooting = false;
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_ANIMUPIDLE;
}

void CPlayer::SetAimDownState()
{
	//�Ϲ� ������ ��쿡�� ������ �����.
	if (m_eCurWeaponState == WEAPON_STATE::WS_NORMAL)
		m_bIsShooting = false;
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_AIMDOWN;
}

void CPlayer::SetNormalUpShotState()
{
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_NORMALUPSHOT;
}

void CPlayer::SetNormalUpShotEndState()
{
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_NORMALUPSHOTEND;
}

void CPlayer::SetSitStartState()
{
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_SITSTART;
	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_SITSTART;
}

void CPlayer::SetUpStartState()
{
	m_bIsShooting = false;
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_UP;
	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_UP;
}

void CPlayer::SetSitIdleState()
{
	m_bIsShooting = false;
	
	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_SITIDLE;
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_SITIDLE;
}

void CPlayer::SetSitMoveState()
{
	m_bIsShooting = false;

	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_SITMOVE;
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_SITMOVE;
}

void CPlayer::SetSitShotState()
{
	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_SITSHOT;
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_SITSHOT;
}

void CPlayer::SetSitShotEndState()
{
	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_SITSHOTEND;
}

void CPlayer::Save(FILE* pFile)
{
	CGameObject::Save(pFile);

	//LowerComponent ����
	//----------------------------
	int iLen = (int)m_pLowerMesh->GetName().length();
	fwrite(&iLen, sizeof(iLen), 1, pFile);
	fwrite(m_pLowerMesh->GetName().c_str(), 1, iLen, pFile);

	//----------------------------

	//UpperComponent ����
	//----------------------------
	iLen = (int)m_pUpperMesh->GetName().length();
	fwrite(&iLen, sizeof(iLen), 1, pFile);
	fwrite(m_pUpperMesh->GetName().c_str(), 1, iLen, pFile);

	//----------------------------

	//CameraComponent ����
	//----------------------------
	iLen = (int)m_pCamera->GetName().length();
	fwrite(&iLen, sizeof(iLen), 1, pFile);
	fwrite(m_pCamera->GetName().c_str(), 1, iLen, pFile);

	//----------------------------

	//Collider ����
	//----------------------------
	iLen = (int)m_pBody->GetName().length();
	fwrite(&iLen, sizeof(iLen), 1, pFile);
	fwrite(m_pBody->GetName().c_str(), 1, iLen, pFile);

	//----------------------------
}

void CPlayer::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	//LowerComponent Load
	//----------------------------
	int iLen = 0;
	char strName[256] = {};
	fread(&iLen, sizeof(iLen), 1, pFile);
	fread(strName, 1, iLen, pFile);

	m_pLowerMesh = (CSpriteComponent*)m_pRootComponent->FindComponent(strName);
	//----------------------------

	//UpperComponent ����
	//----------------------------
	iLen = 0;
	memset(strName,0,256);
	fread(&iLen, sizeof(iLen), 1, pFile);
	fread(strName, 1, iLen, pFile);

	m_pUpperMesh = (CSpriteComponent*)m_pRootComponent->FindComponent(strName);
	//----------------------------

	//CameraComponent ����
	//----------------------------
	iLen = 0;
	memset(strName, 0, 256);
	fread(&iLen, sizeof(iLen), 1, pFile);
	fread(strName, 1, iLen, pFile);

	m_pCamera = (CCamera*)m_pRootComponent->FindComponent(strName);

	//----------------------------

	//Collider ����
	//----------------------------
	iLen = 0;
	memset(strName, 0, 256);
	fread(&iLen, sizeof(iLen), 1, pFile);
	fread(strName, 1, iLen, pFile);

	//m_pBody = (CColliderOBB2D*)m_pRootComponent->FindComponent(strName);

	//----------------------------

	//Sprite���� Setting
	if (!LoadPlayerAnimList())
		return;

	AddBindAxis("MoveSide", this, &CPlayer::MoveSide);
	AddBindAxis("RotationZ", this, &CPlayer::RotationZ);
	AddBindAction("Fire", KT_DOWN, this, &CPlayer::Fire);
	AddBindAction("AimUp", KT_DOWN, this, &CPlayer::AimUp);
	AddBindAction("AimUp", KT_UP, this, &CPlayer::AimDown);
	AddBindAction("Down",KT_DOWN, this, &CPlayer::Down);
	AddBindAction("Down", KT_UP, this, &CPlayer::Up);
	AddBindAction("Bomb", KT_DOWN, this, &CPlayer::ThrowBomb);
}

void CPlayer::CollisionBegin(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CPlayer::CollisionEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
}
