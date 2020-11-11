#pragma once

#include "UI/UIObject.h"

class CTestSlot : public CUIObject
{
public:
	CTestSlot();
	CTestSlot(const CTestSlot& ui);
	~CTestSlot();

private:
	class CUISlot* m_pSlot;
	class CTestIcon* m_pIcon;

public:
	bool AddIcon(class CTestIcon* pIcon);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CTestSlot* Clone();
};

