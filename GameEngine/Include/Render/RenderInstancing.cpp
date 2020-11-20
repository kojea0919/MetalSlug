#include "RenderInstancing.h"
#include "../Resource/Mesh.h"
#include "../Resource/Material.h"
#include "../Device.h"
#include "../Resource/Shader.h"
#include "../Resource/ShaderManager.h"

CRenderInstancing::CRenderInstancing()
	: m_pMesh(nullptr), m_pMaterial(nullptr),
	m_iCount(0)
{
}

CRenderInstancing::~CRenderInstancing()
{
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_tInstancingBuffer.pBuffer);
	SAFE_DELETE_ARRAY(m_tInstancingBuffer.pData);
}

void CRenderInstancing::SetMaterial(CMaterial* pMaterial)
{
	m_pMaterial = pMaterial;
}

bool CRenderInstancing::Init(CMesh* pMesh, CMaterial* pMaterial, int iSize, const string& strShaderName, int iCount)
{
	//Resource Setting
	//--------------------------------------
	m_pMesh = pMesh;
	m_pMaterial = pMaterial;
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(strShaderName);
	//--------------------------------------

	//인스턴싱용 버퍼 생성
	//--------------------------------------
	m_tInstancingBuffer.iSize = iSize;
	m_tInstancingBuffer.iCount = iCount;
	m_tInstancingBuffer.eUsage = D3D11_USAGE_DYNAMIC;
	m_tInstancingBuffer.pData = new char[m_tInstancingBuffer.iSize * m_tInstancingBuffer.iCount];
	
	D3D11_BUFFER_DESC tDesc = {};

	tDesc.ByteWidth = m_tInstancingBuffer.iSize * m_tInstancingBuffer.iCount;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if(FAILED(DEVICE->CreateBuffer(&tDesc,nullptr,&m_tInstancingBuffer.pBuffer)))
		return false;
	//--------------------------------------

	return true;
}

void CRenderInstancing::AddInstancingData(const void* pData)
{
	//현재 저장한 데이터 수가 버퍼 최대 크기를 초과한 경우 공간 재할당
	//---------------------------------------------------------
	if (m_tInstancingBuffer.iCount == m_iCount)
	{
		m_tInstancingBuffer.iCount *= 2;

		int iCount = m_tInstancingBuffer.iCount;
		int iSize = m_tInstancingBuffer.iSize;

		void* pCopy = new char[iCount * iSize];
		
		memcpy(pCopy, m_tInstancingBuffer.pData, m_iCount * iSize);

		SAFE_RELEASE(m_tInstancingBuffer.pBuffer);
		SAFE_DELETE_ARRAY(m_tInstancingBuffer.pData);

		D3D11_BUFFER_DESC tDesc = {};

		tDesc.ByteWidth = m_tInstancingBuffer.iSize + m_tInstancingBuffer.iCount;
		tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		tDesc.Usage = D3D11_USAGE_DYNAMIC;
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &m_tInstancingBuffer.pBuffer)))
			return;
	}
	//---------------------------------------------------------

	memcpy(((char*)m_tInstancingBuffer.pData) + (m_iCount * m_tInstancingBuffer.iSize)
		,pData,	m_tInstancingBuffer.iSize);

	++m_iCount;
}

void CRenderInstancing::Render(float fTime)
{
	//출력할게 없는 경우 pass
	//------------------------
	if (m_iCount == 0)
		return;
	//------------------------

	D3D11_MAPPED_SUBRESOURCE	tMap = {};

	CONTEXT->Map(m_tInstancingBuffer.pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

	memcpy(tMap.pData, m_tInstancingBuffer.pData, m_tInstancingBuffer.iSize * m_iCount);

	CONTEXT->Unmap(m_tInstancingBuffer.pBuffer, 0);

	m_pMaterial->SetMaterial();

	m_pShader->SetShader();

	//instancing용 Render함수 호출
	m_pMesh->RenderInstancing(&m_tInstancingBuffer,m_iCount, fTime);
}

void CRenderInstancing::Clear()
{
	m_iCount = 0;
}
