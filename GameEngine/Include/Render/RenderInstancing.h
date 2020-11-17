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
	bool Init(class CMesh* pMesh, class CMaterial* pMaterial);
	void AddInstancingData(const InstancingData* pData);
	void Render();
};

