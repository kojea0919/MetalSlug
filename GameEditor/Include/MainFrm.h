
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CSplitterWnd	m_SplitWnd;
	CSplitterWnd	m_SplitWndWorld;
	CSplitterWnd	m_SplitWndToolbar;

	//애니메이션 2D Editor Dialog
	class CAnimation2DEditorDlg* m_pAnim2DEditor;

	//Player Editor Dialog
	class CPlayerEditDlg* m_pPlayerEditor;

	//Scene Editor
	class CSceneEditor * m_pSceneEditor;

	//Anim Combine Editor
	class CAnimation2DBindDlg* m_pAnimBindEditor;

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnAnimation2dEditor();
	afx_msg void OnPlayerEditor();
	afx_msg void OnScene();
	afx_msg void OnAnim2dcombineedit();
};


