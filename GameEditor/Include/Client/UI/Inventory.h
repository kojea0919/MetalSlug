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

	class CUIImage* m_pSlotImage;

	class CUIImage* m_pBarBack;
	class CUIImage* m_pBar;

private:
	//슬롯 이미지가 출력 가능한 세로 크기
	float			m_fSlotRenderHeight;
	
	//슬롯 이미지의 세로 크기
	float			m_fSlotHeight;

	//현재 출력되는 Slot의 시작과 끝 (상대)좌표
	float			m_fStart;
	float			m_fEnd;

	float			m_fBarMoveLen;

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

