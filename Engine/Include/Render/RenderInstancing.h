#pragma once

#include "../GameEngine.h"

class CRenderInstancing
{
public:
	CRenderInstancing();
	~CRenderInstancing();

private:
	class CMesh*			m_pMesh;
	class CMaterial*		m_pMaterial;
	VertexBuffer			m_tInstancingBuffer;
	int						m_iCount;

public:
	bool CheckMesh(class CMesh* pMesh)
	{
		return m_pMesh == pMesh;
	}

	bool CheckMaterial(class CMaterial* pMaterial)
	{
		return m_pMaterial == pMaterial;
	}

public:
	bool Init(class CMesh* pMesh, class CMaterial* pMaterial);
	void AddInstancingData(const InstancingData* pData);
	void Render(float fTime);
	void Clear();
};

