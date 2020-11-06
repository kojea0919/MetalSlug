#pragma once

#include "UI/UIObject.h"

class CTestButton :	public CUIObject
{
public:
	CTestButton();
	CTestButton(const CTestButton& ui);
	~CTestButton();

private:
	class CUIButton* m_pButton;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CTestButton* Clone();


private:
	void ButtonCallback();
};

