#include "TextUI.h"
#include "UI/UIText.h"

CTextUI::CTextUI()
{
}

CTextUI::CTextUI(const CTextUI& ui)
    : CUIObject(ui)
{
}

CTextUI::~CTextUI()
{
    SAFE_RELEASE(m_pText);
}

bool CTextUI::Init()
{
    if (!CUIObject::Init())
        return false;

    m_pText = CreateControl<CUIText>("Text");

    m_pText->SetText(TEXT("Player"));

    SetRoot(m_pText);

    return true;
}

void CTextUI::Start()
{
    CUIObject::Start();
}

void CTextUI::Update(float fTime)
{
    CUIObject::Update(fTime);
}

void CTextUI::PostUpdate(float fTime)
{
    CUIObject::PostUpdate(fTime);
}

void CTextUI::PrevRender()
{
    CUIObject::PrevRender();
}

void CTextUI::Render()
{
    CUIObject::Render();
}

void CTextUI::PostRender()
{
    CUIObject::PostRender();
}

CTextUI* CTextUI::Clone()
{
    return new CTextUI(*this);
}
