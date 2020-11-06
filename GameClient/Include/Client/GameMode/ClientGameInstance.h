#pragma once

#include "GameInstance.h"

class CClientGameInstance :	public CGameInstance
{
public:
	CClientGameInstance();
	virtual ~CClientGameInstance();

public:
	virtual bool Init();
};

