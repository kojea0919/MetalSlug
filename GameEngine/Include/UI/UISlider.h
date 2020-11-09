#pragma once

#include "UIButton.h"

class CUISlider : public CUIButton
{
	friend class CUIObject;

protected:
	CUISlider();
	CUISlider(const CUISlider& control);
	virtual ~CUISlider();

protected:
	
	float		m_fCurRate;

	//Bar Image
	class CUIImage * m_pBarImage;

	//Mouse Pos
	Vector2			m_vMousePos;

	//Min,Max
	int				m_iMinX;
	int				m_iMaxX;

public:
	void SetMinX(int iMinX)
	{
		m_iMinX = iMinX;
	}

	void SetMaxX(int iMaxX)
	{
		m_iMaxX = iMaxX;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CUISlider* Clone();
};

