
#include "ClientCreateSystem.h"
#include "Scene/SceneManager.h"
#include "Object/Player.h"
#include "Object/Bullet.h"
#include "GameMode/MainGameMode.h"

DEFINITION_SINGLE(CClientCreateSystem)

CClientCreateSystem::CClientCreateSystem()
{
}

CClientCreateSystem::~CClientCreateSystem()
{
}

bool CClientCreateSystem::Init()
{
	GET_SINGLE(CSceneManager)->SetCreateObjectFunction<CClientCreateSystem>(this,
		&CClientCreateSystem::CreateObject);
	GET_SINGLE(CSceneManager)->SetCreateGameModeFunction<CClientCreateSystem>(this,
		&CClientCreateSystem::CreateGameMode);

	return true;
}

CGameObject* CClientCreateSystem::CreateObject(int iObjType)
{
	CGameObject* pObj = nullptr;

	switch (iObjType)
	{
	case OT_PLAYER:
		pObj = new CPlayer;
		break;
	case OT_BULLET:
		pObj = new CBullet;
		break;
	}

	return pObj;
}

CGameMode* CClientCreateSystem::CreateGameMode(int iGameModeType)
{
	CGameMode* pGameMode = nullptr;

	switch (iGameModeType)
	{
	case GM_MAIN:
		pGameMode = new CMainGameMode;
		break;
	}

	return pGameMode;
}
