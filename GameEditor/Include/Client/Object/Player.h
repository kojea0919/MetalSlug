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
	//��ü
	class CSpriteComponent* m_pLowerMesh;

	//��ü
	class CSpriteComponent* m_pUpperMesh;

	class CCamera* m_pCamera;

	//class CColliderOBB2D* m_pBody;
	//class CColliderRect* m_pBody;
	class CColliderSphere2D* m_pBody;

	float			m_fColorChangeTime;
	float			m_fColorChangeTimer;

	//���� �����ӿ����� �ӵ�(Move Start �ִϸ��̼��� ���ؼ� �ʿ�)
	float			m_fPrevVelocity;

	//�÷��̾ �ٶ󺸴� ����
	//-------------------------------
	bool			m_bIsRightDir;
	bool			m_bPrevRightDir;
	//-------------------------------

	//�÷��̾��� ���� ����
	//-------------------------------
	
	//���� ���� ������ ��Ÿ���� bool����
	bool			m_bIsShooting;

	//���� �ִϸ��̼��� ������� ���¿��� �� 
	//���� ��ư�� ���ȴ��� ��Ÿ���� bool����
	//(�������� �߻��ϴµ� ���)
	bool			m_bRecvFireInput;
	//-------------------------------

	//�÷��̾� Aim�� ������ Up����
	bool			m_bIsAimUp;

	//�÷��̾� ���� �������� �ִϸ��̼�
	//-------------------------------
	PLAYER_ANIMSTATE	m_eUpperAnimState;
	PLAYER_ANIMSTATE	m_eLowerAnimState;
	//-------------------------------



protected:
	//PlayerAnimList������ �о �÷��̾�� ����ϴ� �ִϸ��̼� Setting
	bool LoadPlayerAnimList();

protected:
	//�����̴� State Update
	void UpdateMoveState(float fTime);

	//���� State Update
	void UpdateAttackState(float fTime);

	//State�� ���� Animation Update
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

	//Player�� ó�� ������ �� �ִϸ��̼��� ���� ��� ȣ��
	void MoveStartEndProc();

	//Player�� ���ߴ� �ִϸ��̼��� ���� ��� ȣ��
	//void MoveEndProc();

	//Player�� �����ϴ� �ִϸ��̼��� ���� ��� ȣ��
	void AttackEndProc();

	//Player�� Idle���·� ���ư����ϴ� ��� ȣ��
	void GotoIdleProc();

	//Player�� Aim������ �������� �ٲ����ϴ� ��� ȣ��
	void AimUpProc();

	//---------------------------------

	//State Set�Լ�
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

