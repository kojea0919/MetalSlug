#pragma once

#include "Collider.h"

class CColliderOBB : public CCollider
{
	friend class CGameObject;

protected:
	CColliderOBB();
	CColliderOBB(const CColliderOBB & com);
	virtual ~CColliderOBB();

private:
	Vector2		m_vExtent;
	OBBInfo		m_tInfo;

public:
	OBBInfo GetInfo() const
	{
		return m_tInfo;
	}

public:
	void SetExtent(float x, float y);
	void SetExtent(const Vector2& vExtent);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CColliderOBB* Clone();

public:
	virtual bool Collision(CCollider* pCollider);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

