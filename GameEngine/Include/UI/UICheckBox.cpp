#include "UICheckBox.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Texture.h"
#include "../Resource/Material.h"
#include "../Resource/Sound.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Input.h"
#include "../Resource/ShaderManager.h"

CUICheckBox::CUICheckBox()
	: m_bIsCheck(false)
{
}

CUICheckBox::CUICheckBox(const CUICheckBox& control)
	: CUIButton(control)
{
}

CUICheckBox::~CUICheckBox()
{
	SAFE_RELEASE(m_pCheckTexture);
}

void CUICheckBox::SetCheckTexture(const string& strName)
{
	SAFE_RELEASE(m_pCheckTexture);

	m_pCheckTexture = m_pScene->GetResourceManager()->FindTexture(strName);
}

bool CUICheckBox::Init()
{
	if (!CUIButton::Init())
		return false;

	m_pMaterial->SetShader("CheckBoxShader");

	return true;
}

void CUICheckBox::Start()
{
	CUIButton::Start();
}

void CUICheckBox::Update(float fTime)
{
	CUIButton::Update(fTime);
}

void CUICheckBox::PostUpdate(float fTime)
{
	CUIButton::PostUpdate(fTime);

	if (m_bClick)
		m_bIsCheck = !m_bIsCheck;
}

void CUICheckBox::PrevRender()
{
	CUIButton::PrevRender();
}

void CUICheckBox::Render()
{
	CUIButton::Render();
}

void CUICheckBox::PostRender()
{
	ButtonCBuffer	tCBuffer = {};
	tCBuffer.iCheck = m_bIsCheck ? 1 : 0;

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Button", &tCBuffer);

	if (m_pCheckTexture)
		m_pCheckTexture->SetShader(10, (int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL);

	CUIButton::PostRender();
}

CUICheckBox* CUICheckBox::Clone()
{
	return new CUICheckBox(*this);
}
