#pragma once

#include "../Ref.h"

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

public:
	MESH_TYPE GetMeshType()	const
	{
		return m_eMeshType;
	}

	//Material Set�Լ�
	void SetMaterial(class CMaterial* pMaterial);

	//Instancing Buffer �����Լ�
	//bool CreateInstancingBuffer(int iSize, int iCount);

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
	virtual void RenderInstancing(const PVertexBuffer pData,int iCount, float fTime);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};