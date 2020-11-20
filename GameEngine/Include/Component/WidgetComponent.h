#pragma once

#include "PrimitiveComponent.h"

class CWidgetComponent : public CPrimitiveComponent
{
	friend class CGameObject;

protected:
	CWidgetComponent();
	CWidgetComponent(const CWidgetComponent& com);
	virtual ~CWidgetComponent();

protected:
	class CUIObject*	m_pUIObject;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CWidgetComponent* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

