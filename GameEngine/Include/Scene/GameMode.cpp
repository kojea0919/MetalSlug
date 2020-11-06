#include "GameMode.h"
#include "../GameObject.h"
#include "Scene.h"
#include "../PathManager.h"

CGameMode::CGameMode()	:
	m_pPlayer(nullptr),
	m_pScene(nullptr)
{
}

CGameMode::~CGameMode()
{
	SAFE_RELEASE(m_pPlayer);
}

CGameObject* CGameMode::GetPlayer() const
{
	if (m_pPlayer)
		m_pPlayer->AddRef();
	return m_pPlayer;
}

void CGameMode::SetPlayer(CGameObject* pObj)
{
	SAFE_RELEASE(m_pPlayer);
	m_pPlayer = pObj;

	if (m_pPlayer)
	{
		m_pPlayer->AddRef();
	}
}

bool CGameMode::Init()
{
	return true;
}

void CGameMode::Start()
{
}

void CGameMode::Update(float fTime)
{
}

void CGameMode::PostUpdate(float fTime)
{
}

void CGameMode::Collision(float fTime)
{
}

void CGameMode::PrevRender(float fTime)
{
}

void CGameMode::Render(float fTime)
{
}

void CGameMode::PostRender(float fTime)
{
}

void CGameMode::Save(FILE* pFile)
{
	bool	bPlayer = false;

	if (m_pPlayer)
		bPlayer = true;

	fwrite(&bPlayer, sizeof(bool), 1, pFile);

	if (m_pPlayer)
	{
		int	iLength = (int)m_pPlayer->GetName().length();
		fwrite(&iLength, sizeof(int), 1, pFile);
		fwrite(m_pPlayer->GetName().c_str(), 1, iLength, pFile);
	}
}

void CGameMode::Load(FILE* pFile)
{
	bool	bPlayer = false;

	fread(&bPlayer, sizeof(bool), 1, pFile);

	if (bPlayer)
	{
		int	iLength = 0;
		char	strName[256] = {};
		fread(&iLength, sizeof(int), 1, pFile);
		fread(strName, 1, iLength, pFile);

		m_pPlayer = m_pScene->FindObject(strName);
	}
}

bool CGameMode::LoadAnimation2DSequence()
{
	return true;
}
