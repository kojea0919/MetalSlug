#pragma once

#include <afxwin.h>

class CAnimationPlayerView : public CView
{
	friend class CAnimation2DEditorDlg;
	friend class CAnimation2DBindDlg;

protected:
	CAnimationPlayerView();
	~CAnimationPlayerView();
	DECLARE_DYNCREATE(CAnimationPlayerView)

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
	//--------------------

public:
	//모든 상태 초기화
	void Clear();

public:
	static void CALLBACK PlayAnimation(HWND hWnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime);

	void StopAnimation();

	//해당 인덱스에 해당하는 이미지를 출력
	void DrawImageAndAxis(int iIdx);

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
	//--------------------

	class CAnimation2DEditorDlg* m_pParentDlg;

	//현재 출력해야할 이미지 인덱스
	static int			m_iCurPrintImageIdx;

	bool				m_bPlay;

	static HWND			m_hTemp;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

