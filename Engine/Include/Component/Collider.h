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

	//에디터에서만 Render
	//---------------------------------	
	class CMesh*		m_pDebugMesh;
	bool				m_bEditorRender;
	class CMaterial*	m_pMaterial;
	//---------------------------------

	//충돌시 교점
	Vector3				m_vImpactPoint;

	//Collider 속성
	//---------------------------------
	bool				m_bUI;
	bool				m_b2D;
	//---------------------------------

	//Collider크기
	//---------------------------------
	Vector3				m_vMin;
	Vector3				m_vMax;
	//---------------------------------

	//속한 Section인덱스 List
	list<int>			m_SectionList;

	//?
	list<int>			m_CheckSectionList;

	//현재 프레임에서 떨어진 충돌체를 Check했는지에대한 bool변수
	bool				m_bCurrentFrameSectionCheck;

	//이전 프레임에서 충돌된 Collider List
	list<CCollider*>	m_PrevCollisionList;
	
	//현재 충돌된 Collider List
	list<CCollider*>	m_CurrentCollisionList;

	//해당 Collider가 사용하는 Profile(어떤 채널과 충돌처리를 할지)
	PCollisionProfile	m_pProfile;

	//충돌시 Callback함수
	//----------------------------------------------
	list<function<void(CCollider*, CCollider*, float)>>	m_Callback[(int)Collision_State::Max];
	list<function<void(CCollider*, const Vector2&, float)>>	m_MouseCallback[(int)Collision_State::Max];
	//----------------------------------------------
	
	bool				m_bMouseCollision;
	bool				m_bCollisionEnable;

	//Instancing을 할지 나타내는 bool변수
	bool				m_bInstancing;

	bool				m_bIsCollision;

public:
	//Get함수
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
	//해당 이름의 Profile Setting
	void SetCollisionProfile(const string& strName);

public:
	//이전에 충돌중이던 Collider를 Check해서 떨어진 경우를 판별
	void CheckPrevColliderSection(float fTime);

	//PrevColliderList에 추가,제거
	//-----------------------------------------
	void AddPrevCollider(CCollider* pCollider);
	void DeletePrevCollider(CCollider* pCollider);
	//-----------------------------------------

	//PrevColliderList가 비어있는지 확인
	bool EmptyPrevCollision();

	//해당 Collider가 PrevColliderList에 있는지 확인
	bool CheckPrevCollision(CCollider* pCollider);

	//충돌 상태에 따른 등록한 Callback함수 호출
	void CallCollisionCallback(Collision_State eState, CCollider* pDest,
		float fTime);

	//마우스와의 충돌에 따른 등록한 Callback함수 호출
	void CallMouseCollisionCallback(Collision_State eState, const Vector2& vMousePos, float fTime);

	//CurrentCollisionList에 추가
	void AddCurrentCollision(CCollider* pCollider);
	
	//해당 Collider가 CurrentCollisionList에 있는지 확인
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
	//충돌시 호출할 Callback함수 등록
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

