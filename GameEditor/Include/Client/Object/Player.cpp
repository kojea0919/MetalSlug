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


CPlayer::CPlayer()
	:m_pLowerMesh(nullptr), m_pUpperMesh(nullptr),
	m_pCamera(nullptr), m_pBody(nullptr), m_fColorChangeTime(1.f),
	m_fColorChangeTimer(0.f), m_fPrevVelocity(0.f),
	m_bIsRightDir(true), m_bPrevRightDir(true),
	m_bIsShooting(false), m_bRecvFireInput(false),
	m_bIsAimUp(false),m_eUpperAnimState(PLAYER_ANIMSTATE::PS_IDLE),
	m_eLowerAnimState(PLAYER_ANIMSTATE::PS_IDLE),
	m_bIsStand(true),m_bIsCanFire(true)
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

	//AnimList파일 경로 Setting
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

	//애니메이션 수 Setting
	//-------------------
	int iSize = 0;
	fscanf_s(pFile, "%d\n", &iSize);
	//-------------------

	//애니메이션 Setting
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

	//애니메이션 수 Setting
	//-------------------
	iSize = 0;
	fscanf_s(pFile, "%d\n", &iSize);
	//-------------------

	//애니메이션 Setting
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
	

	//멈춤 상태 -> 움직이는 상태
	//---------------------------------	
	if (m_fPrevVelocity == 0 && fCurVelocity > 0)
	{
		//서있는 경우는 기본 움직임
		if(m_bIsStand)
			SetMoveStartState();
		//앉아있는 경우는 앉은 움직임
		else
			SetSitMoveState();

	}
	//---------------------------------

	//멈춤 상태
	//---------------------------------
	else if (m_fPrevVelocity > 0 && fCurVelocity == 0)
	{
		//서있는 경우
		if (m_bIsStand)
			SetIdleState();
		//앉아있는 경우
		else
			SetSitIdleState();
	}
	//---------------------------------
}

