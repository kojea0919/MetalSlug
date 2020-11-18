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

	//�̹� ���̿��� ��� pass
	//--------------------------------------
	if (m_iRenderStartIdx > m_iRenderMaxIdx)
	{
		--m_iRenderStartIdx;
		return;
	}
	//--------------------------------------

	//�Ʒ��� ��ĭ ������ �۾�
	//--------------------------------------
	m_pBar->AddRelativePos(0.f, (float)-m_iScrollMoveUnit, 0.f);

	size_t iSize = m_vecSlot.size();
	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		CUISlot* pSlot = m_vecSlot[iCnt];
		pSlot->AddRelativePos(0.f, (float)m_iGridMoveUnit, 0.f);

		int iHeightIdx = (int)iCnt / m_iSlotWidthCnt;
		int iWidthIdx = (int)iCnt % m_iSlotWidthCnt;
		
		//������ �ִ� Slot���� Enable�� false���·�
		if (m_iRenderStartIdx > iHeightIdx)
			pSlot->Enable(false);

		////���� ��������ϴ� Slot���� Enable�� true���·�
		else if(m_iRenderStartIdx + m_iSlotRenderHeightCnt == iHeightIdx + 1)
			pSlot->Enable(true);
	}
	//--------------------------------------
}

void CInventory::ScrollUp()
{
	--m_iRenderStartIdx;

	//�̹� ���̿��� ��� pass
	//--------------------------------------
	if (m_iRenderStartIdx < 0)
	{
		++m_iRenderStartIdx;
		return;
	}
	//--------------------------------------

	//���� ��ĭ �ø��� �۾�
	//--------------------------------------
	m_pBar->AddRelativePos(0.f, (float)m_iScrollMoveUnit, 0.f);

	size_t iSize = m_vecSlot.size();
	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		CUISlot* pSlot = m_vecSlot[iCnt];
		pSlot->AddRelativePos(0.f, (float)-m_iGridMoveUnit, 0.f);

		int iHeightIdx = (int)iCnt / m_iSlotWidthCnt;
		int iWidthIdx = (int)iCnt % m_iSlotWidthCnt;

		//���� ��������ϴ� Slot��
		if (m_iRenderStartIdx == iHeightIdx)
			pSlot->Enable(true);

		//�ǾƷ� �ִ� Slot��
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

	//Title Bar �⺻ Setting
	//------------------------------------------
	m_pTitleBar->SetInheritScale(false);
	m_pTitleBar->SetRelativePos(5.f, 308.f, 0.f);
	m_pTitleBar->SetRelativeScale(245.f, 31.f, 1.f);
	m_pTitleBar->SetTexture("Inventorytitlebar");
	m_pTitleBar->SetZOrder(1);
	//------------------------------------------

	//ExitButton �⺻ Setting
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

	//SlotGrid �⺻ Setting
	//------------------------------------------
	iSize = m_vecSlot.size();
	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		CUISlot* pSlotImage = m_vecSlot[iCnt];

		pSlotImage->SetInheritScale(false);
		pSlotImage->SetTexture("InventorySlot");
		pSlotImage->SetZOrder(2);
		pSlotImage->SetRelativeScale(75.f, 75.f, 0.f);


		//��ġ Setting
		//--------------------------------------
		int iHeightIdx = (int)iCnt / m_iSlotWidthCnt;
		int iWidthIdx = (int)iCnt % m_iSlotWidthCnt;

		pSlotImage->SetRelativePos(5.f + 75.f * iWidthIdx, 233.f - iHeightIdx * 75.f, 0.f);
		//--------------------------------------

		m_pBack->AddChild(pSlotImage);

		//��� ����� �ƴ� �͵� ó��
		//--------------------------------------
		if (iHeightIdx + 1 > m_iSlotRenderHeightCnt)
		{
			pSlotImage->Enable(false);
		}
		//--------------------------------------
	}

	m_iGridMoveUnit = 75;
	//------------------------------------------

	//Bar Back Image �⺻ Setting
	//------------------------------------------
	m_pBarBack->SetInheritScale(false);
	m_pBarBack->SetTexture("InventoryScrollBar");
	m_pBarBack->SetZOrder(2);
	m_pBarBack->SetRelativeScale(17.f, 299.f, 0.f);
	m_pBarBack->SetRelativePos(230.f, 8.f, 0.f);
	//------------------------------------------

	//Scroll�� ������ �� �ִ� ����
	m_iCanMoveScrollLen = 268;


	//Scrol �⺻ Setting
	//------------------------------------------

	m_pBar->SetInheritScale(false);
	m_pBar->SetTexture("InventoryScroll");

	//��ũ���� ���� ũ��� ���� ��µǴ� ���� ������ ���� / �� ���� ������ ����
	//�� ����ؼ� Setting
	float fRate = m_iSlotRenderHeightCnt / (float)m_iSlotHeightCnt;
	float fScrollYLen = fRate * m_iCanMoveScrollLen;
	m_pBar->SetRelativeScale(15.f, fScrollYLen,0.f);
	m_pBar->SetRelativePos(1.f, m_iCanMoveScrollLen - fScrollYLen + 15.f, 0.f);
	m_pBar->SetZOrder(3);

	//�ѹ� ��ũ�� ������ ���������� ũ��
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
