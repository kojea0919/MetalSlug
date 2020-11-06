#pragma once

#include "GameObject.h"

class CMonster : public CGameObject
{
public:
	CMonster();
	CMonster(const CMonster& monster);
	virtual ~CMonster();

protected:
	class CSpriteComponent* m_pMesh;
	class CColliderRect* m_pBody;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);

public:
	void CollisionBegin(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void CollisionEnd(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

