#pragma once
#include "Collider.h"
class CColliderRect : public CCollider
{
	friend class CGameObject;

protected:
	CColliderRect();
	CColliderRect(const CColliderRect& com);
	virtual ~CColliderRect();

private:
	Vector2		m_vExtent;
	RectInfo	m_tInfo;

public:
	RectInfo GetInfo()	const
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
	virtual CColliderRect* Clone();

public:
	virtual bool Collision(CCollider* pCollider);
	virtual bool CollisionMouse(const Vector2& vMouse);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

