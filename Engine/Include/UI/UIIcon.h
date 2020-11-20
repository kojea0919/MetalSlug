#pragma once

#include "UIControl.h"

class CUIIcon : public CUIControl
{
	friend class CUIObject;

protected:
	CUIIcon();
	CUIIcon(const CUIIcon& control);
	virtual ~CUIIcon();

protected:
	bool		m_bMouseCollision;
	Vector3		m_vPrevPos;
	bool		m_bDown;
	bool		m_bPush;
	bool		m_bUp;
	bool		m_bDrag;

	bool		m_bAttachMouse;

	int			m_iPrevOwnerZOrder;
	int			m_iPrevZOrder;

public:
	bool IsUp() const
	{
		return m_bUp;
	}
	
	bool IsAttachMouse() const
	{
		return m_bAttachMouse;
	}

	void DetatchMouse()
	{
		m_bAttachMouse = false;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CUIIcon* Clone();
	virtual void CollisionMouse(const Vector2& vMousePos, float fTime);
	virtual void CollisionReleaseMouse(const Vector2& vMousePos, float fTime);
};