//void CPlayer::UpdateAttackState(float fTime)
//{
//	//현재 기본 공격중인 경우 기본공격 Setting
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
			m_pLowerMesh->ChangeSprite("PlayerRightSitStart");
			break;
		case PLAYER_ANIMSTATE::PS_SITIDLE:
			m_pLowerMesh->ChangeSprite("PlayerRightSitIdle");
			break;
		case PLAYER_ANIMSTATE::PS_UP:
			m_pLowerMesh->ChangeSprite("PlayerRightUpStart");
			break;
		case PLAYER_ANIMSTATE::PS_SITMOVE:
			m_pLowerMesh->ChangeSprite("PlayerRightSitMove");
			break;
		case PLAYER_ANIMSTATE::PS_SITSHOT:
			m_pLowerMesh->ChangeSprite("PlayerRightSitShot");
			break;
		case PLAYER_ANIMSTATE::PS_SITSHOTEND:
			m_pLowerMesh->ChangeSprite("PlayerRightSitShotEnd");
			break;
		default:
			break;
		}

		switch (m_eUpperAnimState)
		{
		case PLAYER_ANIMSTATE::PS_IDLE:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerRightUpperIdle");
			break;
		case PLAYER_ANIMSTATE::PS_MOVESTART:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerRightUpperMoveStart");
			break;
		case PLAYER_ANIMSTATE::PS_MOVE:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerRightUpperMove");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALSHOT:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerRightNormalMidShot");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALSHOTEND:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerRightNormalMidShotEnd");
			break;
		case PLAYER_ANIMSTATE::PS_AIMUPSTART:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerRightAimUpStart");
			break;
		case PLAYER_ANIMSTATE::PS_ANIMUPIDLE:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerRightAimUpIdle");
			break;
		case PLAYER_ANIMSTATE::PS_AIMDOWN:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerRightAimDown");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALUPSHOT:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerRightNormalUpShot");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALUPSHOTEND:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerRightNormalUpShotEnd");
			break;
		case PLAYER_ANIMSTATE::PS_SITSTART:
		case PLAYER_ANIMSTATE::PS_SITIDLE:
		case PLAYER_ANIMSTATE::PS_UP:
		case PLAYER_ANIMSTATE::PS_SITSHOT:
		case PLAYER_ANIMSTATE::PS_SITSHOTEND:
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
			m_pLowerMesh->ChangeSprite("PlayerLeftSitStart");
			break;
		case PLAYER_ANIMSTATE::PS_SITIDLE:
			m_pLowerMesh->ChangeSprite("PlayerLeftSitIdle");
			break;
		case PLAYER_ANIMSTATE::PS_UP:
			m_pLowerMesh->ChangeSprite("PlayerLeftUpStart");
			break;
		case PLAYER_ANIMSTATE::PS_SITMOVE:
			m_pLowerMesh->ChangeSprite("PlayerLeftSitMove");
			break;
		case PLAYER_ANIMSTATE::PS_SITSHOT:
			m_pLowerMesh->ChangeSprite("PlayerLeftSitShot");
			break;
		case PLAYER_ANIMSTATE::PS_SITSHOTEND:
			m_pLowerMesh->ChangeSprite("PlayerLeftSitShotEnd");
			break;
		default:
			break;
		}

		switch (m_eUpperAnimState)
		{
		case PLAYER_ANIMSTATE::PS_IDLE:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerLeftUpperIdle");
			break;
		case PLAYER_ANIMSTATE::PS_MOVESTART:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerLeftUpperMoveStart");
			break;
		case PLAYER_ANIMSTATE::PS_MOVE:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerLeftUpperMove");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALSHOT:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerLeftNormalMidShot");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALSHOTEND:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerLeftNormalMidShotEnd");
			break;
		case PLAYER_ANIMSTATE::PS_AIMUPSTART:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerLeftAimUpStart");
			break;
		case PLAYER_ANIMSTATE::PS_ANIMUPIDLE:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerLeftAimUpIdle");
			break;
		case PLAYER_ANIMSTATE::PS_AIMDOWN:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerLeftAimDown");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALUPSHOT:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerLeftNormalUpShot");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALUPSHOTEND:
			m_pUpperMesh->Enable(true);
			m_pUpperMesh->ChangeSprite("PlayerLeftNormalUpShotEnd");
			break;
		case PLAYER_ANIMSTATE::PS_SITSTART:
		case PLAYER_ANIMSTATE::PS_SITIDLE:
		case PLAYER_ANIMSTATE::PS_UP:
		case PLAYER_ANIMSTATE::PS_SITSHOT:
			m_pUpperMesh->Enable(false);
			break;
		default:
			break;
		}
	}
}


