#pragma once

#include "GameEngine.h"

enum OBJECT_TYPE
{
	OT_PLAYER,
	OT_BULLET
};

enum GAMEMODE_TYPE
{
	GM_MAIN
};

class CClientCreateSystem
{
public:
	bool Init();
	class CGameObject* CreateObject(int iObjType);
	class CGameMode* CreateGameMode(int iGameModeType);

	DECLARE_SINGLE(CClientCreateSystem)
};

