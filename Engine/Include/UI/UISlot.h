#pragma once

#include "UIControl.h"

class CUISlot : public CUIControl
{
	friend class CUIObject;

protected:
	CUISlot();
	CUISlot(const CUISlot& control);
	virtual ~CUISlot();

protected:
	//연결 지점
	Vector3			m_vConnectPos;

public:
	const Vector3 & GetConnectPos() const
	{
		return m_vConnectPos;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CUISlot* Clone();
};

