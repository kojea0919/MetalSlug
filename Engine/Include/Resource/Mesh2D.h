#pragma once

#include "Mesh.h"

class CMesh2D :
	public CMesh
{
	friend class CResourceManager;
	friend class CSceneResource;

protected:
	CMesh2D();
	~CMesh2D();

private:
	//정점 버퍼
	VertexBuffer	m_tVB;

	//인덱스 버퍼
	IndexBuffer		m_tIB;

	//위상 구조
	D3D11_PRIMITIVE_TOPOLOGY	m_ePrimitive;

public:
	//Default2D Mesh Create
	bool CreateMesh();

	//인자로 넘긴 Mesh Create
	virtual bool CreateMesh(void* pVertices, int iVtxCount, int iVtxSize,
		D3D11_USAGE eVtxUsage,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void* pIndices = nullptr,
		int iIdxCount = 0, int iIdxSize = 0,
		D3D11_USAGE eIdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_UNKNOWN);

public:
	virtual void Render(float fTime);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

