#include "UIProgressBar.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Texture.h"
#include "../Resource/Material.h"
#include "../Resource/ShaderManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CUIProgressBar::CUIProgressBar()
	:m_eDir(BAR_DIR::LeftRight), m_fPercent(1.f)
{
}

CUIProgressBar::CUIProgressBar(const CUIProgressBar& control)
	: CUIControl(control)
{
}

CUIProgressBar::~CUIProgressBar()
{
}

bool CUIProgressBar::Init()
{
	if (!CUIControl::Init())
		return false;

	SAFE_RELEASE(m_pMaterial);
	m_pMaterial = m_pScene->GetResourceManager()->FindMaterial("ProgressBarMaterial");

	return true;
}

void CUIProgressBar::Start()
{
	CUIControl::Start();
}

void CUIProgressBar::Update(float fTime)
{
	CUIControl::Update(fTime);
}

void CUIProgressBar::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CUIProgressBar::PrevRender()
{
	CUIControl::PrevRender();
}

void CUIProgressBar::Render()
{
	CUIControl::Render();

	//상수버퍼 Update
	//------------------------------
	m_tCBuffer.iBarDir = (int)m_eDir;
	m_tCBuffer.fPercent = m_fPercent;

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Bar", &m_tCBuffer);
	//------------------------------

	m_pUITexture->SetShader(0, (int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL);
	m_pMaterial->SetMaterial();

	m_pBodyMesh->Render(0.f);
}

void CUIProgressBar::PostRender()
{
	CUIControl::PostRender();
}

CUIProgressBar* CUIProgressBar::Clone()
{
	return new CUIProgressBar(*this);
}
