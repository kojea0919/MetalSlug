#pragma once

#include "../GameEngine.h"

class CTransform
{
	friend class CSceneComponent;

private:
	CTransform();
	CTransform(const CTransform& transform);
	~CTransform();

private:
	class CScene* m_pScene;
	class CSceneComponent* m_pOwner;

	//�θ� Transform
	CTransform* m_pParent;
	//�ڽ� Transform
	vector<CTransform*> m_vecChild;
	TransformCBuffer	m_tCBuffer;

	bool				m_b2D;

public:
	void SetTransformSpace(bool b2D)
	{
		m_b2D = b2D;
	}

private:
	Vector3		m_vVelocityScale;//Scale �ӵ�
	Vector3		m_vVelocityRot;//Rotation �ӵ�
	Vector3		m_vVelocity;//Position �ӵ�

	//�θ��� Transform�� ��������� ���� bool����
	bool		m_bInheritScale;
	bool		m_bInheritRotX;
	bool		m_bInheritRotY;
	bool		m_bInheritRotZ;

	//�̹� �����ӿ��� Transform�� Update�ƴ��� ��Ÿ���� bool����
	bool		m_bUpdateScale;
	bool		m_bUpdateRot;
	bool		m_bUpdatePos;

public:
	void InitVelocity()
	{
		m_vVelocity = Vector3();
	}

public:
	//Velocity Get�Լ�
	//-------------------------------
	Vector3 GetVelocityScale() const
	{
		return m_vVelocityScale;
	}
	Vector3 GetVelocityRot() const
	{
		return m_vVelocityRot;
	}
	Vector3 GetVelocity() const
	{
		return m_vVelocity;
	}
	float GetCurrentVelocity() const
	{
		return m_vVelocity.Length();
	}
	//-------------------------------

public:
	//Inherit
	//----------------------------------

	void SetInheritScale(bool bInherit)
	{
		m_bInheritScale = bInherit;
	}

	void SetInheritRotX(bool bInherit)
	{
		m_bInheritRotX = bInherit;
	}

	void SetInheritRotY(bool bInherit)
	{
		m_bInheritRotY = bInherit;
	}

	void SetInheritRotZ(bool bInherit)
	{
		m_bInheritRotZ = bInherit;
	}

	//�θ� Transform�� �����ϴ� �Լ�
	//----------------------------
	void InheritScale();
	void InheritRot();
	void InheritPos();
	//----------------------------

	//----------------------------------

private:
	//�θ� ���ؿ��� Transform
	Vector3		m_vRelativeScale;
	Vector3		m_vRelativeRot;
	Vector3		m_vRelativePos;
	Vector3		m_vRelativeAxis[AXIS_END];

public:
	//Relative
	//----------------------------------
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
	//----------------------------------

public:
	//Relative
	//----------------------------------
	Vector3 GetRelativeScale() const;
	Vector3 GetRelativeRot() const;
	Vector3 GetRelativePos() const;
	Vector3 GetRelativeAxis(AXIS eAxis) const;
	//----------------------------------

private:
	Vector3		m_vWorldScale;
	Vector3		m_vWorldRot;
	Vector3		m_vWorldPos;
	Vector3		m_vWorldAxis[AXIS_END];
	Vector3		m_vPivot;
	Vector3		m_vMeshSize;

	Matrix		m_matScale;
	Matrix		m_matRot;
	Matrix		m_matTranslation;
	Matrix		m_matWorld;

public:
	// World
	//----------------------------------
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
	//----------------------------------
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
	void Start();
	void Update(float fTime);
	void PostUpdate(float fTime);
	void SetTransform();
	CTransform* Clone();

public:
	//Save,Load�Լ�
	//--------------------
	void Save(FILE* pFile);
	void Load(FILE* pFile);
	//--------------------
};
