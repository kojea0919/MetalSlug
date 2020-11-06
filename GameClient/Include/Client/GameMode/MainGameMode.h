#pragma once

#include "Scene/GameMode.h"

class CMainGameMode	:
	public CGameMode
{
public:
	CMainGameMode();
	~CMainGameMode();

public:
	//현재 Stage
	int	m_iCurrentStage;

public:
	virtual bool Init();

private:
	//현재 GameMode에서 사용하는 Animation Load
	virtual bool LoadAnimation2DSequence();

	bool LoadAnimation2D(FILE* pFile);

	bool LoadAniFile(const char* pAniFileName);

private:
	//GameMode에서 사용하는 Texture Load
	bool LoadTexture();

	//GameMOde에서 사용하는 애니메이션 정보 Load
	bool LoadAnimation2D();
};

