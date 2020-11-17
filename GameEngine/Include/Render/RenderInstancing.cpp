#include "RenderInstancing.h"
#include "../Resource/Mesh.h"
#include "../Resource/Material.h"
#include "../Device.h"

CRenderInstancing::CRenderInstancing()
	: m_pMesh(nullptr), m_pMaterial(nullptr),
	m_iCount(0)
{
}

CRenderInstancing::~CRenderInstancing()
{
	SAFE_RELEASE(m_tInstancingBuffer.pBuffer);
	SAFE_DELETE_ARRAY(m_tInstancingBuffer.pData);
}

bool CRenderInstancing::Init(CMesh* pMesh, CMaterial* pMaterial)
{
	m_pMesh = pMesh;
	m_pMaterial = pMaterial;

	m_tInstancingBuffer.iSize = sizeof(InstancingData);
	m_tInstancingBuffer.iCount = 1000;
	m_tInstancingBuffer.eUsage = D3D11_USAGE_DYNAMIC;
	m_tInstancingBuffer.pData = new char[m_tInstancingBuffer.iSize * m_tInstancingBuffer.iCount];
	
	D3D11_BUFFER_DESC tDesc = {};

	tDesc.ByteWidth = m_tInstancingBuffer.iSize * m_tInstancingBuffer.iCount;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if(FAILED(DEVICE->CreateBuffer(&tDesc,nullptr,&m_tInstancingBuffer.pBuffer)))
		return false;

	return true;
}

void CRenderInstancing::AddInstancingData(const InstancingData* pData)
{
	memcpy(((InstancingData*)m_tInstancingBuffer.pData) +m_iCount,pData,
		m_tInstancingBuffer.iSize);

	++m_iCount;
}

void CRenderInstancing::Render()
{
	D3D11_MAPPED_SUBRESOURCE	tMap = {};

	CONTEXT->Map(m_tInstancingBuffer.pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

	memcpy(tMap.pData, m_tInstancingBuffer.pData, m_tInstancingBuffer.iSize * m_iCount);

	CONTEXT->Unmap(m_tInstancingBuffer.pBuffer, 0);
}
