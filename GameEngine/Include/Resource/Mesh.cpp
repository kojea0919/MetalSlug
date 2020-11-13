#include "Mesh.h"
#include "Material.h"
#include "../Device.h"

CMesh::CMesh()
	: m_pMaterial(nullptr)
{
}

CMesh::~CMesh()
{
	SAFE_RELEASE(m_pMaterial);
}

void CMesh::SetMaterial(CMaterial* pMaterial)
{
	SAFE_RELEASE(m_pMaterial);
	m_pMaterial = pMaterial;

	if (pMaterial)
		pMaterial->AddRef();
}

//bool CMesh::CreateInstancingBuffer(int iSize, int iCount)
//{
//	//전에 사용하던 버퍼 Release
//	//---------------------------------------------
//	if (m_pInstanceBuffer)
//	{
//		SAFE_DELETE_ARRAY(m_pInstanceBuffer->pData);
//		SAFE_RELEASE(m_pInstanceBuffer->pBuffer);
//		SAFE_DELETE(m_pInstanceBuffer);
//	}
//	//---------------------------------------------
//
//	m_pInstanceBuffer = new VertexBuffer;
//
//	m_pInstanceBuffer->iSize = iSize;
//	m_pInstanceBuffer->iCount = iCount;
//
//	//CPU에서 WVP Matrix Setting하여 넘김
//	m_pInstanceBuffer->eUsage = D3D11_USAGE_DYNAMIC;
//
//	m_pInstanceBuffer->pData = new char[iSize * iCount];
//
//	D3D11_BUFFER_DESC	tDesc = {};
//	tDesc.ByteWidth = iSize * iCount;
//	tDesc.Usage = D3D11_USAGE_DYNAMIC;
//	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//
//	if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &m_pInstanceBuffer->pBuffer)))
//		return false;
//
//	return true;
//}

CMaterial* CMesh::GetMaterial() const
{
	if (m_pMaterial)
		m_pMaterial->AddRef();

	return m_pMaterial;
}

bool CMesh::CreateMesh(void* pVertices, int iVtxCount, int iVtxSize, D3D11_USAGE eVtxUsage, D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void* pIndices, int iIdxCount, int iIdxSize, D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt)
{
	return true;
}

void CMesh::Save(FILE* pFile)
{
	CRef::Save(pFile);
}

void CMesh::Load(FILE* pFile)
{
	CRef::Load(pFile);
}
