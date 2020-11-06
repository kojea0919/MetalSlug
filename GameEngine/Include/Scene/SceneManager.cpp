
#include "SceneManager.h"
#include "Scene.h"
#include "../PathManager.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager()	:
	m_pScene(nullptr),
	m_pSceneNext(nullptr),
	m_bChange(true)
{
}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_pSceneNext);
	SAFE_DELETE(m_pScene);
}

bool CSceneManager::Init()
{
	m_pScene = new CScene;

	m_pScene->Init();

	m_pScene->SetEditorCreateFunction<CSceneManager>(this, &CSceneManager::CreateObjectCallback);
	m_pScene->SetCreateObjectFunction<CSceneManager>(this, &CSceneManager::LoadObjectCallback);
	m_pScene->SetCreateGameModeFunction<CSceneManager>(this, &CSceneManager::LoadGameModeCallback);

	return true;
}

bool CSceneManager::Update(float fTime)
{
	if (m_bChange)
		m_bChange = false;

	m_pScene->Update(fTime);

	return ChangeScene();
}

bool CSceneManager::PostUpdate(float fTime)
{
	m_pScene->PostUpdate(fTime);

	return ChangeScene();
}

bool CSceneManager::Collision(float fTime)
{
	m_pScene->Collision(fTime);

	return ChangeScene();
}

void CSceneManager::PrevRender(float fTime)
{
	m_pScene->PrevRender(fTime);
}

void CSceneManager::Render(float fTime)
{
	m_pScene->Render(fTime);
}

void CSceneManager::PostRender(float fTime)
{
	m_pScene->PostRender(fTime);
}

bool CSceneManager::ChangeScene()
{
	if (m_pSceneNext)
	{
		SAFE_DELETE(m_pScene);
		m_pScene = m_pSceneNext;
		m_pSceneNext = nullptr;
		return true;
	}

	return false;
}

CScene* CSceneManager::CreateNextScene()
{
	SAFE_DELETE(m_pSceneNext);
	m_pSceneNext = new CScene;

	m_pSceneNext->Init();

	m_pSceneNext->SetEditorCreateFunction<CSceneManager>(this, &CSceneManager::CreateObjectCallback);
	m_pSceneNext->SetCreateObjectFunction<CSceneManager>(this, &CSceneManager::LoadObjectCallback);
	m_pSceneNext->SetCreateGameModeFunction<CSceneManager>(this, &CSceneManager::LoadGameModeCallback);

	return m_pSceneNext;
}

void CSceneManager::CreateObjectCallback(CGameObject* pObj)
{
	if (m_EditorCreateFunc)
		m_EditorCreateFunc(pObj);
}

CGameObject* CSceneManager::LoadObjectCallback(int iObjType)
{
	if (m_CreateObjectFunction)
		return m_CreateObjectFunction(iObjType);

	return nullptr;
}

CGameMode* CSceneManager::LoadGameModeCallback(int iGameModeType)
{
	if (m_CreateGameModeFunction)
		return m_CreateGameModeFunction(iGameModeType);

	return nullptr;
}

void CSceneManager::SaveScene(const char* pFileName, const string& strPathName)
{
	char	strFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindPathMultibyte(strPathName);

	if(pPath)
		strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFileName);

	SaveSceneFullPath(strFullPath);
}

void CSceneManager::SaveSceneFullPath(const char* pFullPath)
{
	m_pScene->SaveSceneFullPath(pFullPath);
}

void CSceneManager::LoadScene(const char* pFileName, const string& strPathName)
{
	char	strFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindPathMultibyte(strPathName);

	if (pPath)
		strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFileName);

	LoadSceneFullPath(strFullPath);
}

void CSceneManager::LoadSceneFullPath(const char* pFullPath)
{
	// 새로운 장면을 만들고 해당 장면을 불러온다.
	CreateNextScene();

	m_pSceneNext->LoadSceneFullPath(pFullPath);
}
