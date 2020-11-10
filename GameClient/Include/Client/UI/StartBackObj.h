#pragma once

#include "UI/UIObject.h"

class CStartBackObj : public CUIObject
{
public:
	CStartBackObj();
	CStartBackObj(const CStartBackObj& ui);
	~CStartBackObj();

private:
	class CUIImage*		m_pBack;
	class CUIButton*	m_pStartButton;
	class CUIButton*	m_pExitButton;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CStartBackObj* Clone();

private:
	void StartButtonCallback();
	void ExitButtonCallback();
};

