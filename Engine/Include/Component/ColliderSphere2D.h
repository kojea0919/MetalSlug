#pragma once

#include "Collider.h"

class CColliderSphere2D : public CCollider
{
	friend class CGameObject;

protected:
	CColliderSphere2D();
	CColliderSphere2D(const CColliderSphere2D& com);
	virtual ~CColliderSphere2D();

private:
	SphereInfo m_tInfo;

public:
	//Get함수
	//-------------------------
	SphereInfo GetInfo() const
	{
		return m_tInfo;
	}
	//-------------------------

public:
	//Set함수
	//-------------------------
	void SetRadius(float fRadius);
	//-------------------------

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CColliderSphere2D* Clone();

public:
	virtual bool Collision(CCollider* pCollider);
	virtual bool CollisionMouse(const Vector2& vMouse);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);


};

