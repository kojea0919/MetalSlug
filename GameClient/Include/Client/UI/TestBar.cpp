#include "TestBar.h"
#include "UI/UIProgressBar.h"
#include "UI/UIImage.h"

CTestBar::CTestBar()
{
}

CTestBar::CTestBar(const CTestBar& ui)
	: CUIObject(ui)
{
}

CTestBar::~CTestBar()
{
	SAFE_RELEASE(m_pBack);
	SAFE_RELEASE(m_pBar);
}

bool CTestBar::Init()
{
	if (!CUIObject::Init())
		return false;

	m_pBack = CreateControl<CUIImage>("Back");
	m_pBar = CreateControl<CUIProgressBar>("Bar");

	m_pBack->SetRelativePos(15.f, 605.f, 0.f);
	m_pBack->SetRelativeScale(210.f, 40.f, 1.f);
	m_pBack->SetTexture("UIBarBack");

	m_pBar->SetInheritScale(false);
	m_pBar->SetRelativePos(5.f, 5.f, 0.f);
	m_pBar->SetRelativeScale(200.f, 30.f, 1.f);
	m_pBar->SetTexture("UIBar");
	m_pBar->SetZOrder(1); 
	//m_pBar->SetBarDir(BAR_DIR::RightLeft);
	//m_pBar->SetBarDir(BAR_DIR::BottomTop);
	m_pBar->SetBarDir(BAR_DIR::TopBottom);

	SetRoot(m_pBack);

	m_pBack->AddChild(m_pBar);

	return true;
}

void CTestBar::Start()
{
	CUIObject::Start();
}

void CTestBar::Update(float fTime)
{
	CUIObject::Update(fTime);
	static float	fPercent = 1.f;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		fPercent -= fTime / 3.f;

		if (fPercent < 0.f)
			fPercent = 0.f;

		m_pBar->SetPercent(fPercent);
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		fPercent += fTime / 3.f;

		if (fPercent > 1.f)
			fPercent = 1.f;

		m_pBar->SetPercent(fPercent);
	}
}

void CTestBar::PostUpdate(float fTime)
{
	CUIObject::PostUpdate(fTime);
}

void CTestBar::PrevRender()
{
	CUIObject::PrevRender();
}

void CTestBar::Render()
{
	CUIObject::Render();
}

void CTestBar::PostRender()
{
	CUIObject::PostRender();
}

CTestBar* CTestBar::Clone()
{
	return new CTestBar(*this);
}
