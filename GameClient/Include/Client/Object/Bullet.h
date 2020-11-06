#pragma once

#include "GameObject.h"

class CBullet :
	public CGameObject
{
public:
	CBullet();
	CBullet(const CBullet& bullet);
	virtual ~CBullet();

protected:
	class CMesh2DComponent* m_pMesh;
	float	m_fDistance;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
};

