#include "StartBackObj.h"
#include "UI/UIImage.h"
#include "UI/UIButton.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../GameMode/MainGameMode.h"
#include "../GameMode/TestGameMode.h"

CStartBackObj::CStartBackObj()
{
}

CStartBackObj::CStartBackObj(const CStartBackObj& ui)
	: CUIObject(ui)
{
}

CStartBackObj::~CStartBackObj()
{
	SAFE_RELEASE(m_pBack);
	SAFE_RELEASE(m_pStartButton);
	SAFE_RELEASE(m_pExitButton);
}

bool CStartBackObj::Init()
{
	if (!CUIObject::Init())
		return false;

	m_pBack = CreateControl<CUIImage>("Back");
	m_pStartButton = CreateControl<CUIButton>("StartButton");
	m_pExitButton = CreateControl<CUIButton>("ExitButton");

	m_pStartButton->SetZOrder(1);
	m_pExitButton->SetZOrder(1);

	Resolution	tRS = RESOLUTION;

	m_pBack->SetTexture("StartBack");
	m_pBack->SetRelativeScale((float)tRS.iWidth, (float)tRS.iHeight, 1.f);

	SetRoot(m_pBack);

	m_pBack->AddChild(m_pStartButton);
	m_pBack->AddChild(m_pExitButton);

	m_pStartButton->SetButtonStateTexture(Button_State::Normal, "StartButton");
	m_pStartButton->SetButtonStateTexture(Button_State::MouseOn, "StartButton");
	m_pStartButton->SetButtonStateTexture(Button_State::Click, "StartButton");
	m_pStartButton->SetButtonStateTexture(Button_State::Disable, "StartButton");

	m_pStartButton->SetButtonStateColor(Button_State::Normal, Vector4(0.8f, 0.8f, 0.8f, 1.f));
	m_pStartButton->SetButtonStateColor(Button_State::MouseOn, Vector4(1.f, 1.f, 1.f, 1.f));
	m_pStartButton->SetButtonStateColor(Button_State::Click, Vector4(0.6f, 0.6f, 0.6f, 1.f));
	m_pStartButton->SetButtonStateColor(Button_State::Disable, Vector4(0.2f, 0.2f, 0.2f, 1.f));

	m_pStartButton->SetButtonSound(BUTTON_SOUND::SOUND_MOUSEON, "ButtonMouseOn");
	m_pStartButton->SetButtonSound(BUTTON_SOUND::SOUND_CLICK, "ButtonClick");

	Vector3	vButtonPos;
	vButtonPos.x = tRS.iWidth / 2.f - 100.f;
	vButtonPos.y = tRS.iHeight / 2.f + 50.f;

	m_pStartButton->SetRelativePos(vButtonPos);
	m_pStartButton->SetRelativeScale(200.f, 100.f, 1.f);


	m_pExitButton->SetButtonStateTexture(Button_State::Normal, "ExitButton");
	m_pExitButton->SetButtonStateTexture(Button_State::MouseOn, "ExitButton");
	m_pExitButton->SetButtonStateTexture(Button_State::Click, "ExitButton");
	m_pExitButton->SetButtonStateTexture(Button_State::Disable, "ExitButton");

	m_pExitButton->SetButtonStateColor(Button_State::Normal, Vector4(0.8f, 0.8f, 0.8f, 1.f));
	m_pExitButton->SetButtonStateColor(Button_State::MouseOn, Vector4(1.f, 1.f, 1.f, 1.f));
	m_pExitButton->SetButtonStateColor(Button_State::Click, Vector4(0.6f, 0.6f, 0.6f, 1.f));
	m_pExitButton->SetButtonStateColor(Button_State::Disable, Vector4(0.2f, 0.2f, 0.2f, 1.f));

	m_pExitButton->SetButtonSound(BUTTON_SOUND::SOUND_MOUSEON, "ButtonMouseOn");
	m_pExitButton->SetButtonSound(BUTTON_SOUND::SOUND_CLICK, "ButtonClick");

	vButtonPos.x = tRS.iWidth / 2.f - 100.f;
	vButtonPos.y = tRS.iHeight / 2.f - 150.f;

	m_pExitButton->SetRelativePos(vButtonPos);
	m_pExitButton->SetRelativeScale(200.f, 100.f, 1.f);


	m_pStartButton->SetClickCallback<CStartBackObj>(this, &CStartBackObj::StartButtonCallback);
	m_pExitButton->SetClickCallback<CStartBackObj>(this, &CStartBackObj::ExitButtonCallback);

	return true;
}

void CStartBackObj::Start()
{
	CUIObject::Start();
}

void CStartBackObj::Update(float fTime)
{
	CUIObject::Update(fTime);
}

void CStartBackObj::PostUpdate(float fTime)
{
	CUIObject::PostUpdate(fTime);
}

void CStartBackObj::PrevRender()
{
	CUIObject::PrevRender();
}

void CStartBackObj::Render()
{
	CUIObject::Render();
}

void CStartBackObj::PostRender()
{
	CUIObject::PostRender();
}

CStartBackObj* CStartBackObj::Clone()
{
	return new CStartBackObj(*this);
}

void CStartBackObj::StartButtonCallback()
{
	GET_SINGLE(CSceneManager)->CreateNextScene();

	GET_SINGLE(CSceneManager)->SetGameMode<CTestGameMode>(false);
}

void CStartBackObj::ExitButtonCallback()
{
}
