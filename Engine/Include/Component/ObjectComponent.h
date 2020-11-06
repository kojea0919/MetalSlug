#pragma once

#include "Component.h"

class CObjectComponent :
	public CComponent
{
protected:
	CObjectComponent();
	CObjectComponent(const CObjectComponent& com);
	virtual ~CObjectComponent();

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CObjectComponent* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

