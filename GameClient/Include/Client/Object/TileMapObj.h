#pragma once

#include "GameObject.h"

class CTileMapObj : public CGameObject
{
public:
	CTileMapObj();
	CTileMapObj(const CTileMapObj& obj);
	virtual ~CTileMapObj();

protected:
	class CTileMap* m_pTileMap;


public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
};

