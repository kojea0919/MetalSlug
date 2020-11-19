#pragma once

#include "../GameEngine.h"

class CTile
{
	friend class CTileMap;

protected:
	CTile();
	virtual ~CTile();

protected:
	TILE_SHAPE	m_eShape;
	class CMaterial* m_pMaterial;
	class CMesh2D* m_pMesh;
	Vector3		m_vTilePos;
	Vector3		m_vTileSize;

	//InstancingData 정보
	//-----------------------
	Matrix		m_matWorld;//위치
	Vector2		m_vFrameStart;//텍스처 시작
	Vector2		m_vFrameEnd;//텍스처 끝
	//-----------------------

public:
	Matrix GetWorldMatirx() const
	{
		return m_matWorld;
	}

	Vector2 GetFrameStart() const
	{
		return m_vFrameStart;
	}

	Vector2 GetFrameEnd() const
	{
		return m_vFrameEnd;
	}

public:
	void SetPos(const Vector3& vPos)
	{
		m_vTilePos = vPos;
	}

	void SetSize(const Vector3& vSize)
	{
		m_vTileSize = vSize;
	}

	void SetShape(TILE_SHAPE eShape)
	{
		m_eShape = eShape;
	}

	void SetFrameStart(float x, float y)
	{
		m_vFrameStart = Vector2(x, y);
	}

	void SetFrameEnd(float x, float y)
	{
		m_vFrameEnd = Vector2(x, y);
	}

	void SetFrameStart(const Vector2& vFrame)
	{
		m_vFrameStart = vFrame;
	}

	void SetFrameEnd(const Vector2& vFrame)
	{
		m_vFrameEnd = vFrame;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Render(float fTime);
};

