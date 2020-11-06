#include "UICheckBox.h"

CUICheckBox::CUICheckBox()
	: m_bIsCheck(false)
{
}

CUICheckBox::CUICheckBox(const CUICheckBox& control)
	: CUIButton(control)
{
}

CUICheckBox::~CUICheckBox()
{
}

bool CUICheckBox::Init()
{
	if (!CUIButton::Init())
		return false;

	return true;
}

void CUICheckBox::Start()
{
	CUIButton::Start();
}

void CUICheckBox::Update(float fTime)
{
	CUIButton::Update(fTime);
}

void CUICheckBox::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);

	//���콺 �Է¿� ���� ��ư ���� update
	//-------------------------------
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		//ó�� ������ ���
		//-----------------------
		if (!m_bDown && !m_bPush)
			m_bDown = true;
		//-----------------------

		//ó�� ���� ������ �ִ� ���
		//-----------------------
		else if (!m_bPush)
		{
			m_bDown = false;
			m_bPush = true;
		}
		//-----------------------
	}

	//ó�� Ű�� �� ���
	//--------------------------
	else if (m_bPush || m_bDown)
	{
		m_bUp = true;
		m_bPush = false;
		m_bDown = false;
	}
	//--------------------------

	//Ű�� �� ���� �Է��� ���� ���
	else if (m_bUp)
		m_bUp = false;

	if (m_eState == Button_State::Click)
	{
		if (m_bDown)
			m_eState = Button_State::Normal;
	}
	else if (m_eState == Button_State::Normal)
	{
		if (m_bDown)
			m_eState = Button_State::Click;
	}
}

void CUICheckBox::PrevRender()
{
	CUIButton::PrevRender();
}

void CUICheckBox::Render()
{
	CUIButton::Render();
}

void CUICheckBox::PostRender()
{
	CUIButton::PostRender();
}

CUICheckBox* CUICheckBox::Clone()
{
	return new CUICheckBox(*this);
}

void CUICheckBox::CollisionMouse(const Vector2& vMousePos, float fTime)
{
}

void CUICheckBox::CollisionReleaseMouse(const Vector2& vMousePos, float fTime)
{
}
