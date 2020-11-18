#include "Inventory.h"
#include "UI/UITitleBar.h"
#include "UI/UIImage.h"
#include "UI/UIButton.h"
#include "UI/UIObject.h"
#include "UI/UISlot.h"

CInventory::CInventory()
	: m_iSlotWidthCnt(3),m_iSlotHeightCnt(6),
	m_iSlotRenderHeightCnt(4),m_iRenderStartIdx(0),
	m_iRenderMaxIdx(2)
{
}

CInventory::CInventory(const CInventory& ui)
	: CUIObject(ui),m_iRenderMaxIdx(ui.m_iRenderMaxIdx)
{
}

CInventory::~CInventory()
{
	SAFE_RELEASE(m_pBack);
	SAFE_RELEASE(m_pTitleBar);
	SAFE_RELEASE(m_pExitButton);
	SAFE_RELEASE_VECLIST(m_vecSlot);
	SAFE_RELEASE(m_pBarBack);
	SAFE_RELEASE(m_pBar);
}

void CInventory::ScrollDown()
{
	++m_iRenderStartIdx;

	//이미 끝이였던 경우 pass
	//--------------------------------------
	if (m_iRenderStartIdx > m_iRenderMaxIdx)
	{
		--m_iRenderStartIdx;
		return;
	}
	//--------------------------------------

	//아래로 한칸 내리는 작업
	//--------------------------------------
	m_pBar->AddRelativePos(0.f, (float)-m_iScrollMoveUnit, 0.f);

	size_t iSize = m_vecSlot.size();
	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		CUISlot* pSlot = m_vecSlot[iCnt];
		pSlot->AddRelativePos(0.f, (float)m_iGridMoveUnit, 0.f);

		int iHeightIdx = (int)iCnt / m_iSlotWidthCnt;
		int iWidthIdx = (int)iCnt % m_iSlotWidthCnt;
		
		//맨위에 있던 Slot들은 Enable을 false상태로
		if (m_iRenderStartIdx > iHeightIdx)
			pSlot->Enable(false);

		////새로 보여줘야하는 Slot들은 Enable을 true상태로
		else if(m_iRenderStartIdx + m_iSlotRenderHeightCnt == iHeightIdx + 1)
			pSlot->Enable(true);
	}
	//--------------------------------------
}

void CInventory::ScrollUp()
{
	--m_iRenderStartIdx;

	//이미 끝이였던 경우 pass
	//--------------------------------------
	if (m_iRenderStartIdx < 0)
	{
		++m_iRenderStartIdx;
		return;
	}
	//--------------------------------------

	//위로 한칸 올리는 작업
	//--------------------------------------
	m_pBar->AddRelativePos(0.f, (float)m_iScrollMoveUnit, 0.f);

	size_t iSize = m_vecSlot.size();
	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		CUISlot* pSlot = m_vecSlot[iCnt];
		pSlot->AddRelativePos(0.f, (float)-m_iGridMoveUnit, 0.f);

		int iHeightIdx = (int)iCnt / m_iSlotWidthCnt;
		int iWidthIdx = (int)iCnt % m_iSlotWidthCnt;

		//새로 보여줘야하는 Slot들
		if (m_iRenderStartIdx == iHeightIdx)
			pSlot->Enable(true);

		//맨아래 있던 Slot들
		else if(m_iRenderStartIdx + m_iSlotRenderHeightCnt <= iHeightIdx)
			pSlot->Enable(false);
	}
	//--------------------------------------
}

