#pragma once

#include "../Ref.h"

//정점 버퍼
//------------------------------
typedef struct _tagVertexBuffer
{
	ID3D11Buffer*	pBuffer;	//버퍼
	int				iSize;		//정점 1개의 크기
	int				iCount;		//정점 수
	D3D11_USAGE		eUsage;		

	void* pData;

	_tagVertexBuffer()
	{
		pBuffer = nullptr;
		pData = nullptr;
		iSize = 0;
		iCount = 0;
	}
}VertexBuffer, *PVertexBuffer;
//------------------------------


//인덱스 버퍼
//------------------------------
typedef struct _tagIndexBuffer
{
	ID3D11Buffer* pBuffer;	//버퍼
	int				iSize;		//정점 1개의 크기
	int				iCount;		//정점 수
	D3D11_USAGE		eUsage;
	DXGI_FORMAT		eFmt;
}IndexBuffer, *PIndexBuffer;
//------------------------------

class CMesh : public CRef
{
	friend class CResourceManager;
	friend class CSceneResource;

protected:
	CMesh();
	virtual ~CMesh() = 0;

protected:
	Vector3		m_vMin;
	Vector3		m_vMax;

	//해당 Mesh에서 사용할 Material
	class CMaterial* m_pMaterial;

	//Mesh타입
	MESH_TYPE	m_eMeshType;

	//Instancing용 버퍼
	PVertexBuffer	m_pInstanceBuffer;

public:
	//Material Set함수
	void SetMaterial(class CMaterial* pMaterial);

	//Instancing Buffer 생성함수
	bool CreateInstancingBuffer(int iSize, int iCount);

	//Material Get함수
	class CMaterial* GetMaterial() const;

	//Min,Max Get함수
	//-------------------------------
	Vector3 GetMin() const
	{
		return m_vMin;
	}
	Vector3 GetMax() const
	{
		return m_vMax;
	}
	//-------------------------------

public:
	virtual bool CreateMesh(void* pVertices, int iVtxCount, int iVtxSize,
		D3D11_USAGE eVtxUsage,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void* pIndices = nullptr,
		int iIdxCount = 0, int iIdxSize = 0,
		D3D11_USAGE eIdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_UNKNOWN);

public:
	virtual void Render(float fTime) = 0;

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};