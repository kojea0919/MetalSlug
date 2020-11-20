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
	//정점 버퍼와 인덱스 버퍼에 들어있는 정보로 그리는 기본 함수
	virtual void Render(float fTime);

	//Instancing 버퍼를 받아 그리는 Instancing용 그리기 함수
	virtual void RenderInstancing(const PVertexBuffer pData, int iCount, float fTime);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

