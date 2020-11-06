#pragma once

#include "GameObject.h"

enum class PLAYER_ANIMSTATE
{
	PS_IDLE,
	PS_MOVESTART,
	PS_MOVE,
	PS_NORMALSHOT,
	PS_NORMALSHOTEND,
	PS_AIMUPSTART,
	PS_ANIMUPIDLE,
	PS_AIMDOWN,
	PS_NORMALUPSHOT,
	PS_NORMALUPSHOTEND
};

class CPlayer :
	public CGameObject
{
public:
	CPlayer();
	CPlayer(const CPlayer& player);
	virtual ~CPlayer();

protected:
	//하체
	class CSpriteComponent* m_pLowerMesh;

	//상체
	class CSpriteComponent* m_pUpperMesh;

	class CCamera* m_pCamera;

	//class CColliderOBB2D* m_pBody;
	//class CColliderRect* m_pBody;
	class CColliderSphere2D* m_pBody;

	float			m_fColorChangeTime;
	float			m_fColorChangeTimer;

	//이전 프레임에서의 속도(Move Start 애니메이션을 위해서 필요)
	float			m_fPrevVelocity;

	//플레이어가 바라보는 방향
	//-------------------------------
	bool			m_bIsRightDir;
	bool			m_bPrevRightDir;
	//-------------------------------

	//플레이어의 공격 여부
	//-------------------------------
	
	//현재 공격 중인지 나타내는 bool변수
	bool			m_bIsShooting;

	//공격 애니메이션이 재생중인 상태에서 또 
	//공격 버튼이 눌렸는지 나타내는 bool변수
	//(연속으로 발사하는데 사용)
	bool			m_bRecvFireInput;
	//-------------------------------

	//플레이어 Aim의 방향이 Up인지
	bool			m_bIsAimUp;

	//플레이어 현재 실행중인 애니메이션
	//-------------------------------
	PLAYER_ANIMSTATE	m_eUpperAnimState;
	PLAYER_ANIMSTATE	m_eLowerAnimState;
	//-------------------------------



protected:
	//PlayerAnimList파일을 읽어서 플레이어에서 사용하는 애니메이션 Setting
	bool LoadPlayerAnimList();

protected:
	//움직이는 State Update
	void UpdateMoveState(float fTime);

	//공격 State Update
	void UpdateAttackState(float fTime);

	//State에 따른 Animation Update
	void UpdateAnimation(float fTime);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual void CallAnimation2DNotify(const string& strName);

protected:
	void MoveSide(float fScale, float fTime);
	void RotationZ(float fScale, float fTime);

	void AimUp(float fTime);
	void AimDown(float fTime);
	void Fire(float fTime);
	void AttackEnd();

protected:
	//Animation Notify Proc
	//---------------------------------

	//Player를 처음 움직일 때 애니메이션이 끝난 경우 호출
	void MoveStartEndProc();

	//Player가 멈추는 애니메이션이 끝난 경우 호출
	//void MoveEndProc();

	//Player가 공격하는 애니메이션이 끝난 경우 호출
	void AttackEndProc();

	//Player가 Idle상태로 돌아가야하는 경우 호출
	void GotoIdleProc();

	//Player가 Aim방향이 위쪽으로 바뀌어야하는 경우 호출
	void AimUpProc();

	//---------------------------------

	//State Set함수
	//---------------------------------
	void SetIdleState();
	void SetMoveStartState();
	void SetMoveState();
	void SetNormalMidShotState();
	void SetNormalMidShotEndState();
	void SetAimUpState();
	void SetAimUpIdleState();
	void SetAimDownState();
	void SetNormalUpShotState();
	void SetNormalUpShotEndState();
	//---------------------------------
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void CollisionBegin(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void CollisionEnd(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

