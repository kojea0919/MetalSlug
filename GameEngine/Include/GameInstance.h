#pragma once

#include "GameEngine.h"

class CGameInstance
{
	friend class CEngine;

protected:
	CGameInstance();
	virtual ~CGameInstance();

public:
	virtual bool Init();
};

