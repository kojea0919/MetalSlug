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
	SAFE_RELEASE(m_pSlotImage);
	SAFE_RELEASE(m_pBarBack);
	SAFE_RELEASE(m_pBar);
}

void CInventory::ScrollDown()
{

}

void CInventory::ScrollUp()
{
}

bool CInventory::Init()
{
	if (!CUIObject::Init())
		return false;

	SetZOrder(10);

	m_pBack = CreateControl<CUIImage>("Back");
	m_pTitleBar = CreateControl<CUITitleBar>("TitleBar");
	m_pExitButton = CreateControl<CUIButton>("CloseButton");
	m_pSlotImage = CreateControl<CUIImage>("Slot");
	m_pBarBack = CreateControl<CUIImage>("ScrollBarBack");
	m_pBar = CreateControl<CUIImage>("ScrollBar");

	m_pBack->SetRelativePos(50.f, 100.f, 0.f);
	m_pBack->SetRelativeScale(165.f, 200.f, 1.f);
	m_pBack->SetTexture("InventoryBackImage");

	m_pTitleBar->SetInheritScale(false);
	m_pTitleBar->SetRelativePos(4.f, 175.f, 0.f);
	m_pTitleBar->SetRelativeScale(135.f, 22.f, 1.f);
	m_pTitleBar->SetTexture("Inventorytitlebar");
	m_pTitleBar->SetZOrder(1);

	m_pExitButton->SetInheritScale(false);
	m_pExitButton->SetRelativePos(140.f, 175.f, 0.f);
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

	m_pSlotImage->SetInheritScale(false);
	m_pSlotImage->SetTexture("SlotGrid");
	m_pSlotImage->SetZOrder(2);
	m_pSlotImage->SetRelativeScale(150.f, 249.f, 0.f);
	m_pSlotImage->SetRelativePos(3.f, -74.f, 0.f);

	m_pBarBack->SetInheritScale(false);
	m_pBarBack->SetTexture("InventoryScrollBar");
	m_pBarBack->SetZOrder(2);
	m_pBarBack->SetRelativeScale(10.f, 171.f, 0.f);
	m_pBarBack->SetRelativePos(153.f, 3.f, 0.f);

	//세로 크기 Setting
	//------------------------
	m_fSlotHeight = 249.f;
	m_fSlotRenderHeight = 175.f;
	//------------------------

	//BackBar에서 Bar의 가동 범위 크기
	//------------------------
	m_fBarMoveLen = 147.f;
	//------------------------

	//Bar의 세로 크기 Setting
	//------------------------
	float fBarHeight = m_fSlotRenderHeight * m_fBarMoveLen / m_fSlotHeight;
	//------------------------

	m_pBar->SetInheritScale(false);
	m_pBar->SetTexture("InventoryScroll");
	m_pBar->SetRelativeScale(10.f, fBarHeight,0.f);
	m_pBar->SetRelativePos(153.f, 60.f, 0.f);
	m_pBar->SetZOrder(3);

	SetRoot(m_pBack);

	m_pBack->AddChild(m_pTitleBar);
	m_pBack->AddChild(m_pExitButton);
	m_pBack->AddChild(m_pSlotImage);
	m_pBack->AddChild(m_pBarBack);
	m_pBack->AddChild(m_pBar);

	//출력 좌표 Setting
	//--------------------------
	m_fStart = 0.f;
	m_fEnd = m_fSlotRenderHeight;
	//--------------------------

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
