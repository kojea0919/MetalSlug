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

	vector<class CUISlot*> m_vecSlot;

	class CUIImage* m_pBarBack;
	class CUIImage* m_pBar;

private:
	//슬롯 개수
	//---------------------------------
	int					m_iSlotWidthCnt;
	int					m_iSlotHeightCnt;

	//출력되는 슬롯 세로 개수
	int					m_iSlotRenderHeightCnt;

	//---------------------------------


	//Scroll이 움직일수 있는 길이
	int					m_iCanMoveScrollLen;

	//Scroll이 한번에 움직여야 하는 크기
	int 				m_iScrollMoveUnit;

	//Scroll 현재 출력시작 Idx
	int					m_iRenderStartIdx;
	const int			m_iRenderMaxIdx;

	//Grid가 한번에 움직여야 하는 크기
	int					m_iGridMoveUnit;

public:
	void ScrollDown();
	void ScrollUp();

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

