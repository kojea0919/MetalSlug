#pragma once

#include "UIButton.h"

class CUICheckBox :	public CUIButton
{
	friend class CUIObject;

protected:
	CUICheckBox();
	CUICheckBox(const CUICheckBox& control);
	virtual ~CUICheckBox();


protected:
	bool		m_bIsCheck;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CUICheckBox* Clone();

public:
	virtual void CollisionMouse(const Vector2& vMousePos, float fTime);
	virtual void CollisionReleaseMouse(const Vector2& vMousePos, float fTime);
};

