#pragma once

#include "UIButton.h"

class CUICheckBox :	public CUIButton
{
	friend class CUIObject;

protected:
	CUICheckBox();
	CUICheckBox(const CUICheckBox& control);
	virtual ~CUICheckBox();


protected:
	bool		m_bIsCheck;

	//Check¿ÃπÃ¡ˆ
	class CTexture* m_pCheckTexture;

public:
	void SetCheck(bool bCheck)
	{
		m_bIsCheck = bCheck;
	}

	void SetCheckTexture(const string& strName);
public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CUICheckBox* Clone();
};

