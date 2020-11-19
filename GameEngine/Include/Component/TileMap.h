#pragma once

#include "PrimitiveComponent.h"
#include "Tile.h"

class CTileMap : public CPrimitiveComponent
{
	friend class CGameObject;

protected:
	CTileMap();
	CTileMap(const CTileMap& com);
	virtual ~CTileMap();

protected:
	TILE_SHAPE		m_eShape;
	int				m_iCountX;
	int				m_iCountY;
	Vector3			m_vTileSize;
	Vector2			m_vTileImageSize;
	vector<class CTile*>	m_vecTile;

	//���� ȭ�鿡�� Ÿ�� ����
	//------------------------
	int				m_iStartX;
	int				m_iStartY;
	int				m_iEndX;
	int				m_iEndY;
	//------------------------

	bool			m_bTileMapRender;

	//Tilemap���� ����� RenderInstancing, Material
	//--------------------------------------
	class CRenderInstancing* m_pInstancing;
	class CMaterial* m_pTileMaterial;
	//--------------------------------------

	//Tile�� �׻� ��濡 ���(���� ���� ��Ȱ��ȭ)
	class CRenderState* m_pDepthDisable;

public:
	void SetTexture(const string& strName);
	void SetTexture(class CTexture* pTexture);
	void SetTileMaterial(const string& strName);
	void SetTileMaterial(class CMaterial* pMaterial);
	void SetTileImageSize(float x, float y);
	void SetTileImageSize(const Vector2& vSize);
	void SetTileFrame(const Vector3& vPos, int iImageFrameX, int iImageFrameY);
	void SetTileFrame(int idxX, int idxY, int iImageFrameX, int iImageFrameY);

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
		//���, ����, ũ�� Setting
		//---------------------------
		m_eShape = eShape;
		m_iCountX = iCountX;
		m_iCountY = iCountY;
		m_vTileSize.x = fTileSizeX;
		m_vTileSize.y = fTileSizeY;
		//---------------------------

		if (eShape == TILE_SHAPE::Rect)
		{
			//���ϴ��� ��ġ�� Setting
			Vector3	vPos = Vector3(0.f, -fTileSizeY, 0.f);

			for (int i = 0; i < m_iCountY; ++i)
			{
				vPos.x = 0.f;
				vPos.y += fTileSizeY;

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

		size_t	iSize = m_vecTile.size();

		for (size_t i = 0; i < iSize; ++i)
		{
			m_vecTile[i]->Start();
		}

		//World�� ���� ��ġ, ũ�� Setting
		SetWorldInfo();
	}

	void SetWorldInfo();
};