bool CInventory::Init()
{
	if (!CUIObject::Init())
		return false;

	SetZOrder(10);

	m_pBack = CreateControl<CUIImage>("Back");
	m_pTitleBar = CreateControl<CUITitleBar>("TitleBar");
	m_pExitButton = CreateControl<CUIButton>("CloseButton");

	size_t iSize = (size_t)(m_iSlotWidthCnt * m_iSlotHeightCnt);
	m_vecSlot.resize(iSize);
	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		m_vecSlot[iCnt] = CreateControl<CUISlot>("Slot");
	}

	m_pBarBack = CreateControl<CUIImage>("ScrollBarBack");
	m_pBar = CreateControl<CUIImage>("ScrollBar");

	m_pBack->SetRelativePos(50.f, 100.f, 0.f);
	m_pBack->SetRelativeScale(254.f, 343.f, 1.f);
	m_pBack->SetTexture("InventoryBackImage");

	//Title Bar 기본 Setting
	//------------------------------------------
	m_pTitleBar->SetInheritScale(false);
	m_pTitleBar->SetRelativePos(5.f, 308.f, 0.f);
	m_pTitleBar->SetRelativeScale(245.f, 31.f, 1.f);
	m_pTitleBar->SetTexture("Inventorytitlebar");
	m_pTitleBar->SetZOrder(1);
	//------------------------------------------

	//ExitButton 기본 Setting
	//------------------------------------------
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
	//------------------------------------------

	//SlotGrid 기본 Setting
	//------------------------------------------
	iSize = m_vecSlot.size();
	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		CUISlot* pSlotImage = m_vecSlot[iCnt];

		pSlotImage->SetInheritScale(false);
		pSlotImage->SetTexture("InventorySlot");
		pSlotImage->SetZOrder(2);
		pSlotImage->SetRelativeScale(75.f, 75.f, 0.f);


		//위치 Setting
		//--------------------------------------
		int iHeightIdx = (int)iCnt / m_iSlotWidthCnt;
		int iWidthIdx = (int)iCnt % m_iSlotWidthCnt;

		pSlotImage->SetRelativePos(5.f + 75.f * iWidthIdx, 233.f - iHeightIdx * 75.f, 0.f);
		//--------------------------------------

		m_pBack->AddChild(pSlotImage);

		//출력 대상이 아닌 것들 처리
		//--------------------------------------
		if (iHeightIdx + 1 > m_iSlotRenderHeightCnt)
		{
			pSlotImage->Enable(false);
		}
		//--------------------------------------
	}

	m_iGridMoveUnit = 75;
	//------------------------------------------

	//Bar Back Image 기본 Setting
	//------------------------------------------
	m_pBarBack->SetInheritScale(false);
	m_pBarBack->SetTexture("InventoryScrollBar");
	m_pBarBack->SetZOrder(2);
	m_pBarBack->SetRelativeScale(17.f, 299.f, 0.f);
	m_pBarBack->SetRelativePos(230.f, 8.f, 0.f);
	//------------------------------------------

	//Scroll이 움직일 수 있는 길이
	m_iCanMoveScrollLen = 268;


	//Scrol 기본 Setting
	//------------------------------------------

	m_pBar->SetInheritScale(false);
	m_pBar->SetTexture("InventoryScroll");

	//스크롤의 세로 크기는 현재 출력되는 세로 슬롯의 개수 / 총 세로 슬롯의 개수
	//에 비례해서 Setting
	float fRate = m_iSlotRenderHeightCnt / (float)m_iSlotHeightCnt;
	float fScrollYLen = fRate * m_iCanMoveScrollLen;
	m_pBar->SetRelativeScale(15.f, fScrollYLen,0.f);
	m_pBar->SetRelativePos(1.f, m_iCanMoveScrollLen - fScrollYLen + 15.f, 0.f);
	m_pBar->SetZOrder(3);

	//한번 스크롤 됐을때 움직여야할 크기
	m_iScrollMoveUnit = m_iCanMoveScrollLen / m_iSlotHeightCnt;

	//------------------------------------------


	SetRoot(m_pBack);

	m_pBack->AddChild(m_pTitleBar);
	m_pBack->AddChild(m_pExitButton);
	m_pBack->AddChild(m_pBarBack);
	m_pBarBack->AddChild(m_pBar);
	
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
