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
	//GameMode에서 사용하는 Texture Load
	bool LoadTexture();

	//GameMOde에서 사용하는 애니메이션 정보 Load
	bool LoadAnimation2D();
};