bool CPlayer::Init()
{
	//각 Component 생성
	//---------------------------------
	m_pLowerMesh = CreateComponent<CSpriteComponent>("Lower");
	m_pUpperMesh = CreateComponent<CSpriteComponent>("Upper");
	//m_pBody = CreateComponent<CColliderOBB2D>("Collider");
	//m_pBody = CreateComponent<CColliderRect>("Collider");
	m_pBody = CreateComponent<CColliderSphere2D>("Collider");
	m_pCamera = CreateComponent<CCamera>("Camera");
	//---------------------------------

	//계층 관계 Setting
	//---------------------------------
	SetRootComponent(m_pLowerMesh);
	m_pLowerMesh->AddChild(m_pBody);
	m_pLowerMesh->AddChild(m_pUpperMesh);
	m_pLowerMesh->AddChild(m_pCamera);
	//---------------------------------

	//사용할 Material Setting
	//---------------------------------
	CMaterial* pMaterial = m_pLowerMesh->GetMaterial();

	SAFE_RELEASE(pMaterial);
	//---------------------------------

	//Transform정보 Setting
	//---------------------------------
	m_pLowerMesh->SetWorldScale(1.f, 1.f, 1.f);
	m_pLowerMesh->SetScale(2.f);
	m_pLowerMesh->SetRelativePos(500.f, 300.f, 0.f);
	m_pLowerMesh->InitVelocity();
	m_pLowerMesh->SetPivot(0.5f, 0.5f, 0.f);
	m_pLowerMesh->ChangeSprite("PlayerRightLowerIdle");

	m_pUpperMesh->SetInheritScale(false);
	m_pUpperMesh->SetScale(2.f);
	m_pUpperMesh->SetPivot(0.5f, 0.5f, 0.f);
	m_pUpperMesh->SetRender_Priority(Render_Priority::RP_MID);
	m_pUpperMesh->ChangeSprite("PlayerRightUpperIdle");

	m_pCamera->SetCameraType(CAMERA_TYPE::CAM2D);
	m_pCamera->SetInheritScale(false);
	m_pCamera->SetInheritRotX(false);
	m_pCamera->SetInheritRotY(false);
	m_pCamera->SetInheritRotZ(false);
	m_pCamera->SetPivot(0.7f, 0.5f, 0.f);

	m_pBody->SetInheritScale(false);
	//---------------------------------

	//Sprite정보 Setting
	if (!LoadPlayerAnimList())
		return false;


	AddBindAxis("MoveSide", this, &CPlayer::MoveSide);
	AddBindAxis("RotationZ", this, &CPlayer::RotationZ);
	AddBindAction("Fire", KT_DOWN, this, &CPlayer::Fire);
	AddBindAction("AimUp",KT_DOWN, this, &CPlayer::AimUp);
	AddBindAction("AimUp", KT_UP, this, &CPlayer::AimDown);
	AddBindAction("Down", KT_DOWN, this, &CPlayer::Down);
	AddBindAction("Down", KT_UP, this, &CPlayer::Up);

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

	//현재 속도 저장
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
		// 총알을 발사한다.
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
	//aimup상태가 아니고 서있는 경우 aimup 상태로 Setting
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
	//aimup상태이고 서있는 경우 aimdown 상태로 Setting
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
		//앉아서 총쏘는 경우는 이동 불가능
		if (!m_bIsStand && m_bIsShooting)
			return;

		//왼쪽 오른쪽 Check
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


		AddRelativePos(GetWorldAxis(AXIS_X) * 300.f * fScale * fTime);
	}
}

void CPlayer::RotationZ(float fScale, float fTime)
{
	AddRelativeRotationZ(180.f * fScale * fTime);
}

void CPlayer::Fire(float fTime)
{
	/*if (m_bIsCanFire)
	{*/
	
	//이미 공격 중인 경우는 pass
	if (m_bIsShooting)
	{
		m_bRecvFireInput = true;
		return;
	}

	m_bRecvFireInput = false;
	m_bIsShooting = true;
	
	//현재 기본 공격중인 경우 기본공격 Setting
	//---------------------------------
	if (m_bIsAimUp)
		SetNormalUpShotState();
	else if (!m_bIsStand)
		SetSitShotState();
	else
		SetNormalMidShotState();
	//---------------------------------


	//}	
}

void CPlayer::AttackEnd()
{
	
}

void CPlayer::Down(float fTime)
{
	//현재 동작중인 애니메이션이 Jump인 경우에는 aim을 아래쪽으로
	//-----------------------------------------------
	//-----------------------------------------------

	if(!m_bIsStand)
		return;

	//1. 현재 서서 공격중인 경우 
	if (m_eUpperAnimState == PLAYER_ANIMSTATE::PS_NORMALSHOT)
	{
		//1-1 공격키가 눌린 경우 앉아서 공격으로 전환
		if (m_bRecvFireInput)
		{
			m_bIsStand = false;
			m_eLowerAnimState = PLAYER_ANIMSTATE::PS_SITSHOT;
			m_eUpperAnimState = PLAYER_ANIMSTATE::PS_SITSHOT;
		}
		//1-2 공격키가 안눌린 경우 서서 공격이 끝나면 앉는 상태로 전환
		//AttackEnd에서 처리
		else
		{
			return;
		}
	}

	else
	{
		m_bIsStand = false;
		m_eLowerAnimState = PLAYER_ANIMSTATE::PS_SITSTART;
		m_eUpperAnimState = PLAYER_ANIMSTATE::PS_SITSTART;
	}
}

