#pragma once

#include "Scene/GameMode.h"

class CStartGameMode : public CGameMode
{
public:
	CStartGameMode();
	~CStartGameMode();

public:
	virtual bool Init();

private:
	bool LoadTexture();
	bool LoadAnimation2D();
	bool LoadSound();
};

