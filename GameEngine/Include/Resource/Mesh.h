#pragma once

#include "../Ref.h"

//���� ����
//------------------------------
typedef struct _tagVertexBuffer
{
	ID3D11Buffer*	pBuffer;	//����
	int				iSize;		//���� 1���� ũ��
	int				iCount;		//���� ��
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


//�ε��� ����
//------------------------------
typedef struct _tagIndexBuffer
{
	ID3D11Buffer* pBuffer;	//����
	int				iSize;		//���� 1���� ũ��
	int				iCount;		//���� ��
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

	//�ش� Mesh���� ����� Material
	class CMaterial* m_pMaterial;

	//MeshŸ��
	MESH_TYPE	m_eMeshType;

	//Instancing�� ����
	PVertexBuffer	m_pInstanceBuffer;

public:
	//Material Set�Լ�
	void SetMaterial(class CMaterial* pMaterial);

	//Instancing Buffer �����Լ�
	bool CreateInstancingBuffer(int iSize, int iCount);

	//Material Get�Լ�
	class CMaterial* GetMaterial() const;

	//Min,Max Get�Լ�
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