void CPlayer::Up(float fTime)
{
	if (m_bIsStand)
		return;

	m_bIsStand = true;

	//1. 앉아서 공격중인 경우
	if (m_eLowerAnimState == PLAYER_ANIMSTATE::PS_SITSHOT)
	{
		//1-1 공격키가 눌린 경우 서서 공격으로 전환
		/*if (m_bRecvFireInput && m_eLowerAnimState == PLAYER_ANIMSTATE::PS_SITSHOTEND)
		{
			m_eUpperAnimState = PLAYER_ANIMSTATE::PS_NORMALSHOT;
			m_eLowerAnimState = PLAYER_ANIMSTATE::PS_IDLE;
		}*/

		//1-2 공격키가 안눌린 경우 앉아서 공격이 끝나면 서있는 상태로 전환
		//AttackEnd에서 처리
		/*else
		{
			return;
		}*/
		return;
	}

	else
	{
		m_eLowerAnimState = PLAYER_ANIMSTATE::PS_UP;
		m_eUpperAnimState = PLAYER_ANIMSTATE::PS_UP;
	}

}

void CPlayer::MoveStartEndProc()
{
	//MoveStart애니메이션이 끝나면 해당 시점에서의 속도를 Check해서
	//MoveEnd애니메이션을 재생할지 Move애니메이션을 재생할지 결정
	//------------------------------------------------------------

	float fCurVelocity = m_pLowerMesh->GetCurrentVelocity();

	//idle상태로 바꾸기
	if (fCurVelocity == 0)
	{
		SetIdleState();
	}

	//움직이는 애니메이션 재생
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

	//공격키를 안눌렀으면 공격 끝
	//-----------------------------------------------
	if (!m_bRecvFireInput)
	{
		if (m_bIsAimUp)
			SetNormalUpShotEndState();
		else if (!m_bIsStand)
		{
			//서있는 상태에서 앉는 상태로 바뀐경우 
			if (m_eUpperAnimState == PLAYER_ANIMSTATE::PS_NORMALSHOT)
				SetSitStartState();
			//앉아있던 상태에서 공격이 끝난 경우
			else
				SetSitShotEndState();
		}
		else if (m_bIsStand)
		{
			//앉아있는 상태에서 서있는 상태로 바뀐경우
			if (m_eUpperAnimState == PLAYER_ANIMSTATE::PS_SITSHOT)
				SetUpStartState();
			//서있던 상태에서 공격이 끝난 경우
			else
				SetNormalMidShotEndState();
		}
		else
			SetNormalMidShotEndState();
	}
	//-----------------------------------------------

	//공격키를 눌렀으면 계속 공격
	//-----------------------------------------------
	else
	{
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
	m_bIsCanFire = true;

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
		else if (m_eLowerAnimState == PLAYER_ANIMSTATE::PS_SITSTART)
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
	//계속 누르고 있으면 AimUpIdle 상태로
	if (m_bIsAimUp)
	{
		SetAimUpIdleState();
	}
	//중간에 키를 떼면 AimDown상태로
	else
	{
		SetAimDownState();
	}
}

void CPlayer::SetIdleState()
{
	//하체 상태는 바로 IDLE
	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_IDLE;

	//상체는 MOVE상태인 경우에만 IDLE로(다른 상태인 경우는 그냥 유지)
	if (m_eUpperAnimState == PLAYER_ANIMSTATE::PS_MOVE ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_AIMDOWN ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_NORMALSHOTEND ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_UP)
	{
		m_eUpperAnimState = PLAYER_ANIMSTATE::PS_IDLE;
	}
}

void CPlayer::SetMoveStartState()
{
	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_MOVESTART;

	if (m_eUpperAnimState == PLAYER_ANIMSTATE::PS_IDLE ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_MOVE ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_NORMALSHOTEND ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_UP)
	{
		m_eUpperAnimState = PLAYER_ANIMSTATE::PS_MOVESTART;
	}
}

void CPlayer::SetMoveState()
{
	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_MOVE;

	//총을 쏘던 폭탄을 던지던 중 다시 움직이느 상태로 돌아가야하는 경우 호출
	if (m_eUpperAnimState == PLAYER_ANIMSTATE::PS_MOVESTART ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_AIMDOWN ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_NORMALSHOTEND||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_UP)
		m_eUpperAnimState = PLAYER_ANIMSTATE::PS_MOVE;
}

void CPlayer::SetNormalMidShotState()
{
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_NORMALSHOT;

	float fCurVelocity = m_pLowerMesh->GetCurrentVelocity();

	//멈춤 상태 -> 움직이는 상태
	//---------------------------------	
	if (m_fPrevVelocity == 0 && fCurVelocity > 0)
	{
		m_eLowerAnimState = PLAYER_ANIMSTATE::PS_MOVESTART;
	}
	//---------------------------------

	//멈춤 상태
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
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_AIMUPSTART;
}

void CPlayer::SetAimUpIdleState()
{
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_ANIMUPIDLE;
}

void CPlayer::SetAimDownState()
{
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
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_UP;
	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_UP;
}

void CPlayer::SetSitIdleState()
{
	//하체 상태는 바로 IDLE
	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_SITIDLE;
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_SITIDLE;
}

void CPlayer::SetSitMoveState()
{
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

	//LowerComponent 저장
	//----------------------------
	int iLen = (int)m_pLowerMesh->GetName().length();
	fwrite(&iLen, sizeof(iLen), 1, pFile);
	fwrite(m_pLowerMesh->GetName().c_str(), 1, iLen, pFile);

	//----------------------------

	//UpperComponent 저장
	//----------------------------
	iLen = (int)m_pUpperMesh->GetName().length();
	fwrite(&iLen, sizeof(iLen), 1, pFile);
	fwrite(m_pUpperMesh->GetName().c_str(), 1, iLen, pFile);

	//----------------------------

	//CameraComponent 저장
	//----------------------------
	iLen = (int)m_pCamera->GetName().length();
	fwrite(&iLen, sizeof(iLen), 1, pFile);
	fwrite(m_pCamera->GetName().c_str(), 1, iLen, pFile);

	//----------------------------

	//Collider 저장
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

	//UpperComponent 저장
	//----------------------------
	iLen = 0;
	memset(strName,0,256);
	fread(&iLen, sizeof(iLen), 1, pFile);
	fread(strName, 1, iLen, pFile);

	m_pUpperMesh = (CSpriteComponent*)m_pRootComponent->FindComponent(strName);
	//----------------------------

	//CameraComponent 저장
	//----------------------------
	iLen = 0;
	memset(strName, 0, 256);
	fread(&iLen, sizeof(iLen), 1, pFile);
	fread(strName, 1, iLen, pFile);

	m_pCamera = (CCamera*)m_pRootComponent->FindComponent(strName);

	//----------------------------

	//Collider 저장
	//----------------------------
	iLen = 0;
	memset(strName, 0, 256);
	fread(&iLen, sizeof(iLen), 1, pFile);
	fread(strName, 1, iLen, pFile);

	//m_pBody = (CColliderOBB2D*)m_pRootComponent->FindComponent(strName);

	//----------------------------

	//Sprite정보 Setting
	if (!LoadPlayerAnimList())
		return;

	AddBindAxis("MoveSide", this, &CPlayer::MoveSide);
	AddBindAxis("RotationZ", this, &CPlayer::RotationZ);
	AddBindAction("Fire", KT_DOWN, this, &CPlayer::Fire);
	AddBindAction("AimUp", KT_DOWN, this, &CPlayer::AimUp);
	AddBindAction("AimUp", KT_UP, this, &CPlayer::AimDown);
	AddBindAction("Down",KT_DOWN, this, &CPlayer::Down);
	AddBindAction("Down", KT_UP, this, &CPlayer::Up);
}

void CPlayer::CollisionBegin(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CPlayer::CollisionEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
}
