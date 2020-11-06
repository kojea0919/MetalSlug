#pragma once

#include "../GameEngine.h"

class CCollisionSection
{
	friend class CSceneCollision;

private:
	CCollisionSection();
	~CCollisionSection();

private:
	//�浹 Check�ؾ��� Collider vec
	vector<class CCollider*>		m_vecCollider;

	//Section ũ��
	Vector3		m_vSectionSize;
	//World ũ��
	Vector3		m_vWorldSize;
	
	//Section �ּ�,�ִ� ��ǥ
	//--------------------
	Vector3		m_vMin;
	Vector3		m_vMax;
	//--------------------

	//Section Index
	//--------------------
	int			m_iIndexX;
	int			m_iIndexY;
	int			m_iIndexZ;
	int			m_iIndex;
	//--------------------

public:
	//Get�Լ�
	//----------------------
	int GetIndex()	const
	{
		return m_iIndex;
	}

	int GetIndexX()	const
	{
		return m_iIndexX;
	}

	int GetIndexY()	const
	{
		return m_iIndexY;
	}

	int GetIndexZ()	const
	{
		return m_iIndexZ;
	}
	//----------------------

public:
	bool Init(int idxX, int idxY, int idxZ, int iIndex,
		const Vector3& vSectionSize, const Vector3& vWorldSize);
	void SetSection(const Vector3& vMin, const Vector3& vMax);
	void AddCollider(class CCollider* pCollider);
	void Clear();
	void Collision(float fTime);
	class CCollider* CollisionMouse(const Vector2& vMouse);
};

