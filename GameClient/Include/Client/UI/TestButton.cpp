#include "TestButton.h"
#include "UI/UIButton.h"
#include "PathManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"

CTestButton::CTestButton()
{
}

CTestButton::CTestButton(const CTestButton& ui)
	: CUIObject(ui)
{
}

CTestButton::~CTestButton()
{
	SAFE_RELEASE(m_pButton);
}

bool CTestButton::Init()
{
	if (!CUIObject::Init())
		return false;

	m_pButton = CreateControl<CUIButton>("Button");

	m_pButton->SetButtonStateTexture(Button_State::Normal, "StartButton");
	m_pButton->SetButtonStateTexture(Button_State::MouseOn, "StartButton");
	m_pButton->SetButtonStateTexture(Button_State::Click, "StartButton");
	m_pButton->SetButtonStateTexture(Button_State::Disable, "StartButton");

	m_pButton->SetButtonStateColor(Button_State::Normal, Vector4(0.8f, 0.8f, 0.8f, 1.f));
	m_pButton->SetButtonStateColor(Button_State::MouseOn, Vector4(1.f, 1.f, 1.f, 1.f));
	m_pButton->SetButtonStateColor(Button_State::Click, Vector4(0.6f, 0.6f, 0.6f, 1.f));
	m_pButton->SetButtonStateColor(Button_State::Disable, Vector4(0.2f, 0.2f, 0.2f, 1.f));

	m_pButton->SetRelativePos(500.f, 400.f, 0.f);

	SetRoot(m_pButton);

	m_pButton->SetClickCallback<CTestButton>(this, &CTestButton::ButtonCallback);


	//testcode
	//--------------------------------
	//const char* pPath = GET_SINGLE(CPathManager)->FindPathMultibyte(SOUND_PATH);
	//if (!pPath)
	//	return false;

	//char strFullPath[MAX_PATH] = {};
	//strcpy_s(strFullPath, pPath);
	//strcat_s(strFullPath, "ButtonSound.mp3");

	//m_pScene->GetResourceManager()->LoadSound("UI", false, "ButtonClickSound",
	//	strFullPath);

	//
	//m_pButton->SetButtonSound(BUTTON_SOUND::SOUND_CLICK, "ButtonClickSound");
	//--------------------------------


	return true;
}

void CTestButton::Start()
{
	CUIObject::Start();
}

void CTestButton::Update(float fTime)
{
	CUIObject::Update(fTime);
}

void CTestButton::PostUpdate(float fTime)
{
	CUIObject::PostUpdate(fTime);
}

void CTestButton::PrevRender()
{
	CUIObject::PrevRender();
}

void CTestButton::Render()
{
	CUIObject::Render();
}

void CTestButton::PostRender()
{
	CUIObject::PostRender();
}

CTestButton* CTestButton::Clone()
{
	return new CTestButton(*this);
}

void CTestButton::ButtonCallback()
{
}
