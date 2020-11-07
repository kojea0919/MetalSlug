#pragma once

#include "UIControl.h"

class CUIImage : public CUIControl
{
	friend class CUIObject;
	
protected:
	CUIImage();
	CUIImage(const CUIImage& control);
	virtual ~CUIImage();

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CUIImage* Clone();
};

