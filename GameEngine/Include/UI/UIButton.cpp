#include "UIButton.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Texture.h"
#include "../Resource/Material.h"
#include "../Resource/Sound.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CUIButton::CUIButton()
	: m_bDown(false), m_bPush(false) ,m_bUp(false)
{
	m_vecButtonState.resize((size_t)Button_State::End);

	m_vecSound.resize(SOUND_END);
	m_vecSound[SOUND_MOUSEON] = nullptr;
	m_vecSound[SOUND_CLICK] = nullptr;
}

CUIButton::CUIButton(const CUIButton& control)
	: CUIControl(control)
{
}

CUIButton::~CUIButton()
{
	SAFE_RELEASE_VECLIST(m_vecSound);

	for (size_t iCnt = 0; iCnt < (size_t)Button_State::End; ++iCnt)
	{
		SAFE_RELEASE(m_vecButtonState[iCnt].pTexture);
	}
}

void CUIButton::SetButtonStateTexture(Button_State eState, const string& strName)
{
	SAFE_RELEASE(m_vecButtonState[(int)eState].pTexture);

	m_vecButtonState[(int)eState].pTexture = m_pScene->GetResourceManager()->FindTexture(strName);
}

void CUIButton::SetButtonStateColor(Button_State eState, const Vector4& vColor)
{
	m_vecButtonState[(int)eState].vColor = vColor;
}

void CUIButton::SetButtonStateColor(Button_State eState, BYTE r, BYTE g, BYTE b, BYTE a)
{
	m_vecButtonState[(int)eState].vColor.x = r / 255.f;
	m_vecButtonState[(int)eState].vColor.y = g / 255.f;
	m_vecButtonState[(int)eState].vColor.z = b / 255.f;
	m_vecButtonState[(int)eState].vColor.w = a / 255.f;
}

void CUIButton::SetButtonSound(BUTTON_SOUND eSound, const string& strName)
{
	SAFE_RELEASE(m_vecSound[eSound]);
	m_vecSound[eSound] = m_pScene->GetResourceManager()->FindSound(strName);
}

bool CUIButton::Init()
{
	if (!CUIControl::Init())
		return false;

	SetRelativeScale(100.f, 50.f, 1.f);

	return true;
}

void CUIButton::Start()
{
	CUIControl::Start();
}

void CUIButton::Update(float fTime)
{
	CUIControl::Update(fTime);
}

void CUIButton::PostUpdate(float fTime)
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

	if (m_eState == Button_State::MouseOn)
	{
		if (m_bDown)
		{
			m_eState = Button_State::Click;
		}
	}

	else if (m_eState == Button_State::Click)
	{
		if (m_bUp && m_ClickCallback)
		{
			if (m_vecSound[SOUND_CLICK])
				m_vecSound[SOUND_CLICK]->Play();
			m_ClickCallback();
			m_eState = Button_State::MouseOn;
		}
	}
	//-------------------------------
}

void CUIButton::PrevRender()
{
	CUIControl::PrevRender();
}

void CUIButton::Render()
{
	CUIControl::Render();

	m_vecButtonState[(int)m_eState].pTexture->SetShader(0, (int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL);
	m_pMaterial->SetDiffuseColor(m_vecButtonState[(int)m_eState].vColor);
	m_pMaterial->SetMaterial();

	m_pBodyMesh->Render(0.f);
}

void CUIButton::PostRender()
{
	CUIControl::PostRender();
}

CUIButton* CUIButton::Clone()
{
	return new CUIButton(*this);
}

void CUIButton::CollisionMouse(const Vector2& vMousePos, float fTime)
{
	CUIControl::CollisionMouse(vMousePos, fTime);

	if (m_eState != Button_State::Disable)
	{
		m_eState = Button_State::MouseOn;

		if (m_vecSound[SOUND_MOUSEON])
			m_vecSound[SOUND_MOUSEON]->Play();
	}
}

void CUIButton::CollisionReleaseMouse(const Vector2& vMousePos, float fTime)
{
	CUIControl::CollisionReleaseMouse(vMousePos, fTime);

	if (m_eState != Button_State::Disable)
		m_eState = Button_State::Normal;
}
