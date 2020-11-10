#include "UICheckBox.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Texture.h"
#include "../Resource/Material.h"
#include "../Resource/Sound.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Input.h"
#include "../Resource/ShaderManager.h"
#include "UIText.h"
#include "../Device.h"


CUICheckBox::CUICheckBox()
	: m_bIsCheck(false),m_pCheckTexture(nullptr),
	m_pText(nullptr)
{
}

CUICheckBox::CUICheckBox(const CUICheckBox& control)
	: CUIButton(control)
{
}

CUICheckBox::~CUICheckBox()
{
	SAFE_RELEASE(m_pText);
	SAFE_RELEASE(m_pCheckTexture);
}

void CUICheckBox::SetCheckTexture(const string& strName)
{
	SAFE_RELEASE(m_pCheckTexture);

	m_pCheckTexture = m_pScene->GetResourceManager()->FindTexture(strName);
}

void CUICheckBox::SetText(const TCHAR* pText)
{
	m_pText->SetText(pText);
}

bool CUICheckBox::Init()
{
	if (!CUIButton::Init())
		return false;

	m_pMaterial->SetShader("CheckBoxShader");

	m_pText = new CUIText;

	if (!m_pText->Init())
		return false;

	m_pText->SetName("CheckBoxText");
	m_pText->m_pViewport = m_pViewport;
	m_pText->m_pOwner = m_pOwner;
	m_pText->m_pScene = m_pScene;

	m_pText->AlphaBlend(true);
	m_pText->SetOpacity(0.5f);
	m_pText->SetShadow(true);
	m_pText->SetShadowOffset(Vector3(2.f, -2.f, 0.f));

	return true;
}

void CUICheckBox::Start()
{
	CUIButton::Start();

	m_pText->Start();
}

void CUICheckBox::Update(float fTime)
{
	CUIButton::Update(fTime);

	m_pText->Update(fTime);
}

void CUICheckBox::PostUpdate(float fTime)
{
	CUIButton::PostUpdate(fTime);

	if (m_bClick)
		m_bIsCheck = !m_bIsCheck;

	//글자수에 비례하여 CheckBox의 왼쪽으로 이동
	//-----------------------------------------
	Vector3 vPos = GetRelativePos();
	vPos.x -= (m_pText->GetTextCount() * 12.f);

	m_pText->SetRelativePos(vPos);
	m_pText->SetRenderArea(vPos.x, vPos.y + 15.f, vPos.x + m_pText->GetTextCount() * 12.f, vPos.y - 15.f);

	m_pText->PostUpdate(fTime);
	//-----------------------------------------
}

void CUICheckBox::PrevRender()
{
	CUIButton::PrevRender();

	m_pText->PrevRender();
}

void CUICheckBox::Render()
{
	ButtonCBuffer	tCBuffer = {};
	tCBuffer.iCheck = m_bIsCheck ? 1 : 0;

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Button", &tCBuffer);

	if (m_pCheckTexture)
		m_pCheckTexture->SetShader(10, (int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL);

	CUIButton::Render();

	m_pText->Render();
}

void CUICheckBox::PostRender()
{
	CUIButton::PostRender();

	m_pText->PostRender();
}

CUICheckBox* CUICheckBox::Clone()
{
	return new CUICheckBox(*this);
}
