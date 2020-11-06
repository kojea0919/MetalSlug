#include "Mesh.h"
#include "Material.h"

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
