#include "StaticMesh.h"

CStaticMesh::CStaticMesh()
{
	m_eMeshType = MESH_TYPE::STATICMESH;
}

CStaticMesh::~CStaticMesh()
{
}

void CStaticMesh::Render(float fTime)
{
}

void CStaticMesh::Save(FILE* pFile)
{
	CMesh::Save(pFile);
}

void CStaticMesh::Load(FILE* pFile)
{
	CMesh::Load(pFile);
}
