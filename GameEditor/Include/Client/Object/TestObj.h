#pragma once

#include "GameObject.h"

class CTestObj : public CGameObject
{
public:
	CTestObj();
	CTestObj(const CTestObj& bullet);
	virtual ~CTestObj();

protected:
	class CMesh2DComponent* m_pMesh;
	//class CColliderPixel* m_pBody;
	class CColliderPixel* m_pBody;
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

