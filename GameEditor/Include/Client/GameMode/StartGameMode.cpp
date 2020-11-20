#include "StartGameMode.h"
#include "Scene/UIViewport.h"
#include "../UI/StartBackObj.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Texture.h"

CStartGameMode::CStartGameMode()
{
}

CStartGameMode::~CStartGameMode()
{
}

bool CStartGameMode::Init()
{
	LoadTexture();
	LoadAnimation2D();
	LoadSound();

	CStartBackObj* pBack = m_pScene->CreateUIObject<CStartBackObj>("Back");

	pBack->AddToViewport();

	SAFE_RELEASE(pBack);

	return true;
}

bool CStartGameMode::LoadTexture()
{
	CSceneResource* pManager = m_pScene->GetResourceManager();

	pManager->LoadTexture("StartButton", TEXT("Start.png"));
	pManager->LoadTexture("ExitButton", TEXT("End.png"));
	pManager->LoadTexture("StartBack", TEXT("Teemo.jpg"));

	return true;
}

bool CStartGameMode::LoadAnimation2D()
{
	return true;
}

bool CStartGameMode::LoadSound()
{
	CSceneResource* pManager = m_pScene->GetResourceManager();

	pManager->LoadSound("UI", false, "ButtonMouseOn", "1Up.wav");
	pManager->LoadSound("UI", false, "ButtonClick", "Stun.wav");

	return true;
}
