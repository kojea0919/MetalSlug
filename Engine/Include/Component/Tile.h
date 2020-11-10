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

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdata(float fTime);
	virtual void Render();
};

