#pragma once

#include "Collider.h"

class CColliderPixel : public CCollider
{
	friend class CGameObject;

protected:
	CColliderPixel();
	CColliderPixel(const CColliderPixel& com);
	virtual ~CColliderPixel();

private:
	PixelColliderInfo	m_tInfo;

public:
	const PixelColliderInfo & GetInfo() const
	{
		return m_tInfo;
	}

public:
	//Image File읽어서 Pixel정보 Setting
	//--------------------------------------------------------
	void SetPixel(const TCHAR* pFileName, const string& strPathName = TEXTURE_PATH);
	void SetPixelFullPath(const TCHAR* pFullPath);
	//--------------------------------------------------------

	//Set함수
	//--------------------------------------------------------
	void SetPixelCollisionType(PIXEL_COLLISION_TYPE eType);
	void SetIgnoreColor(unsigned char r, unsigned char g,
		unsigned char b);
	void SetIgnoreAlpha(unsigned char a);
	void SetCheckColor(unsigned char r, unsigned char g, unsigned char b);
	//--------------------------------------------------------
public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CColliderPixel* Clone();

public:
	virtual bool Collision(CCollider* pCollider);
	virtual bool CollisionMouse(const Vector2& vMouse);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

