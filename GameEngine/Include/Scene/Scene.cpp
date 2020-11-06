#include "Scene.h"
#include "../GameObject.h"
#include "SceneResource.h"
#include "CameraManager.h"
#include "SceneCollision.h"
#include "../PathManager.h"
#include "../Resource/ResourceManager.h"
#include "UIViewport.h"

CScene::CScene()
{
	m_pGameMode = new CGameMode;
	m_pResource = new CSceneResource;
	m_pCameraManager = new CCameraManager;
	m_pCollisionManager = new CSceneCollision;
	m_pUIViewport = new CUIViewport;

	memset(m_strAnimSequenceFileName, 0, sizeof(MAX_PATH));
}

CScene::~CScene()
{
	SAFE_DELETE(m_pUIViewport);
	SAFE_DELETE(m_pCollisionManager);
	SAFE_DELETE(m_pResource);
	SAFE_DELETE(m_pGameMode);
	SAFE_RELEASE_VECLIST(m_ObjList);
	SAFE_DELETE(m_pCameraManager);
}

void CScene::SetSequenceFileName(const char* pFileName)
{
	if (pFileName)
	{
		strcpy_s(m_strAnimSequenceFileName, pFileName);
		LoadAnimation2DSequence();
	}
}

void CScene::AddSequenceResource(const char* pFileName)
{
	if (pFileName)
	{
		LoadAniFile(pFileName);

		//Sequence List���� ���� Update
		//----------------------------------------
		//���� ��� Setting
		//----------------------------------------------
		char strFullPath[MAX_PATH] = {};
		const char* pPath = GET_SINGLE(CPathManager)->FindPathMultibyte(RESOURCE_PATH);

		if (pPath)
			strcpy_s(strFullPath, pPath);

		strcat_s(strFullPath, m_strAnimSequenceFileName);
		//-------------------------------------------

		m_vecAniFileName.push_back(pFileName);

		FILE* pFile = nullptr;
		fopen_s(&pFile, strFullPath, "wt");

		int iSize = (int)m_vecAniFileName.size();
		fprintf_s(pFile, "%d\n", iSize);

		for (int iCnt = 0; iCnt < iSize; ++iCnt)
		{
			fprintf_s(pFile, "%s\n", m_vecAniFileName[iCnt].c_str());
		}

		fclose(pFile);
		//----------------------------------------
	}
}

CGameObject* CScene::FindObject(const string& strName)
{
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == strName)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return nullptr;
}

bool CScene::Init()
{
	if (!m_pCollisionManager->Init())
		return false;

	if (!m_pUIViewport->Init())
		return false;

	return true;
}

void CScene::Start()
{
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}

	m_pUIViewport->Start();
}

void CScene::Update(float fTime)
{
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		if (!(*iter)->IsStart())
			(*iter)->Start();

		(*iter)->Update(fTime);
		++iter;
	}

	m_pUIViewport->Update(fTime);
}

void CScene::PostUpdate(float fTime)
{
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
 			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		if (!(*iter)->IsStart())
			(*iter)->Start();

		(*iter)->PostUpdate(fTime);
		++iter;
	}

	// �÷��̾ �߽����� Center�� Setting
	if(m_pGameMode && m_pGameMode->m_pPlayer)
		m_pCollisionManager->SetCenter(m_pGameMode->m_pPlayer->GetWorldPos());

	m_pUIViewport->PostUpdate(fTime);
}

void CScene::Collision(float fTime)
{
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		if (!(*iter)->IsStart())
			(*iter)->Start();

		(*iter)->Collision(fTime);
		++iter;
	}

	m_pCollisionManager->Collision(fTime);
}

void CScene::PrevRender(float fTime)
{
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		if (!(*iter)->IsStart())
			(*iter)->Start();

		(*iter)->PrevRender(fTime);
		++iter;
	}

	m_pUIViewport->PrevRender(fTime);
}

void CScene::Render(float fTime)
{
	m_pUIViewport->Render(fTime);
}

void CScene::PostRender(float fTime)
{
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		if (!(*iter)->IsStart())
			(*iter)->Start();

		(*iter)->PostRender(fTime);
		++iter;
	}

	m_pUIViewport->PostRender(fTime);
}

bool CScene::LoadAnimation2DSequence()
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
	strcat_s(strFullPath, m_strAnimSequenceFileName);
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

	SetAnimNotify();

	return true;
}

bool CScene::LoadAnimation2D(FILE * pFile)
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

		fscanf_s(pFile, "%s\n", strFileName, MAX_PATH);

		if (LoadAniFile(strFileName))
			m_vecAniFileName.push_back(strFileName);
	}
	//---------------------

	return true;
}

bool CScene::LoadAniFile(const char* pAniFileName)
{
	if (!pAniFileName)
		return false;

	//�ؽ�ó �ε�
	GET_SINGLE(CResourceManager)->LoadAnimation2DSequence(pAniFileName);

	return true;
}

