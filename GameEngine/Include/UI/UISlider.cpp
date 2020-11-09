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

	//ó�� ��ư�� ������ �� ���콺 ��ġ ����
	if (m_bDown)
	{
		m_vMousePos = GET_SINGLE(CInput)->GetMousePos();
	}
	//�巡�׵� ��� Rate�ݿ�
	else if(m_eState == Button_State::Click)
	{
		//��ȭ�� ���
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
