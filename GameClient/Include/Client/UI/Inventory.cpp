#include "Inventory.h"
#include "UI/UITitleBar.h"
#include "UI/UIImage.h"
#include "UI/UIButton.h"
#include "UI/UIObject.h"

CInventory::CInventory()
{
}

CInventory::CInventory(const CInventory& ui)
	: CUIObject(ui)
{
}

CInventory::~CInventory()
{
	SAFE_RELEASE(m_pBack);
	SAFE_RELEASE(m_pTitleBar);
	SAFE_RELEASE(m_pExitButton);
}

bool CInventory::Init()
{
	if (!CUIObject::Init())
		return false;

	SetZOrder(10);

	m_pBack = CreateControl<CUIImage>("Back");
	m_pTitleBar = CreateControl<CUITitleBar>("TitleBar");
	m_pExitButton = CreateControl<CUIButton>("CloseButton");

	m_pBack->SetRelativePos(50.f, 100.f, 0.f);
	m_pBack->SetRelativeScale(300.f, 400.f, 1.f);
	m_pBack->SetTexture("UIBarBack");

	m_pTitleBar->SetInheritScale(false);
	m_pTitleBar->SetRelativePos(0.f, 380.f, 0.f);
	m_pTitleBar->SetRelativeScale(280.f, 20.f, 1.f);
	m_pTitleBar->SetTexture("UIBar");
	m_pTitleBar->SetZOrder(1);

	m_pExitButton->SetInheritScale(false);
	m_pExitButton->SetRelativePos(280.f, 380.f, 0.f);
	m_pExitButton->SetRelativeScale(20.f, 20.f, 1.f);
	m_pExitButton->SetZOrder(1);
	m_pExitButton->SetClickCallback<CInventory>(this, &CInventory::ExitButtonCallback);


	m_pExitButton->SetButtonStateTexture(Button_State::Normal, "CloseButton");
	m_pExitButton->SetButtonStateTexture(Button_State::MouseOn, "CloseButton");
	m_pExitButton->SetButtonStateTexture(Button_State::Click, "CloseButton");
	m_pExitButton->SetButtonStateTexture(Button_State::Disable, "CloseButton");

	m_pExitButton->SetButtonStateColor(Button_State::Normal, Vector4(0.8f, 0.8f, 0.8f, 1.f));
	m_pExitButton->SetButtonStateColor(Button_State::MouseOn, Vector4(1.f, 1.f, 1.f, 1.f));
	m_pExitButton->SetButtonStateColor(Button_State::Click, Vector4(0.6f, 0.6f, 0.6f, 1.f));
	m_pExitButton->SetButtonStateColor(Button_State::Disable, Vector4(0.2f, 0.2f, 0.2f, 1.f));

	m_pExitButton->SetButtonSound(BUTTON_SOUND::SOUND_MOUSEON, "ButtonMouseOn");
	m_pExitButton->SetButtonSound(BUTTON_SOUND::SOUND_CLICK, "ButtonClick");

	SetRoot(m_pBack);

	m_pBack->AddChild(m_pTitleBar);
	m_pBack->AddChild(m_pExitButton);

	return true;
}

void CInventory::Start()
{
	CUIObject::Start();
}

void CInventory::Update(float fTime)
{
	CUIObject::Update(fTime);
}

void CInventory::PostUpdate(float fTime)
{
	CUIObject::PostUpdate(fTime);
}

void CInventory::PrevRender()
{
	CUIObject::PrevRender();
}

void CInventory::Render()
{
	CUIObject::Render();
}

void CInventory::PostRender()
{
	CUIObject::PostRender();
}

CInventory* CInventory::Clone()
{
	return new CInventory(*this);
}

void CInventory::ExitButtonCallback()
{
	Enable(false);
}
