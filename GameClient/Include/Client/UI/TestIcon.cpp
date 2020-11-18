#include "TestIcon.h"
#include "UI/UIIcon.h"


CTestIcon::CTestIcon()
{
}

CTestIcon::CTestIcon(const CTestIcon& ui)
	: CUIObject(ui)
{
}

CTestIcon::~CTestIcon()
{
	SAFE_RELEASE(m_pIcon);
}

bool CTestIcon::IsUp() const
{
	return m_pIcon->IsUp();
}

bool CTestIcon::IsAttachMouse() const
{
	return m_pIcon->IsAttachMouse();
}

void CTestIcon::DetatchMouse()
{
	m_pIcon->DetatchMouse();
}

bool CTestIcon::Init()
{
	if (!CUIObject::Init())
		return false;

	m_pIcon = CreateControl<CUIIcon>("Icon");

	//m_pIcon->SetRelativePos(150.f, 100.f, 0.f);
	m_pIcon->SetRelativeScale(45.f, 45.f, 0.f);
	m_pIcon->SetTexture("WeaponIcon");

	SetRoot(m_pIcon);

	return true;
}

void CTestIcon::Start()
{
	CUIObject::Start();
}

void CTestIcon::Update(float fTime)
{
	CUIObject::Update(fTime);
}

void CTestIcon::PostUpdate(float fTime)
{
	CUIObject::PostUpdate(fTime);
}

void CTestIcon::PrevRender()
{
	CUIObject::PrevRender();
}

void CTestIcon::Render()
{
	CUIObject::Render();
}

void CTestIcon::PostRender()
{
	CUIObject::PostRender();
}

CTestIcon* CTestIcon::Clone()
{
	return new CTestIcon(*this);
}
