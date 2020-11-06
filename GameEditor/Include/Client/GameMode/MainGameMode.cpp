#include "MainGameMode.h"
#include "../Object/Player.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Texture.h"
#include "../Object/Monster.h"
#include "PathManager.h"
#include "Resource/ResourceManager.h"
#include "Resource/Animation2DSequence.h"
#include "../Object/TestObj.h"

CMainGameMode::CMainGameMode()
	: m_iCurrentStage(1)
{
}

CMainGameMode::~CMainGameMode()
{
}

bool CMainGameMode::Init()
{
	LoadTexture();

	// ������Ʈ ������ ȣ������ �Լ��� �ش� ��鿡 ����Ѵ�.

	CPlayer* pPlayer = m_pScene->CreateObject<CPlayer>("Player");

	// GameMode�� �÷��̾ ����Ѵ�.
	SetPlayer(pPlayer);

	SAFE_RELEASE(pPlayer);

	/*CMonster* pMonster = m_pScene->CreateObject<CMonster>("Monster");

	SAFE_RELEASE(pMonster);*/

	CTestObj* pTestObj = m_pScene->CreateObject<CTestObj>("TestObj");

	SAFE_RELEASE(pTestObj);

	return true;
}

bool CMainGameMode::LoadAnimation2DSequence()
{
	//���� ��� Setting
	//----------------------------------------------
	char strFullPath[MAX_PATH] = {};
	const char* pPath = GET_SINGLE(CPathManager)->FindPathMultibyte(RESOURCE_PATH);

	if (pPath)
		strcpy_s(strFullPath, pPath);
	else
		return false;

	//���� Stage�� ���� �����̸� Setting
	//-------------------------------------------
	string strFileName = "MainGameMode_Stage" + to_string(m_iCurrentStage);
	strFileName += ".txt";

	strcat_s(strFullPath, strFileName.c_str());
	//-------------------------------------------

	//----------------------------------------------

	//���� Open
	//----------------------------------------------
	FILE* pFile = nullptr;
	
	fopen_s(&pFile, strFullPath, "rt");
	if (!pFile)
		return false;

	LoadAnimation2D(pFile);

	fclose(pFile);
	//----------------------------------------------

	return true;
}

bool CMainGameMode::LoadAnimation2D(FILE* pFile)
{
	if (!pFile)
		return false;

	//Load�ؾ��� ���� ����
	//---------------------
	int iSize = 0;

	fscanf_s(pFile, "%d\n", &iSize);
	//---------------------

	//�� ani���� Load
	//---------------------
	
	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		char strFileName[MAX_PATH] = {};

		fscanf_s(pFile, "%s\n", strFileName,MAX_PATH);

		LoadAniFile(strFileName);
	}
	//---------------------

	return true;
}

bool CMainGameMode::LoadAniFile(const char* pAniFileName)
{
	if (!pAniFileName)
		return false;

	//�ؽ�ó �ε�
	GET_SINGLE(CResourceManager)->LoadAnimation2DSequence(pAniFileName);

	return true;
}

bool CMainGameMode::LoadTexture()
{
	CSceneResource* pManager = m_pScene->GetResourceManager();

	pManager->LoadTexture("PixelTest", TEXT("PixelCollision.png"));

	return true;
}

bool CMainGameMode::LoadAnimation2D()
{
	return true;

	CSceneResource* pManager = m_pScene->GetResourceManager();

	CTexture* pTexture = pManager->FindTexture("PlayerAtlas");

	pManager->CreateAnimation2DSequence("PlayerIdle",
		pTexture);

	for (int i = 0; i < 1; ++i)
	{
		pManager->AddAnimation2DFrame("PlayerIdle",
			Vector2(i * 50.f, 0.f), Vector2((i + 1) * 50.f, 37.f));
	}

	SAFE_RELEASE(pTexture);

	pTexture = pManager->FindTexture("PlayerRun");

	pManager->CreateAnimation2DSequence("PlayerRun",
		pTexture);

	pManager->SetAnimation2DFrame("PlayerRun",
		(int)pTexture->GetImageCount());

	SAFE_RELEASE(pTexture);

	// Player Attack
	pTexture = pManager->FindTexture("PlayerAttack");

	pManager->CreateAnimation2DSequence("PlayerAttack",
		pTexture);

	pManager->SetAnimation2DFrame("PlayerAttack",
		(int)pTexture->GetImageCount());

	pManager->AddAnimation2DNotify("PlayerAttack",
		"Attack", 2);

	SAFE_RELEASE(pTexture);

	pTexture = pManager->FindTexture("MonsterAtlas");

	pManager->CreateAnimation2DSequence("MonsterIdle",
		pTexture);

	for (int i = 0; i < 14; ++i)
	{
		pManager->AddAnimation2DFrame("MonsterIdle",
			Vector2(i * 45.f, 60.f), Vector2((i + 1) * 45.f, 120.f));
	}

	SAFE_RELEASE(pTexture);

	pTexture = pManager->FindTexture("MonsterAtlas");

	pManager->CreateAnimation2DSequence("MonsterRun",
		pTexture);

	for (int i = 0; i < 8; ++i)
	{
		pManager->AddAnimation2DFrame("MonsterRun",
			Vector2(i * 45.f, 300.f), Vector2((i + 1) * 45.f, 360.f));
	}

	SAFE_RELEASE(pTexture);

	pTexture = pManager->FindTexture("MonsterAtlas");

	pManager->CreateAnimation2DSequence("MonsterAttack",
		pTexture);

	for (int i = 0; i < 21; ++i)
	{
		pManager->AddAnimation2DFrame("MonsterAttack",
			Vector2(i * 45.f, 180.f), Vector2((i + 1) * 45.f, 240.f));
	}

	SAFE_RELEASE(pTexture);

	pManager->AddAnimation2DNotify("MonsterAttack",
		"Attack", 8);

	return true;
}
