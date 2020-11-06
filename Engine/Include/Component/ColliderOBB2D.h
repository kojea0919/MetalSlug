#pragma once

#include "Collider.h"

class CColliderOBB2D : public CCollider
{
	friend class CGameObject;

protected:
	CColliderOBB2D();
	CColliderOBB2D(const CColliderOBB2D & com);
	virtual ~CColliderOBB2D();

private:
	OBBInfo	m_tInfo;

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
	virtual CColliderOBB2D* Clone();

public:
	virtual bool Collision(CCollider* pCollider);
	virtual bool CollisionMouse(const Vector2& vMouse);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

