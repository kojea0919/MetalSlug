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
	//���� ����
	VertexBuffer	m_tVB;

	//�ε��� ����
	IndexBuffer		m_tIB;

	//���� ����
	D3D11_PRIMITIVE_TOPOLOGY	m_ePrimitive;

public:
	//Default2D Mesh Create
	bool CreateMesh();

	//���ڷ� �ѱ� Mesh Create
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

