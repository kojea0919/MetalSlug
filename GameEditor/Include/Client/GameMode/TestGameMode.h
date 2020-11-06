#pragma once

#include "Scene/GameMode.h"

class CTestGameMode : public CGameMode
{
public:
	CTestGameMode();
	~CTestGameMode();

public:
	virtual bool Init();

private:
	//GameMode���� ����ϴ� Texture Load
	bool LoadTexture();

	//GameMOde���� ����ϴ� �ִϸ��̼� ���� Load
	bool LoadAnimation2D();
};

