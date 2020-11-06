#include "pch.h"
#include "RectViewer.h"

CRectViewer::CRectViewer()
{
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	m_iUseCnt = 8;
}

CRectViewer::~CRectViewer()
{
}

bool CRectViewer::Init(CEditDevice* pDevice)
{
	if (!CViewer::Init(pDevice))
		return false;

	return true;
}

bool CRectViewer::SetRect(POINT tLT, POINT tRB)
{
	m_tStart = tLT;
	m_tEnd = tRB;

	if (!CreateCBuffer())
		return false;

	return true;
}

void CRectViewer::SetIndexBuffer()
{
	m_sIdx[0] = 0;
	m_sIdx[1] = 1;
	m_sIdx[2] = 1;
	m_sIdx[3] = 3;
	m_sIdx[4] = 3;
	m_sIdx[5] = 2;
	m_sIdx[6] = 2;
	m_sIdx[7] = 0;
}

void CRectViewer::SetShaderName()
{
	lstrcpy(m_strFileName, TEXT("TextureViewerStandard2D.fx"));
	strcpy_s(m_strVSFuncName, 64, "Standard2DRectViewVS");
	strcpy_s(m_strPSFuncName, 64, "Standard2DRectViewPS");
}

void CRectViewer::SetCBuffer()
{
	//크기 Setting
	m_ViewerCBuffer.matWVP.Scaling((float)(m_tEnd.x - m_tStart.x), (float)(m_tEnd.y - m_tStart.y), 1.f);

	//위치 Setting
	Matrix Translation;
	Translation.Translation(Vector3((float)m_tStart.x, 750.f - (float)m_tStart.y - (m_tEnd.y - m_tStart.y), 0.f));
	m_ViewerCBuffer.matWVP = m_ViewerCBuffer.matWVP * Translation;

	m_ViewerCBuffer.matWVP = m_ViewerCBuffer.matWVP * XMMatrixOrthographicOffCenterLH(0.f, (float)800,
		0.f, (float)750, 0.f, 1000.f);

	m_ViewerCBuffer.matWVP.Transpose();
}
