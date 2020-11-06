#pragma once

#include "UIControl.h"

//Bar가 차는 방향
//----------------
enum class BAR_DIR
{
	LeftRight,
	RightLeft,
	TopBottom,
	BottomTop
};
//----------------

class CUIProgressBar : public CUIControl
{
	friend class CUIObject;

protected:
	CUIProgressBar();
	CUIProgressBar(const CUIProgressBar& control);
	virtual ~CUIProgressBar();

protected:
	BAR_DIR			m_eDir;
	float			m_fPercent;
	BarCBuffer		m_tCBuffer;

public:
	void SetBarDir(BAR_DIR eDir)
	{
		m_eDir = eDir;
	}

	void SetPercent(float fPercent)
	{
		m_fPercent = fPercent;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CUIProgressBar* Clone();

};

