#pragma once

#include "PrimitiveComponent.h"

class CCollider : public CSceneComponent
{
	friend class CCollision;
	friend class CGameObject;

protected:
	CCollider();
	CCollider(const CCollider& com);
	virtual ~CCollider() = 0;

protected:
	COLLIDER_TYPE		m_eColliderType;

	//�����Ϳ����� Render
	//---------------------------------	
	class CMesh*		m_pDebugMesh;
	bool				m_bEditorRender;
	class CMaterial*	m_pMaterial;
	//---------------------------------

	//�浹�� ����
	Vector3				m_vImpactPoint;

	//Collider �Ӽ�
	//---------------------------------
	bool				m_bUI;
	bool				m_b2D;
	//---------------------------------

	//Colliderũ��
	//---------------------------------
	Vector3				m_vMin;
	Vector3				m_vMax;
	//---------------------------------

	//���� Section�ε��� List
	list<int>			m_SectionList;

	//?
	list<int>			m_CheckSectionList;

	//���� �����ӿ��� ������ �浹ü�� Check�ߴ��������� bool����
	bool				m_bCurrentFrameSectionCheck;

	//���� �����ӿ��� �浹�� Collider List
	list<CCollider*>	m_PrevCollisionList;
	
	//���� �浹�� Collider List
	list<CCollider*>	m_CurrentCollisionList;

	//�ش� Collider�� ����ϴ� Profile(� ä�ΰ� �浹ó���� ����)
	PCollisionProfile	m_pProfile;

	//�浹�� Callback�Լ�
	//----------------------------------------------
	list<function<void(CCollider*, CCollider*, float)>>	m_Callback[(int)Collision_State::Max];
	list<function<void(CCollider*, const Vector2&, float)>>	m_MouseCallback[(int)Collision_State::Max];
	//----------------------------------------------
	
	bool				m_bMouseCollision;
	bool				m_bCollisionEnable;

	//Instancing�� ���� ��Ÿ���� bool����
	bool				m_bInstancing;

	bool				m_bIsCollision;

public:
	//Get�Լ�
	//---------------------------------
	class CMesh* GetMesh() const;
	class CMaterial* GetMaterial() const;

	bool IsCollision() const
	{
		return m_bIsCollision;
	}

	COLLIDER_TYPE GetColliderType() const
	{
		return m_eColliderType;
	}
	bool Is2D() const
	{
		return m_b2D;
	}
	bool IsUI() const
	{
		return m_bUI;
	}
	Vector3 GetMin()	const
	{
		return m_vMin;
	}
	Vector3 GetMax()	const
	{
		return m_vMax;
	}
	void AddSection(int iIndex)
	{
		m_SectionList.push_back(iIndex);
	}
	void Clear()
	{
		m_CurrentCollisionList.clear();
		m_SectionList.clear();
		m_bCurrentFrameSectionCheck = false;
	}
	PCollisionProfile GetCollisionProfile() const
	{
		return m_pProfile;
	}
	bool IsCurrentFrameSectionCheck() const
	{
		return m_bCurrentFrameSectionCheck;
	}
	void CurrentFrameSectionCheck()
	{
		m_bCurrentFrameSectionCheck = true;
	}
	void SetMouseCollision(bool bCollision)
	{
		m_bMouseCollision = bCollision;
	}
	bool IsInstancing() const
	{
		return m_bInstancing;
	}
	//---------------------------------

public:
	//�ش� �̸��� Profile Setting
	void SetCollisionProfile(const string& strName);

public:
	//������ �浹���̴� Collider�� Check�ؼ� ������ ��츦 �Ǻ�
	void CheckPrevColliderSection(float fTime);

	//PrevColliderList�� �߰�,����
	//-----------------------------------------
	void AddPrevCollider(CCollider* pCollider);
	void DeletePrevCollider(CCollider* pCollider);
	//-----------------------------------------

	//PrevColliderList�� ����ִ��� Ȯ��
	bool EmptyPrevCollision();

	//�ش� Collider�� PrevColliderList�� �ִ��� Ȯ��
	bool CheckPrevCollision(CCollider* pCollider);

	//�浹 ���¿� ���� ����� Callback�Լ� ȣ��
	void CallCollisionCallback(Collision_State eState, CCollider* pDest,
		float fTime);

	//���콺���� �浹�� ���� ����� Callback�Լ� ȣ��
	void CallMouseCollisionCallback(Collision_State eState, const Vector2& vMousePos, float fTime);

	//CurrentCollisionList�� �߰�
	void AddCurrentCollision(CCollider* pCollider);
	
	//�ش� Collider�� CurrentCollisionList�� �ִ��� Ȯ��
	bool CheckCurrentCollision(CCollider* pCollider);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CCollider* Clone();

public:
	virtual bool Collision(CCollider* pCollider) = 0;
	virtual bool CollisionMouse(const Vector2& vMouse) = 0;

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	//�浹�� ȣ���� Callback�Լ� ���
	//---------------------------------------
	template<typename T>
	void SetCallback(Collision_State eState, T* pObj,
		void(T::* pFunc)(CCollider*, CCollider*, float))
	{
		m_Callback[(int)eState].push_back(bind(pFunc, pObj, placeholders::_1, placeholders::_2, placeholders::_3));
	}
	template<typename T>
	void SetMouseCallback(Collision_State eState, T* pObj,
		void(T::* pFunc)(CCollider*, const Vector2&, float))
	{
		m_MouseCallback[(int)eState].push_back(bind(pFunc, pObj, placeholders::_1, placeholders::_2, placeholders::_3));
	}
	//---------------------------------------
};

