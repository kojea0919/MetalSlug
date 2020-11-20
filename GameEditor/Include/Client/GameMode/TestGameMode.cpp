#include "TestGameMode.h"
#include "../Object/Player.h"
#include "../Object/TestObj.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Texture.h"
#include "Resource/Material.h"
#include "UI/UIObject.h"
#include "UI/UIButton.h"
#include "Scene/UIViewport.h"
#include "../UI/TestButton.h"
#include "../UI/TestBar.h"
#include "../UI/CheckBox.h"
#include "../UI/Slider.h"
#include "../UI/Inventory.h"
#include "../UI/TestSlot.h"
#include "../UI/TestIcon.h"
#include "../Object/TileMapObj.h"

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
	CreateMaterial();

	// 오브젝트 생성시 호출해줄 함수를 해당 장면에 등록한다.

	CPlayer* pPlayer = m_pScene->CreateObject<CPlayer>("Player");

	// GameMode에 플레이어를 등록한다.
	SetPlayer(pPlayer);

	SAFE_RELEASE(pPlayer);

	/*CMonster* pMonster = m_pScene->CreateObject<CMonster>("Monster");

	SAFE_RELEASE(pMonster);*/

	//CTestObj* pTestObj = m_pScene->CreateObject<CTestObj>("TestObj");

	//SAFE_RELEASE(pTestObj);

	//CTestButton* pTestButton = m_pScene->CreateUIObject<CTestButton>("TestButton");

	//SAFE_RELEASE(pTestButton);

	//CTestBar* pTestBar = m_pScene->CreateUIObject<CTestBar>("TestBar");

	//SAFE_RELEASE(pTestBar);

	CTileMapObj* pTileMapObj = m_pScene->CreateObject<CTileMapObj>("TileMapObj");

	SAFE_RELEASE(pTileMapObj);

	/*CCheckBox* pTestCheckBox = m_pScene->CreateUIObject<CCheckBox>("TestCheckBox");

	SAFE_RELEASE(pTestCheckBox);

	CSlider* pSlider = m_pScene->CreateUIObject<CSlider>("Slider");

	SAFE_RELEASE(pSlider);*/

	CInventory* pInventory = m_pScene->CreateUIObject<CInventory>("Inventory");

	pInventory->AddToViewport();

	((CPlayer*)m_pPlayer)->SetInventory(pInventory);

	SAFE_RELEASE(pInventory);

	CTestIcon* pTestIcon = m_pScene->CreateUIObject<CTestIcon>("TestIcon");

	CTestSlot* pTestSlot = m_pScene->CreateUIObject<CTestSlot>("TestSlot");

	pTestIcon->AddToViewport();
	pTestSlot->AddToViewport();

	pTestSlot->AddIcon(pTestIcon);
	
	CTestSlot* pTestSlot2 = m_pScene->CreateUIObject<CTestSlot>("TestSlot");

	pTestSlot2->AddToViewport();
	pTestSlot2->SetWorldPos(pTestSlot->GetWorldPos() + Vector3(80.f, 0.f, 0.f));

	SAFE_RELEASE(pTestSlot2);
	SAFE_RELEASE(pTestIcon);
	SAFE_RELEASE(pTestSlot);

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
	pManager->LoadTexture("WeaponSlot", TEXT("WeaponSlot.png"));
	pManager->LoadTexture("WeaponIcon", TEXT("WeaponIcon.png"));
	pManager->LoadTexture("InventoryBackImage", TEXT("InventoryBackImage.png"));
	pManager->LoadTexture("Inventorytitlebar", TEXT("Inventorytitlebar.png"));
	pManager->LoadTexture("InventorySlot", TEXT("InventorySlot.png"));
	pManager->LoadTexture("InventoryScrollBar", TEXT("InventoryScrollBar.png"));
	pManager->LoadTexture("InventoryScroll", TEXT("InventoryScroll.png"));
	pManager->LoadTexture("PixelTest", TEXT("PixelCollision.png"));
	pManager->LoadTexture("Heavy", TEXT("Heavy.png"));
	pManager->LoadTexture("Tile", TEXT("Floors.png"));

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

bool CTestGameMode::CreateMaterial()
{
	CSceneResource* pManager = m_pScene->GetResourceManager();

	//해당 모드에서 사용하는 Texture를 Setting해주기 위해서 GameMode에서 생성
	//-------------------------------------------------------------------------------

	//Pixel충돌용 Material
	//--------------------------------------------------------------------
	pManager->CreateMaterial("TestPixelMtrl");

	CMaterial* pMtrl = pManager->FindMaterial("TestPixelMtrl");

	pMtrl->SetShader("NormalShader");
	pMtrl->SetTexture(TEXTURE_LINK::DIFFUSE, "PixelTest",
		(int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL);

	SAFE_RELEASE(pMtrl);
	//--------------------------------------------------------------------

	//Tile Material
	//--------------------------------------------------------------------
	pManager->CreateMaterial("TileMtrl");

	pMtrl = pManager->FindMaterial("TileMtrl");

	pMtrl->SetShader("InstancingShader2D");
	pMtrl->SetTexture(TEXTURE_LINK::DIFFUSE, "Tile",
		(int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL);

	SAFE_RELEASE(pMtrl);
	//--------------------------------------------------------------------

	//Bullet Material
	//--------------------------------------------------------------------
	pManager->CreateMaterial("BulletMtrl");
	
	pMtrl = pManager->FindMaterial("BulletMtrl");

	pMtrl->SetShader("NormalShader");
	pMtrl->SetTexture(TEXTURE_LINK::DIFFUSE, "UIBar",
		(int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL);

	SAFE_RELEASE(pMtrl);
	//--------------------------------------------------------------------

	//-------------------------------------------------------------------------------

	return true;
}
