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
	//SceneComponent�� ũ��, ����, ��ġ ����
	class CTransform* m_pTransform;

	//�θ� Component
	CSceneComponent*	m_pParent;

	//�ڽ� Component
	vector<CSceneComponent*>	m_vecChild;

	//SceneComponent Ÿ��(2D,3D,UI)
	SCENECOMPONENT_TYPE	m_eSceneComponentType;

	//SceneComponent Class Ÿ��(Mesh,Mesh2D,Static)
	SCENECOMPONENT_CLASS_TYPE m_eSceneClassType;

	Render_Priority		m_eRenderPriority;

public:
	//Transform Get�Լ�
	class CTransform* GetTransform() const
	{
		return m_pTransform;
	}

	//SceneComponentŸ�� Get�Լ�
	SCENECOMPONENT_TYPE GetSceneComponentType() const
	{
		return m_eSceneComponentType;
	}

	//SceneComponent Class Ÿ�� Get�Լ�
	SCENECOMPONENT_CLASS_TYPE GetSceneComponentClassType()
	{
		return m_eSceneClassType;
	}

	//�θ� Component �̸� Get�Լ�
	string GetParentName() const
	{
		if (!m_pParent)
			return "";
		return m_pParent->GetName();
	}

	//Render Priority  Set�Լ�
	//--------------------------
	void SetRender_Priority(Render_Priority eRenderPriority)
	{
		m_eRenderPriority = eRenderPriority;
	}
	//--------------------------

	//Render Priority Get�Լ�
	//--------------------------
	Render_Priority GetRender_Priority() const
	{
		return m_eRenderPriority;
	}
	//--------------------------

public:
	//�ڽ� Component �߰�, ���� �Լ�
	//--------------------------------------------------
	void AddChild(CSceneComponent* pChild, const string& strSocketName = "");
	void DeleteChild(CSceneComponent* pChild);
	//--------------------------------------------------

	//Component Ž�� �Լ�
	CSceneComponent* FindComponent(const string& strName);

	//Component�� �̸� ���� �Լ�
	void GetAllComponentName(vector<HierarchyName>& vecName);

	//Component ���� �Լ�
	void GetAllComponent(vector<CSceneComponent*>& vecSceneCom);

private:
	//�θ�, �ڽİ��� ���� �Լ�
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
	//Velocity Get�Լ�
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
	//Get,Set�Լ�
	//----------------------------
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	//----------------------------
	
};