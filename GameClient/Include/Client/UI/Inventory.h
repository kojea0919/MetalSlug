#pragma once

#include "UI/UIObject.h"

class CInventory : public CUIObject
{
public:
	CInventory();
	CInventory(const CInventory& ui);
	~CInventory();

private:
	class CUIImage* m_pBack;
	class CUITitleBar* m_pTitleBar;
	class CUIButton* m_pExitButton;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CInventory* Clone();

private:
	void ExitButtonCallback();
};

