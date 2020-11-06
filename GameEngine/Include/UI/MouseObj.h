#pragma once

#include "UIObject.h"

class CMouseObj : public CUIObject
{
public:
	CMouseObj();
	CMouseObj(const CMouseObj& mouse);
	virtual ~CMouseObj();

protected:
	class CUISprite * m_pImage;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void CollisionBegin(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void CollisionEnd(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

