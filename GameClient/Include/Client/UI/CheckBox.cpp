#include "CheckBox.h"
#include "UI/UICheckBox.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"

CCheckBox::CCheckBox()
{
}

CCheckBox::CCheckBox(const CCheckBox& ui)
	: CUIObject(ui)
{
}

CCheckBox::~CCheckBox()
{
	SAFE_RELEASE(m_pCheckBox);
}

bool CCheckBox::Init()
{
	if (!CUIObject::Init())
		return false;

	m_pCheckBox = CreateControl<CUICheckBox>("Button");

	m_pCheckBox->SetButtonStateTexture(Button_State::Normal, "UICheckBack");
	m_pCheckBox->SetButtonStateTexture(Button_State::MouseOn, "UICheckBack");
	m_pCheckBox->SetButtonStateTexture(Button_State::Click, "UICheckBack");
	m_pCheckBox->SetButtonStateTexture(Button_State::Disable, "UICheckBack");

	m_pCheckBox->SetButtonStateColor(Button_State::Normal, Vector4(0.8f, 0.8f, 0.8f, 1.f));
	m_pCheckBox->SetButtonStateColor(Button_State::MouseOn, Vector4(1.f, 1.f, 1.f, 1.f));
	m_pCheckBox->SetButtonStateColor(Button_State::Click, Vector4(0.6f, 0.6f, 0.6f, 1.f));
	m_pCheckBox->SetButtonStateColor(Button_State::Disable, Vector4(0.2f, 0.2f, 0.2f, 1.f));

	m_pCheckBox->SetCheckTexture("UICheck");

	m_pCheckBox->SetRelativePos(1000.f, 400.f, 0.f);
	m_pCheckBox->SetRelativeScale(40.f, 40.f, 1.f);

	SetRoot(m_pCheckBox);
	return true;
}

void CCheckBox::Start()
{
	CUIObject::Start();
}

void CCheckBox::Update(float fTime)
{
	CUIObject::Update(fTime);
}

void CCheckBox::PostUpdate(float fTime)
{
	CUIObject::PostUpdate(fTime);
}

void CCheckBox::PrevRender()
{
	CUIObject::PrevRender();
}

void CCheckBox::Render()
{
	CUIObject::Render();
}

void CCheckBox::PostRender()
{
	CUIObject::PostRender();
}

CCheckBox* CCheckBox::Clone()
{
	return new CCheckBox(*this);
}

void CCheckBox::ButtonCallback()
{
}
