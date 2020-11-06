#include "pch.h"
#include "TextureViewer.h"
#include "EditDevice.h"
#include "PathManager.h"

CTextureViewer::CTextureViewer()
	: m_pImage(nullptr),m_pSRV(nullptr),
		m_pPointSmp(nullptr)

{
}

CTextureViewer::~CTextureViewer()
{
	SAFE_RELEASE(m_pImage);
	SAFE_RELEASE(m_pPointSmp);
	SAFE_RELEASE(m_pSRV);
}

bool CTextureViewer::Init(CEditDevice* pDevice)
{
	if (!CViewer::Init(pDevice))
		return false;

	//Sampler Setting
	InitSampler();

	return true;
}

bool CTextureViewer::SetImage(ScratchImage* pImage)
{
	SAFE_RELEASE(m_pImage);

	m_pImage = pImage;

	//ResourceView ����
	if (!CreateResourceView())
		return false;

	if (!CreateCBuffer())
		return false;

	return true;
}

void CTextureViewer::SetIndexBuffer()
{
	m_sIdx[0] = 0;
	m_sIdx[1] = 1;
	m_sIdx[2] = 3;
	m_sIdx[3] = 0;
	m_sIdx[4] = 3;
	m_sIdx[5] = 2;
}

bool CTextureViewer::CreateResourceView()
{
	//�̹����� ���� ShaderResourceView����
	//--------------------------------------------------
	ScratchImage* pCurImage = m_pImage;

	if (FAILED(CreateShaderResourceView(m_pDevice->GetDevice(),
		pCurImage->GetImages(),
		pCurImage->GetImageCount(),
		pCurImage->GetMetadata(),
		&m_pSRV)))
		return false;
	//--------------------------------------------------

	//�̹��� ũ�� Setting
	//--------------------------------------------------
	m_iWidth = (unsigned int)pCurImage->GetImages()[0].width;
	m_iHeight = (unsigned int)pCurImage->GetImages()[0].height;
	//--------------------------------------------------

	//Resourceview Bind
	m_pDevice->GetContext()->PSSetShaderResources(0, 1, &m_pSRV);

	return true;
}

void CTextureViewer::SetShaderName()
{
	lstrcpy(m_strFileName, TEXT("TextureViewerStandard2D.fx"));
	strcpy_s(m_strVSFuncName,64, "Standard2DTextureViewVS");
	strcpy_s(m_strPSFuncName,64, "Standard2DTextureViewPS");
}

void CTextureViewer::SetCBuffer()
{
	//������� Setting
	//----------------------------------------------------

	//Texture ũ�� Setting
	m_ViewerCBuffer.matWVP.Scaling((float)m_iWidth, (float)m_iHeight, 1.f);

	//���� ���� �̵� ��� Setting
	Matrix Translation;
	Translation.Translation(Vector3(0.f, 750.f - (float)m_iHeight, 0.f));
	m_ViewerCBuffer.matWVP = m_ViewerCBuffer.matWVP * Translation;

	//���� ��� Setting
	m_ViewerCBuffer.matWVP = m_ViewerCBuffer.matWVP * XMMatrixOrthographicOffCenterLH(0.f, (float)800,
		0.f, (float)750, 0.f, 1000.f);

	m_ViewerCBuffer.matWVP.Transpose();
	//----------------------------------------------------
}

void CTextureViewer::InitSampler()
{
	D3D11_SAMPLER_DESC tDesc = {};

	tDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	tDesc.MinLOD = -FLT_MAX;
	tDesc.MaxLOD = FLT_MAX;

	m_pDevice->GetDevice()->CreateSamplerState(&tDesc, &m_pPointSmp);

	m_pDevice->GetContext()->VSSetSamplers(0, 1, &m_pPointSmp);
	m_pDevice->GetContext()->PSSetSamplers(0, 1, &m_pPointSmp);
}
