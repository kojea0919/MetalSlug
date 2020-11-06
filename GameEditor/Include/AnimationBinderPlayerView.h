#pragma once

#include <afxwin.h>

class CAnimationBinderPlayerView : public CView
{
	friend class CAnimation2DBindDlg;

protected:
	CAnimationBinderPlayerView();
	~CAnimationBinderPlayerView();
	DECLARE_DYNCREATE(CAnimationBinderPlayerView)

public:
	virtual void OnDraw(CDC* /*pDC*/);

public:
	//Set�Լ�
	//--------------------
	void SetViewWidth(int iWidth)
	{
		m_iViewWidth = iWidth;
	}
	void SetViewHeight(int iHeight)
	{
		m_iViewHeight = iHeight;
	}
	void IncreaseScale()
	{
		++m_iCurScale;
		if (m_iCurScale > m_iMaxScale)
			m_iCurScale = m_iMaxScale;
	}
	void DecreaseScale()
	{
		--m_iCurScale;
		if (m_iCurScale < m_iMinScale)
			m_iCurScale = m_iMinScale;
	}
	//--------------------

public:
	//Get�Լ�
	//--------------------
	int	GetViewWidth()const
	{
		return m_iViewWidth;
	}
	int	GetViewHeight()const
	{
		return m_iViewHeight;
	}
	int GetScale() const
	{
		return m_iCurScale;
	}
	//--------------------

public:
	//��� ���� �ʱ�ȭ
	void Clear();

public:
	static void CALLBACK PlayAnimation(HWND hWnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime);

	void StopAnimation();

	//�ش��ϴ� �̹����� ���
	void DrawImageAndAxis(const CImage & tParentImage,const RECT & tParentCutInfo,const Vector2 & tParentPivotInfo,
		const CImage & tChildImage,const RECT & tChildCutInfo,const Vector2 & tChildPivotInfo,
		const Vector2 & tChildRelativePos);

private:
	//x,y�� �׸���
	//--------------------
	void DrawAxis(HDC hDC);
	//--------------------

private:
	void SetPlayAnimationTimer();

private:
	//â ũ��
	//--------------------
	int			m_iViewWidth;
	int			m_iViewHeight;
	//--------------------

	//DC
	//--------------------
	HDC			m_hDC;
	HDC				m_hBackDC;
	//--------------------

	HBITMAP m_hBackBit;
	HBITMAP m_hOldBit;

	class CAnimation2DBindDlg* m_pParentDlg;

	//���� ����ؾ��� �̹��� �ε���
	static int			m_iCurPrintImageIdx;

	bool				m_bPlay;

	static HWND			m_hTemp;

	//Ȯ�� ���
	//----------------------------
	int					m_iCurScale;
	int					m_iMaxScale;
	int					m_iMinScale;
	//----------------------------

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

