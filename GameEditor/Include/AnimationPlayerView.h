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
	//--------------------

public:
	//��� ���� �ʱ�ȭ
	void Clear();

public:
	static void CALLBACK PlayAnimation(HWND hWnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime);

	void StopAnimation();

	//�ش� �ε����� �ش��ϴ� �̹����� ���
	void DrawImageAndAxis(int iIdx);

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
	//--------------------

	class CAnimation2DEditorDlg* m_pParentDlg;

	//���� ����ؾ��� �̹��� �ε���
	static int			m_iCurPrintImageIdx;

	bool				m_bPlay;

	static HWND			m_hTemp;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

