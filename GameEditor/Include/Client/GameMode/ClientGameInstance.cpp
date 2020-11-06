#include "ClientGameInstance.h"

CClientGameInstance::CClientGameInstance()
{
}

CClientGameInstance::~CClientGameInstance()
{
}

bool CClientGameInstance::Init()
{
	if (!CGameInstance::Init())
		return false;

	return true;
}
