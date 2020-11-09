#include "Slider.h"
#include "UI/UISlider.h"
#include "UI/UIImage.h"

CSlider::CSlider()
{
}

CSlider::CSlider(const CSlider& ui)
	: CUIObject(ui)
{
}

CSlider::~CSlider()
{
	SAFE_RELEASE(m_pSlider);
	SAFE_RELEASE(m_pBar);
}

bool CSlider::Init()
{
	if (!CUIObject::Init())
		return false;

	m_pSlider = CreateControl<CUISlider>("Slider");
	m_pBar = CreateControl<CUIImage>("Bar");

	m_pSlider->SetButtonStateTexture(Button_State::Normal, "UISliderRect");
	m_pSlider->SetButtonStateTexture(Button_State::MouseOn, "UISliderRect");
	m_pSlider->SetButtonStateTexture(Button_State::Click, "UISliderRect");
	m_pSlider->SetButtonStateTexture(Button_State::Disable, "UISliderRect");

	m_pSlider->SetButtonStateColor(Button_State::Normal, Vector4(0.8f, 0.8f, 0.8f, 1.f));
	m_pSlider->SetButtonStateColor(Button_State::MouseOn, Vector4(1.f, 1.f, 1.f, 1.f));
	m_pSlider->SetButtonStateColor(Button_State::Click, Vector4(0.6f, 0.6f, 0.6f, 1.f));
	m_pSlider->SetButtonStateColor(Button_State::Disable, Vector4(0.2f, 0.2f, 0.2f, 1.f));

	m_pSlider->SetInheritScale(false);
	m_pSlider->SetRelativeScale(20.f, 20.f, 1.f);
	m_pSlider->SetZOrder(1);

	m_pBar->SetRelativeScale(220.f, 20.f, 1.f);
	m_pBar->SetRelativePos(200.f, 300.f, 1.f);
	m_pBar->SetTexture("UISliderBar");

	m_pSlider->SetMinX((m_pBar->GetWorldPos().x));
	m_pSlider->SetMaxX((m_pBar->GetWorldPos().x) + m_pBar->GetWorldScale().x - 20.f);

	SetRoot(m_pBar);
	m_pBar->AddChild(m_pSlider);

	return true;
}

void CSlider::Start()
{
	CUIObject::Start();
}

void CSlider::Update(float fTime)
{
	CUIObject::Update(fTime);
}

void CSlider::PostUpdate(float fTime)
{
	CUIObject::PostUpdate(fTime);
}

void CSlider::PrevRender()
{
	CUIObject::PrevRender();
}

void CSlider::Render()
{
	CUIObject::Render();
}

void CSlider::PostRender()
{
	CUIObject::PostRender();
}

CSlider* CSlider::Clone()
{
	return new CSlider(*this);
}

void CSlider::ButtonCallback()
{
}
