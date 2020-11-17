#include "Mesh2D.h"
#include "../Device.h"

CMesh2D::CMesh2D()
	: m_ePrimitive(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	memset(&m_tVB, 0, sizeof(VertexBuffer));
	memset(&m_tIB, 0, sizeof(IndexBuffer));

	m_eMeshType = MESH_TYPE::MESH2D;
}

CMesh2D::~CMesh2D()
{
	SAFE_RELEASE(m_tVB.pBuffer);
	SAFE_RELEASE(m_tIB.pBuffer);
}

bool CMesh2D::CreateMesh()
{
	//위상 구조 Setting
	m_ePrimitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//2D에서 사용할 사각형 정점 Setting
	//-------------------------------------
	Vertex2D tRect[4] = {};

	tRect[0].vPos = Vector3(0.f, 1.f, 0.f);
	tRect[0].vUV = Vector2(0.f, 0.f);
	tRect[0].vColor = Vector4::Red;

	tRect[1].vPos = Vector3(1.f, 1.f, 0.f);
	tRect[1].vUV = Vector2(1.f, 0.f);
	tRect[1].vColor = Vector4::Green;

	tRect[2].vPos = Vector3(0.f, 0.f, 0.f);
	tRect[2].vUV = Vector2(0.f, 1.f);
	tRect[2].vColor = Vector4(1.f, 1.f, 0.f, 1.f);

	tRect[3].vPos = Vector3(1.f, 0.f, 0.f);
	tRect[3].vUV = Vector2(1.f, 1.f);
	tRect[3].vColor = Vector4(1.f, 0.f, 1.f, 1.f);

	//-------------------------------------

	//생성할 정점 버퍼 정보 Setting
	//-------------------------------------
	m_tVB.iSize = sizeof(Vertex2D);
	m_tVB.iCount = 4;

	//버퍼를 읽고 쓰는 방식
	//D3D11_USAGE_IMMUTABLE : GPU의 읽기 허용, CPU접근 불가
	m_tVB.eUsage = D3D11_USAGE_IMMUTABLE;

	D3D11_BUFFER_DESC tDesc = {};
	tDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tDesc.ByteWidth = m_tVB.iSize * m_tVB.iCount;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//-------------------------------------

	//버퍼를 초기화하기 위한 데이터 저장
	//-------------------------------------
	D3D11_SUBRESOURCE_DATA tData = {};
	tData.pSysMem = &tRect;
	//-------------------------------------

	//정점 버퍼 생성
	//---------------------------------------------------
	if(FAILED(DEVICE->CreateBuffer(&tDesc,&tData,&m_tVB.pBuffer)))
		return false;
	//---------------------------------------------------

	m_vMin = Vector3(0.f, 0.f, 0.f);
	m_vMax = Vector3(1.f, 1.f, 0.f);


	//사각형을 그리기위한 인덱스 Setting
	//---------------------------------------
	unsigned short sIdx[6] = { 0, 1, 3, 0, 3, 2 };
	//---------------------------------------

	//인덱스 버퍼 Setting
	//---------------------------------------
	m_tIB.iSize = 2;
	m_tIB.iCount = 6;
	m_tIB.eUsage = D3D11_USAGE_IMMUTABLE;
	m_tIB.eFmt = DXGI_FORMAT_R16_UINT;

	D3D11_BUFFER_DESC tIdxDesc = {};
	tIdxDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tIdxDesc.ByteWidth = m_tIB.iCount * m_tIB.iSize;
	tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//---------------------------------------

	//데이터 저장
	//---------------------------------------
	D3D11_SUBRESOURCE_DATA tIdxData = {};
	tIdxData.pSysMem = &sIdx;
	//---------------------------------------

	//인덱스 버퍼 생성
	//---------------------------------------
	if (FAILED(DEVICE->CreateBuffer(&tIdxDesc, &tIdxData, &m_tIB.pBuffer)))
		return false;
	//---------------------------------------

	return true;
}

bool CMesh2D::CreateMesh(void* pVertices, int iVtxCount, int iVtxSize, D3D11_USAGE eVtxUsage, D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void* pIndices, int iIdxCount, int iIdxSize, D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt)
{
	//정점 버퍼 Create
	//-------------------------------------
	m_ePrimitive = ePrimitive;

	m_tVB.iCount = iVtxCount;
	m_tVB.iSize = iVtxSize;
	m_tVB.eUsage = eVtxUsage;

	D3D11_BUFFER_DESC	tVBDesc = {};

	tVBDesc.Usage = eVtxUsage;
	tVBDesc.ByteWidth = m_tVB.iSize * m_tVB.iCount;
	tVBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA	tData = {};

	tData.pSysMem = pVertices;

	if (FAILED(DEVICE->CreateBuffer(&tVBDesc, &tData, &m_tVB.pBuffer)))
		return false;

	char* pVertexData = (char*)pVertices;

	m_vMin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
	m_vMax = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (int i = 0; i < iVtxCount; ++i)
	{
		Vector3* pPos = (Vector3*)pVertexData;
		pVertexData += iVtxSize;

		if (m_vMin.x > pPos->x)
			m_vMin.x = pPos->x;

		if (m_vMin.y > pPos->y)
			m_vMin.y = pPos->y;

		if (m_vMin.z > pPos->z)
			m_vMin.z = pPos->z;

		if (m_vMax.x < pPos->x)
			m_vMax.x = pPos->x;

		if (m_vMax.y < pPos->y)
			m_vMax.y = pPos->y;

		if (m_vMax.z < pPos->z)
			m_vMax.z = pPos->z;
	}
	//-------------------------------------

	//인덱스 버퍼 Create
	//-------------------------------------
	if (pIndices)
	{
		m_tIB.iCount = iIdxCount;
		m_tIB.iSize = iIdxSize;
		m_tIB.eUsage = eIdxUsage;
		m_tIB.eFmt = eFmt;

		D3D11_BUFFER_DESC	tIdxDesc = {};

		tIdxDesc.Usage = eIdxUsage;
		tIdxDesc.ByteWidth = m_tIB.iSize * m_tIB.iCount;
		tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA	tIdxData = {};

		tIdxData.pSysMem = pIndices;

		if (FAILED(DEVICE->CreateBuffer(&tIdxDesc, &tIdxData, &m_tIB.pBuffer)))
			return false;
	}
	//-------------------------------------

	return true;
}

void CMesh2D::Render(float fTime)
{
	UINT iStride = m_tVB.iSize;
	UINT iOffset = 0;

	//위상 구조 Setting
	CONTEXT->IASetPrimitiveTopology(m_ePrimitive);

	//https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetvertexbuffers
	CONTEXT->IASetVertexBuffers(0, 1, &m_tVB.pBuffer, &iStride, &iOffset);

	//인덱스 버퍼가 있는 경우는 입력 조립기에 Setting
	if (m_tIB.pBuffer)
	{
		CONTEXT->IASetIndexBuffer(m_tIB.pBuffer, m_tIB.eFmt, 0);
		CONTEXT->DrawIndexed(m_tIB.iCount, 0, 0);
	}
	else
	{
		CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CONTEXT->Draw(m_tVB.iCount,0);
	}
}

void CMesh2D::RenderInstancing(const PVertexBuffer pData, float fTime)
{
}

void CMesh2D::Save(FILE* pFile)
{
	CMesh::Save(pFile);
}

void CMesh2D::Load(FILE* pFile)
{
	CMesh::Load(pFile);
}
