#include "UISlider.h"
#include "../Input.h"
#include "../Resource/Texture.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Scene.h"

CUISlider::CUISlider()
	: m_fCurRate(0.5f),m_pBarImage(nullptr)
{
}

CUISlider::CUISlider(const CUISlider& control)
	: CUIButton(control)
{
}

CUISlider::~CUISlider()
{
}

bool CUISlider::Init()
{
	if (!CUIButton::Init())
		return false;

	return true;
}

void CUISlider::Start()
{
	CUIButton::Start();
}

void CUISlider::Update(float fTime)
{
	CUIButton::Update(fTime);
}

void CUISlider::PostUpdate(float fTime)
{
	CUIButton::PostUpdate(fTime);

	//처음 버튼이 눌렸을 때 마우스 위치 저장
	if (m_bDown)
	{
		m_vMousePos = GET_SINGLE(CInput)->GetMousePos();
	}
	//드래그된 경우 Rate반영
	else if(m_eState == Button_State::Click)
	{
		//변화율 계산
		//------------------------------------------------------
		Vector2 vNewMousePos = GET_SINGLE(CInput)->GetMousePos();
		Vector2 vDiff = vNewMousePos - m_vMousePos;
		//------------------------------------------------------

		float fAddRate = vDiff.x / (float)(m_iMaxX - m_iMinX);
		m_fCurRate += fAddRate;
		if (m_fCurRate > 1.f)
		{
			m_fCurRate = 1.f;
			SetWorldPos(Vector3(m_iMaxX, GetWorldPos().y, 0.f));
		}
		else if (m_fCurRate < 0.f)
		{
			m_fCurRate = 0.f;
			SetWorldPos(Vector3(m_iMinX, GetWorldPos().y, 0.f));
		}
		else
			AddWorldPos(Vector3(vDiff.x, 0.f, 0.f));
		m_vMousePos = vNewMousePos;
	}
}

void CUISlider::PrevRender()
{
	CUIButton::PrevRender();
}

void CUISlider::Render()
{
	CUIButton::Render();
}

void CUISlider::PostRender()
{
	CUIButton::PostRender();
}

CUISlider* CUISlider::Clone()
{
	return new CUISlider(*this);
}
