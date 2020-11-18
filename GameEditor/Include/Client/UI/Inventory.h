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
	//���� ����
	//---------------------------------
	int					m_iSlotWidthCnt;
	int					m_iSlotHeightCnt;

	//��µǴ� ���� ���� ����
	int					m_iSlotRenderHeightCnt;

	//---------------------------------


	//Scroll�� �����ϼ� �ִ� ����
	int					m_iCanMoveScrollLen;

	//Scroll�� �ѹ��� �������� �ϴ� ũ��
	int 				m_iScrollMoveUnit;

	//Scroll ���� ��½��� Idx
	int					m_iRenderStartIdx;
	const int			m_iRenderMaxIdx;

	//Grid�� �ѹ��� �������� �ϴ� ũ��
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

