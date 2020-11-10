#pragma once

#include "PrimitiveComponent.h"

class CTileMap : public CPrimitiveComponent
{
	friend class CGameObject;

protected:
	CTileMap();
	CTileMap(const CTileMap& com);
	virtual ~CTileMap();

protected:
	class CMaterial* m_pMaterial;
	class CMesh2D* m_pMesh;

protected:
	TILE_SHAPE		m_eShape;
	int				m_iCountX;
	int				m_iCountY;
	Vector3			m_vTileSize;
	vector<class CTile*>	m_vecTile;

public:
	void SetMaterial(class CMaterial* pMaterial);
	class CMaterial* GetMaterial()	const;
	void SetMesh(class CMesh2D* pMesh);
	void SetMesh(const string& strMeshName);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CTileMap* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	template<typename T>
	void CreateTile(TILE_SHAPE eShape, int iCountX, int iCountY,
		float fTileSizeX, float fTileSizeY)
	{
		m_eShape = eShape;
		m_iCountX = iCountX;
		m_iCountY = iCountY;
		m_vTileSize.x = fTileSizeX;
		m_vTileSize.y = fTileSizeY;

		if (eShape == TILE_SHAPE::Rect)
		{
			Vector3	vPos = Vector3(0.f, fTileSizeY * iCountY, 0.f);

			for (int i = 0; i < m_iCountY; ++i)
			{
				vPos.x = 0.f;
				vPos.y -= fTileSizeY;

				for (int j = 0; j < m_iCountX; ++j)
				{
					T* pTile = new T;

					m_vecTile.push_back((CTile*)pTile);

					pTile->SetPos(vPos);
					pTile->SetSize(m_vTileSize);
					pTile->SetShape(eShape);

					if (!pTile->Init())
						return;

					vPos.x += fTileSizeX;
				}
			}
		}

		else if (eShape == TILE_SHAPE::Rhombus)
		{

		}
	}
};

