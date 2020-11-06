#pragma once

#include "UI/UIObject.h"

class CTestBar : public CUIObject
{
public:
	CTestBar();
	CTestBar(const CTestBar& ui);
	~CTestBar();

private:
	class CUIImage* m_pBack;
	class CUIProgressBar* m_pBar;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CTestBar* Clone();
};

