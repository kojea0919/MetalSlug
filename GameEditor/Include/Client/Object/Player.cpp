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
	m_eLowerAnimState(PLAYER_ANIMSTATE::PS_IDLE)
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
		SetMoveStartState();
	}
	//---------------------------------

	//�����̴� ���� -> ���� ����
	//---------------------------------
	else if (m_fPrevVelocity > 0 && fCurVelocity == 0)
	{
		SetIdleState();
	}
	//---------------------------------
}

void CPlayer::UpdateAttackState(float fTime)
{
	//���� �⺻ �������� ��� �⺻���� Setting
	//---------------------------------
	if (m_bIsShooting)
	{
		if(m_bIsAimUp)
			SetNormalUpShotState();
		else
			SetNormalMidShotState();
	}
	//---------------------------------
}

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
		default:
			break;
		}

		switch (m_eUpperAnimState)
		{
		case PLAYER_ANIMSTATE::PS_IDLE:
			m_pUpperMesh->ChangeSprite("PlayerRightUpperIdle");
			break;
		case PLAYER_ANIMSTATE::PS_MOVESTART:
			m_pUpperMesh->ChangeSprite("PlayerRightUpperMoveStart");
			break;
		case PLAYER_ANIMSTATE::PS_MOVE:
			m_pUpperMesh->ChangeSprite("PlayerRightUpperMove");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALSHOT:
			m_pUpperMesh->ChangeSprite("PlayerRightNormalMidShot");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALSHOTEND:
			m_pUpperMesh->ChangeSprite("PlayerRightNormalMidShotEnd");
			break;
		case PLAYER_ANIMSTATE::PS_AIMUPSTART:
			m_pUpperMesh->ChangeSprite("PlayerRightAimUpStart");
			break;
		case PLAYER_ANIMSTATE::PS_ANIMUPIDLE:
			m_pUpperMesh->ChangeSprite("PlayerRightAimUpIdle");
			break;
		case PLAYER_ANIMSTATE::PS_AIMDOWN:
			m_pUpperMesh->ChangeSprite("PlayerRightAimDown");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALUPSHOT:
			m_pUpperMesh->ChangeSprite("PlayerRightNormalUpShot");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALUPSHOTEND:
			m_pUpperMesh->ChangeSprite("PlayerRightNormalUpShotEnd");
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
		default:
			break;
		}

		switch (m_eUpperAnimState)
		{
		case PLAYER_ANIMSTATE::PS_IDLE:
			m_pUpperMesh->ChangeSprite("PlayerLeftUpperIdle");
			break;
		case PLAYER_ANIMSTATE::PS_MOVESTART:
			m_pUpperMesh->ChangeSprite("PlayerLeftUpperMoveStart");
			break;
		case PLAYER_ANIMSTATE::PS_MOVE:
			m_pUpperMesh->ChangeSprite("PlayerLeftUpperMove");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALSHOT:
			m_pUpperMesh->ChangeSprite("PlayerLeftNormalMidShot");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALSHOTEND:
			m_pUpperMesh->ChangeSprite("PlayerLeftNormalMidShotEnd");
			break;
		case PLAYER_ANIMSTATE::PS_AIMUPSTART:
			m_pUpperMesh->ChangeSprite("PlayerLeftAimUpStart");
			break;
		case PLAYER_ANIMSTATE::PS_ANIMUPIDLE:
			m_pUpperMesh->ChangeSprite("PlayerLeftAimUpIdle");
			break;
		case PLAYER_ANIMSTATE::PS_AIMDOWN:
			m_pUpperMesh->ChangeSprite("PlayerLeftAimDown");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALUPSHOT:
			m_pUpperMesh->ChangeSprite("PlayerLeftNormalUpShot");
			break;
		case PLAYER_ANIMSTATE::PS_NORMALUPSHOTEND:
			m_pUpperMesh->ChangeSprite("PlayerLeftNormalUpShotEnd");
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

	//Sprite���� Setting��
	if (!LoadPlayerAnimList())
		return false;


	AddBindAxis("MoveSide", this, &CPlayer::MoveSide);
	AddBindAxis("RotationZ", this, &CPlayer::RotationZ);
	AddBindAction("Fire", KT_DOWN, this, &CPlayer::Fire);
	AddBindAction("AimUp",KT_DOWN, this, &CPlayer::AimUp);
	AddBindAction("AimUp", KT_UP, this, &CPlayer::AimDown);

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

	UpdateAttackState(fTime);

	UpdateAnimation(fTime);
}

void CPlayer::PostUpdate(float fTime)
{
	CGameObject::PostUpdate(fTime);

	//���� �ӵ� ����
	m_fPrevVelocity = m_pLowerMesh->GetCurrentVelocity();

	//fire��ư �Է� ���� �ʱ�ȭ
	m_bRecvFireInput = false;
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
	if (!m_bIsAimUp)
	{
		m_bIsAimUp = true;
		SetAimUpState();
	}
}

void CPlayer::AimDown(float fTime)
{
	if (m_bIsAimUp)
	{
		m_bIsAimUp = false;
		SetAimDownState();
	}
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


		AddRelativePos(GetWorldAxis(AXIS_X) * 300.f * fScale * fTime);
	}
}

void CPlayer::RotationZ(float fScale, float fTime)
{
	AddRelativeRotationZ(180.f * fScale * fTime);
}

void CPlayer::Fire(float fTime)
{
	m_bIsShooting = true;
	m_bRecvFireInput = true;
}

void CPlayer::AttackEnd()
{
	
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
	//����Ű�� �ȴ������� ���� ��
	//-----------------------------------------------
	if(!m_bRecvFireInput)
	{
		m_bIsShooting = false;
		if(m_bIsAimUp)
			SetNormalUpShotEndState();
		else
			SetNormalMidShotEndState();
	}
	//-----------------------------------------------
}

void CPlayer::GotoIdleProc()
{
	if (m_pLowerMesh->GetCurrentVelocity() > 0)
	{
		SetMoveState();
	}
	else
	{
		if (m_bIsAimUp)
			SetAimUpIdleState();
		else
			SetIdleState();
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
	//��ü ���´� �ٷ� IDLE
	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_IDLE;

	//��ü�� MOVE������ ��쿡�� IDLE��(�ٸ� ������ ���� �׳� ����)
	if (m_eUpperAnimState == PLAYER_ANIMSTATE::PS_MOVE ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_AIMDOWN ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_NORMALSHOTEND)
	{
		m_eUpperAnimState = PLAYER_ANIMSTATE::PS_IDLE;
	}
}

void CPlayer::SetMoveStartState()
{
	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_MOVESTART;

	if (m_eUpperAnimState == PLAYER_ANIMSTATE::PS_IDLE ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_MOVE ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_NORMALSHOTEND)
	{
		m_eUpperAnimState = PLAYER_ANIMSTATE::PS_MOVESTART;
	}
}

void CPlayer::SetMoveState()
{
	m_eLowerAnimState = PLAYER_ANIMSTATE::PS_MOVE;

	if (m_eUpperAnimState == PLAYER_ANIMSTATE::PS_MOVESTART ||
		m_eUpperAnimState == PLAYER_ANIMSTATE::PS_AIMDOWN)
		m_eUpperAnimState = PLAYER_ANIMSTATE::PS_MOVE;
}

void CPlayer::SetNormalMidShotState()
{
	m_eUpperAnimState = PLAYER_ANIMSTATE::PS_NORMALSHOT;
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
	AddBindAction("AimUp",KT_UP, this, &CPlayer::AimDown);
}

void CPlayer::CollisionBegin(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CPlayer::CollisionEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
}
