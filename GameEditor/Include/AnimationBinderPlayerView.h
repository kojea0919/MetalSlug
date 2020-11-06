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
	//Set함수
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
	//Get함수
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
	//모든 상태 초기화
	void Clear();

public:
	static void CALLBACK PlayAnimation(HWND hWnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime);

	void StopAnimation();

	//해당하는 이미지를 출력
	void DrawImageAndAxis(const CImage & tParentImage,const RECT & tParentCutInfo,const Vector2 & tParentPivotInfo,
		const CImage & tChildImage,const RECT & tChildCutInfo,const Vector2 & tChildPivotInfo,
		const Vector2 & tChildRelativePos);

private:
	//x,y축 그리기
	//--------------------
	void DrawAxis(HDC hDC);
	//--------------------

private:
	void SetPlayAnimationTimer();

private:
	//창 크기
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

	//현재 출력해야할 이미지 인덱스
	static int			m_iCurPrintImageIdx;

	bool				m_bPlay;

	static HWND			m_hTemp;

	//확대 축소
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

