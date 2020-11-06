#pragma once

#include "Scene/GameMode.h"

class CMainGameMode	:
	public CGameMode
{
public:
	CMainGameMode();
	~CMainGameMode();

public:
	//���� Stage
	int	m_iCurrentStage;

public:
	virtual bool Init();

private:
	//���� GameMode���� ����ϴ� Animation Load
	virtual bool LoadAnimation2DSequence();

	bool LoadAnimation2D(FILE* pFile);

	bool LoadAniFile(const char* pAniFileName);

private:
	//GameMode���� ����ϴ� Texture Load
	bool LoadTexture();

	//GameMOde���� ����ϴ� �ִϸ��̼� ���� Load
	bool LoadAnimation2D();
};

