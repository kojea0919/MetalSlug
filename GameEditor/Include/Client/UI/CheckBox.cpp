#include "CheckBox.h"
#include "UI/UICheckBox.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"

CCheckBox::CCheckBox()
	: m_bIsCheck(false)
{
}

CCheckBox::CCheckBox(const CCheckBox& ui)
	: CUIObject(ui)
{
}

CCheckBox::~CCheckBox()
{
	SAFE_RELEASE(m_pButton);
}

bool CCheckBox::Init()
{
	if (!CUIObject::Init())
		return false;

	m_pButton = CreateControl<CUICheckBox>("Button");

	m_pButton->SetButtonStateTexture(Button_State::Normal, "UIEmptyCheckBox");
	m_pButton->SetButtonStateTexture(Button_State::MouseOn, "UIEmptyCheckBox");
	m_pButton->SetButtonStateTexture(Button_State::Disable, "UIEmptyCheckBox");
	m_pButton->SetButtonStateTexture(Button_State::Click, "UISelectCheckBox");

	m_pButton->SetRelativePos(900.f, 600.f, 0.f);

	SetRoot(m_pButton);

	m_pButton->SetClickCallback<CCheckBox>(this, &CCheckBox::ButtonCallback);

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
