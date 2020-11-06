#pragma once

#include "UI/UIObject.h"

class CCheckBox : public CUIObject
{
public:
	CCheckBox();
	CCheckBox(const CCheckBox& ui);
	~CCheckBox();

private:
	class CUICheckBox* m_pButton;

	bool			m_bIsCheck;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CCheckBox* Clone();


private:
	void ButtonCallback();
};

