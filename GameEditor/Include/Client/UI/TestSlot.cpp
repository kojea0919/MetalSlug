#include "TestSlot.h"
#include "UI/UISlot.h"
#include "UI/UIIcon.h"
#include "TestIcon.h"
#include "Scene/UIViewport.h"
#include "Input.h"

CTestSlot::CTestSlot()
{
}

CTestSlot::CTestSlot(const CTestSlot& ui)
	: CUIObject(ui)
{
}

CTestSlot::~CTestSlot()
{
	SAFE_RELEASE(m_pIcon);
	SAFE_RELEASE(m_pSlot);
}

bool CTestSlot::AddIcon(CTestIcon* pIcon)
{
	if (m_pIcon)
		return false;

	pIcon->AddRef();
	pIcon->SetZOrder(2);
	m_pIcon = pIcon;
	
	pIcon->SetWorldPos(m_pSlot->GetConnectPos() + m_pSlot->GetWorldPos());

	return true;
}

bool CTestSlot::Init()
{
	if (!CUIObject::Init())
		return false;

	m_pSlot = CreateControl<CUISlot>("Back");

	m_pSlot->SetRelativePos(300.f, 300.f, 0.f);
	m_pSlot->SetRelativeScale(60.f, 70.f, 0.f);
	m_pSlot->SetTexture("WeaponSlot");

	SetRoot(m_pSlot);

	return true;
}

void CTestSlot::Start()
{
	CUIObject::Start();
}

void CTestSlot::Update(float fTime)
{
	CUIObject::Update(fTime);

	//Icon에서 마우스가 Up된 경우
	//------------------------------------
	if (m_pIcon && m_pIcon->IsUp())
	{
		Vector2 vMousePos = GET_SINGLE(CInput)->GetMousePos();

		//해당 마우스 자리에 다른 Slot이 있는지 확인
		const list<class CUIObject*>& UIList= m_pViewport->GetUIList();
		auto iter = UIList.begin();
		auto iterEnd = UIList.end();

		for (; iter != iterEnd; ++iter)
		{
			if (*iter == m_pIcon)
				continue;

			CUIControl * pUI = (*iter)->UIInMouse(vMousePos);
			if (pUI)
			{
				CTestSlot* pSlot = dynamic_cast<CTestSlot*>(*iter);
				if (pSlot) 
				{
					if (pSlot == this)
					{
						m_pIcon->SetWorldPos(m_pSlot->GetConnectPos() + m_pSlot->GetWorldPos());
						return;
					}

					pSlot->AddIcon(m_pIcon);
					SAFE_RELEASE(m_pIcon);

					return;
				}
			}
		}
		m_pIcon->SetWorldPos(m_pSlot->GetConnectPos() + m_pSlot->GetWorldPos());
	}
	//------------------------------------
}

void CTestSlot::PostUpdate(float fTime)
{
	CUIObject::PostUpdate(fTime);
}

void CTestSlot::PrevRender()
{
	CUIObject::PrevRender();
}

void CTestSlot::Render()
{
	CUIObject::Render();
}

void CTestSlot::PostRender()
{
	CUIObject::PostRender();
}

CTestSlot* CTestSlot::Clone()
{
	return new CTestSlot(*this);
}
