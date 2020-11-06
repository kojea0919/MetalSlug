#pragma once

#include "../Engine.h"

class CGameMode
{
	friend class CScene;

protected:
	CGameMode();
	virtual ~CGameMode();

protected:
	class CScene* m_pScene;
	class CGameObject* m_pPlayer;
	int		m_iGameModeType;

	GameMode_Type m_eType;

public:
	class CGameObject* GetPlayer()	const;
	void SetPlayer(class CGameObject* pObj);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

protected:
	//현재 GameMode에서 사용하는 Animation Load
	//---------------------------------------
	virtual bool LoadAnimation2DSequence();
	//---------------------------------------
};

