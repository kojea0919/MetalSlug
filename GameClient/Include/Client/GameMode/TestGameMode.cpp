#include "TestGameMode.h"
#include "../Object/Player.h"
#include "../Object/TestObj.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Texture.h"
#include "UI/UIObject.h"
#include "UI/UIButton.h"
#include "Scene/UIViewport.h"
#include "../UI/TestButton.h"
#include "../UI/TestBar.h"
#include "../UI/CheckBox.h"
#include "../UI/Slider.h"
#include "../UI/Inventory.h"

CTestGameMode::CTestGameMode()
{
}

CTestGameMode::~CTestGameMode()
{
}

bool CTestGameMode::Init()
{
	m_pScene->SetSequenceFileName("StageSequence1.txt");

	LoadTexture();
	LoadAnimation2D();

	// ������Ʈ ������ ȣ������ �Լ��� �ش� ��鿡 ����Ѵ�.

	CPlayer* pPlayer = m_pScene->CreateObject<CPlayer>("Player");

	// GameMode�� �÷��̾ ����Ѵ�.
	SetPlayer(pPlayer);

	SAFE_RELEASE(pPlayer);

	/*CMonster* pMonster = m_pScene->CreateObject<CMonster>("Monster");

	SAFE_RELEASE(pMonster);*/

	CTestObj* pTestObj = m_pScene->CreateObject<CTestObj>("TestObj");

	SAFE_RELEASE(pTestObj);

	CTestButton* pTestButton = m_pScene->CreateUIObject<CTestButton>("TestButton");

	SAFE_RELEASE(pTestButton);

	CTestBar* pTestBar = m_pScene->CreateUIObject<CTestBar>("TestBar");

	SAFE_RELEASE(pTestBar);

	CCheckBox* pTestCheckBox = m_pScene->CreateUIObject<CCheckBox>("TestCheckBox");

	SAFE_RELEASE(pTestCheckBox);

	CSlider* pSlider = m_pScene->CreateUIObject<CSlider>("Slider");

	SAFE_RELEASE(pSlider);

	CInventory* pInventory = m_pScene->CreateUIObject<CInventory>("Inventory");

	SAFE_RELEASE(pInventory);

	return true;
}

bool CTestGameMode::LoadTexture()
{
	CSceneResource* pManager = m_pScene->GetResourceManager();

	vector<const TCHAR*>		vecFileName;

	for (int iCnt = 0; iCnt < 13; ++iCnt)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];
		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);
		
		wsprintf(pFileName, TEXT("%d.png"), iCnt);

		vecFileName.push_back(pFileName);
	}

	pManager->LoadTexture("MouseIdle", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	pManager->LoadTexture("StartButton", TEXT("Start.png"));
	pManager->LoadTexture("UIBar", TEXT("BarDefault.bmp"));
	pManager->LoadTexture("UIBarBack", TEXT("BarBack1.png"));
	pManager->LoadTexture("UIEmptyCheckBox", TEXT("EmptyCheckBox.png"));
	pManager->LoadTexture("UISelectCheckBox", TEXT("SelectCheckBox.png"));
	pManager->LoadTexture("UICheckBack", TEXT("CheckBoxBack.png"));
	pManager->LoadTexture("UICheck", TEXT("CheckBoxCheck.png"));
	pManager->LoadTexture("UISliderBar", TEXT("SliderBar.png"));
	pManager->LoadTexture("UISliderRect", TEXT("SliderRect.png"));
	pManager->LoadTexture("CloseButton", TEXT("ExitButton.png"));

	return true;
}

bool CTestGameMode::LoadAnimation2D()
{
	CSceneResource* pManager = m_pScene->GetResourceManager();
	CTexture* pTexture = pManager->FindTexture("MouseIdle");

	pManager->CreateAnimation2DSequence("MouseIdle",
		pTexture);

	pManager->SetAnimation2DFrame("MouseIdle",
		(int)pTexture->GetImageCount());

	SAFE_RELEASE(pTexture);

	return true;
}