void CScene::SetAnimNotify()
{
	m_pResource->AddAnimation2DNotify("PlayerAttack",
			"Attack", 2);

	m_pResource->AddAnimation2DNotify("PlayerRightLowerMoveStart", "MoveStartEnd", 3);
	m_pResource->AddAnimation2DNotify("PlayerLeftLowerMoveStart", "MoveStartEnd", 3);
	m_pResource->AddAnimation2DNotify("PlayerRightNormalMidShot", "ShotEnd", 3);
	m_pResource->AddAnimation2DNotify("PlayerRightNormalMidShotEnd", "GotoIdle", 7);
	m_pResource->AddAnimation2DNotify("PlayerLeftNormalMidShot", "ShotEnd", 3);
	m_pResource->AddAnimation2DNotify("PlayerLeftNormalMidShotEnd", "GotoIdle", 7);
	m_pResource->AddAnimation2DNotify("PlayerRightNormalUpShot", "ShotEnd", 3);
	m_pResource->AddAnimation2DNotify("PlayerRightNormalUpShotEnd", "GotoIdle", 7);
	m_pResource->AddAnimation2DNotify("PlayerLeftNormalUpShot", "ShotEnd", 3);
	m_pResource->AddAnimation2DNotify("PlayerLeftNormalUpShotEnd", "GotoIdle", 7);
	m_pResource->AddAnimation2DNotify("PlayerRightAimUpStart", "AimUp", 2);
	m_pResource->AddAnimation2DNotify("PlayerRightAimDown", "GotoIdle", 2);
	m_pResource->AddAnimation2DNotify("PlayerLeftAimUpStart", "AimUp", 2);
	m_pResource->AddAnimation2DNotify("PlayerLeftAimDown", "GotoIdle", 2);

}

void CScene::SaveScene(const char* pFileName, const string& strPathName)
{

}

void CScene::SaveSceneFullPath(const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "wb");

	if (!pFile)
		return;

	//Scene���� ����ϴ� Sequence List�� �����ϴ� �ؽ�Ʈ �����̸� ����
	int iLen = (int)strlen(m_strAnimSequenceFileName);
	fwrite(&iLen, sizeof(iLen), 1, pFile);
	fwrite(m_strAnimSequenceFileName, 1, iLen, pFile);


	//Scene���� ����ϴ� Resource ����
	m_pResource->Save(pFile);
	
	//Scene�� ������ Object ����
	//-------------------------------------
	int	iCount = (int)m_ObjList.size();
	fwrite(&iCount, 1, sizeof(int), pFile);

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		int	iObjType = (*iter)->GetObjType();
		fwrite(&iObjType, 4, 1, pFile);
		(*iter)->Save(pFile);
	}
	//-------------------------------------

	// GameMode�� Ÿ���� ����
	//-------------------------------------
	fwrite(&m_pGameMode->m_iGameModeType, sizeof(int), 1, pFile);
	m_pGameMode->Save(pFile);
	//-------------------------------------


	fclose(pFile);
}

void CScene::LoadScene(const char* pFileName, const string& strPathName)
{
}

void CScene::LoadSceneFullPath(const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "rb");

	if (!pFile)
		return;

	//Scene���� ����ϴ� Sequence List�� �����ϴ� �ؽ�Ʈ �����̸� Load
	char strFileName[MAX_PATH] = {};
	int iLen = 0;
	fread(&iLen, sizeof(iLen), 1, pFile);
	fread(strFileName, 1, iLen, pFile);
	SetSequenceFileName(strFileName);

	// Scene���� ����ϴ� Resource Load
	m_pResource->Load(pFile);

	//Scene�� ������ Object Load
	//---------------------------------------

	//Object�� Load
	//---------------------------
	int	iCount = 0;
	fread(&iCount, 1, sizeof(int), pFile);
	//---------------------------

	//�� Object Load
	//---------------------------
	for (int i = 0; i < iCount; ++i)
	{
		int	iObjType = 0;
		fread(&iObjType, 4, 1, pFile);
		CGameObject* pObj = m_CreateObjectFunction(iObjType);

		pObj->m_pScene = this;

		pObj->Load(pFile);

		if (m_EditorCreateFunc)
			m_EditorCreateFunc(pObj);

		m_ObjList.push_back(pObj);
	}
	//---------------------------

	//---------------------------------------


	int	iGameModeType = 0;
	fread(&iGameModeType, sizeof(int), 1, pFile);

	SAFE_DELETE(m_pGameMode);

	m_pGameMode = m_CreateGameModeFunction(iGameModeType);

	m_pGameMode->m_pScene = this;

	m_pGameMode->Load(pFile);

	fclose(pFile);
}
