#pragma once

#include "Component.h"


class CSceneComponent : public CComponent
{
	friend class CGameObject;

protected:
	CSceneComponent();
	CSceneComponent(const CSceneComponent& com);
	virtual ~CSceneComponent();

protected:
	//SceneComponent의 크기, 방향, 위치 정보
	class CTransform* m_pTransform;

	//부모 Component
	CSceneComponent*	m_pParent;

	//자식 Component
	vector<CSceneComponent*>	m_vecChild;

	//SceneComponent 타입(2D,3D,UI)
	SCENECOMPONENT_TYPE	m_eSceneComponentType;

	//SceneComponent Class 타입(Mesh,Mesh2D,Static)
	SCENECOMPONENT_CLASS_TYPE m_eSceneClassType;

	Render_Priority		m_eRenderPriority;

public:
	//Transform Get함수
	class CTransform* GetTransform() const
	{
		return m_pTransform;
	}

	//SceneComponent타입 Get함수
	SCENECOMPONENT_TYPE GetSceneComponentType() const
	{
		return m_eSceneComponentType;
	}

	//SceneComponent Class 타입 Get함수
	SCENECOMPONENT_CLASS_TYPE GetSceneComponentClassType()
	{
		return m_eSceneClassType;
	}

	//부모 Component 이름 Get함수
	string GetParentName() const
	{
		if (!m_pParent)
			return "";
		return m_pParent->GetName();
	}

	//Render Priority  Set함수
	//--------------------------
	void SetRender_Priority(Render_Priority eRenderPriority)
	{
		m_eRenderPriority = eRenderPriority;
	}
	//--------------------------

	//Render Priority Get함수
	//--------------------------
	Render_Priority GetRender_Priority() const
	{
		return m_eRenderPriority;
	}
	//--------------------------

public:
	//자식 Component 추가, 제거 함수
	//--------------------------------------------------
	void AddChild(CSceneComponent* pChild, const string& strSocketName = "");
	void DeleteChild(CSceneComponent* pChild);
	//--------------------------------------------------

	//Component 탐색 함수
	CSceneComponent* FindComponent(const string& strName);

	//Component의 이름 저장 함수
	void GetAllComponentName(vector<HierarchyName>& vecName);

	//Component 저장 함수
	void GetAllComponent(vector<CSceneComponent*>& vecSceneCom);

private:
	//부모, 자식관계 제거 함수
	void DetatchChild(CSceneComponent* pChild);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CSceneComponent* Clone();

public:
	void InitVelocity();

public:
	//Velocity Get함수
	//------------------------------
	Vector3 GetVelocityScale() const;
	Vector3 GetVelocityRot() const;
	Vector3 GetVelocity() const;
	float GetCurrentVelocity() const;
	//------------------------------

public:
	void SetInheritScale(bool bInherit);
	void SetInheritRotX(bool bInherit);
	void SetInheritRotY(bool bInherit);
	void SetInheritRotZ(bool bInherit);
	void InheritScale();
	void InheritRot();
	void InheritPos();

public:
	void SetRelativeScale(const Vector3& vScale);
	void SetRelativeScale(float x, float y, float z);
	void SetRelativeRotation(const Vector3& vRot);
	void SetRelativeRotation(float x, float y, float z);
	void SetRelativeRotationX(float x);
	void SetRelativeRotationY(float y);
	void SetRelativeRotationZ(float z);
	void SetRelativePos(const Vector3& vPos);
	void SetRelativePos(float x, float y, float z);
	void AddRelativeScale(const Vector3& vScale);
	void AddRelativeScale(float x, float y, float z);
	void AddRelativePos(const Vector3& vPos);
	void AddRelativePos(float x, float y, float z);
	void AddRelativeRotation(const Vector3& vRot);
	void AddRelativeRotation(float x, float y, float z);
	void AddRelativeRotationX(float x);
	void AddRelativeRotationY(float y);
	void AddRelativeRotationZ(float z);

public:
	Vector3 GetRelativeScale()	const;
	Vector3 GetRelativeRot()	const;
	Vector3 GetRelativePos()	const;
	Vector3 GetRelativeAxis(AXIS eAxis)	const;

public:
	void SetWorldScale(const Vector3& vScale);
	void SetWorldScale(float x, float y, float z);
	void SetWorldRotation(const Vector3& vRot);
	void SetWorldRotation(float x, float y, float z);
	void SetWorldRotationX(float x);
	void SetWorldRotationY(float y);
	void SetWorldRotationZ(float z);
	void SetWorldPos(const Vector3& vPos);
	void SetWorldPos(float x, float y, float z);
	void AddWorldScale(const Vector3& vScale);
	void AddWorldScale(float x, float y, float z);
	void AddWorldPos(const Vector3& vPos);
	void AddWorldPos(float x, float y, float z);
	void AddWorldRotation(const Vector3& vRot);
	void AddWorldRotation(float x, float y, float z);
	void AddWorldRotationX(float x);
	void AddWorldRotationY(float y);
	void AddWorldRotationZ(float z);
	void SetPivot(const Vector3& vPivot);
	void SetPivot(float x, float y, float z);
	void SetMeshSize(const Vector3& vSize);

public:
	Vector3 GetWorldScale()	const;
	Vector3 GetWorldRot()	const;
	Vector3 GetWorldPos()	const;
	Vector3 GetWorldAxis(AXIS eAxis)	const;
	Vector3 GetPivot()	const;

	Matrix GetScaleMatrix()	const;
	Matrix GetRotMatrix()	const;
	Matrix GetTranslationMatrix()	const;
	Matrix GetWorldMatrix()	const;

public:
	//Get,Set함수
	//----------------------------
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	//----------------------------
	
};