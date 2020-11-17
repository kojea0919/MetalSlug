#pragma once

#include "UI/UIObject.h"

class CSlider : public CUIObject
{
public:
	CSlider();
	CSlider(const CSlider& ui);
	~CSlider();

private:
	class CUISlider*	m_pSlider;
	class CUIImage*		m_pBar;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CSlider* Clone(); 


private:
	void ButtonCallback();
};

