#pragma once

#include "UI/UIObject.h"

class CTextUI : public CUIObject
{
public:
	CTextUI();
	CTextUI(const CTextUI& ui);
	~CTextUI();

private:
	class CUIText* m_pText;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CTextUI* Clone();
};

