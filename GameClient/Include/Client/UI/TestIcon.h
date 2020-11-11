#pragma once

#include "UI/UIObject.h"

class CTestIcon : public CUIObject
{
	friend class CTestSlot;
public:
	CTestIcon();
	CTestIcon(const CTestIcon& ui);
	~CTestIcon();

private:
	class CUIIcon* m_pIcon;

public:
	bool IsUp() const;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CTestIcon* Clone();
};

