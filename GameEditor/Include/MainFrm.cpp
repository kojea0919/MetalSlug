
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "GameEditor.h"

#include "MainFrm.h"
#include "WorldObjectDlg.h"
#include "DetailObjDlg.h"
#include "GameEditorView.h"
#include "ViewManager.h"
#include "ToolBarDlg.h"
#include "Animation2DEditorDlg.h"
#include "PlayerEditDlg.h"
#include "SceneEditor.h"
#include "Animation2DBindDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_COMMAND(ID_ANIMATION2D_EDITOR, &CMainFrame::OnAnimation2dEditor)
	ON_COMMAND(ID_PLAYER_EDITOR, &CMainFrame::OnPlayerEditor)
	ON_COMMAND(ID_SCENE, &CMainFrame::OnScene)
	ON_COMMAND(ID_ANIM2DCOMBINEEDIT, &CMainFrame::OnAnim2dcombineedit)
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
	: m_pAnim2DEditor(nullptr), m_pPlayerEditor(nullptr),
	m_pSceneEditor(nullptr), m_pAnimBindEditor(nullptr)
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
	if (m_pAnim2DEditor)
	{
		m_pAnim2DEditor->DestroyWindow();
		SAFE_DELETE(m_pAnim2DEditor);
	}

	if (m_pPlayerEditor)
	{
		m_pPlayerEditor->DestroyWindow();
		SAFE_DELETE(m_pPlayerEditor);
	}

	if (m_pSceneEditor)
	{
		m_pSceneEditor->DestroyWindow();
		SAFE_DELETE(m_pSceneEditor);
	}

	if (m_pAnimBindEditor)
	{
		m_pAnimBindEditor->DestroyWindow();
		SAFE_DELETE(m_pAnimBindEditor);
	}
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	cs.cx = 1920;
	cs.cy = 1080;

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	m_SplitWnd.CreateStatic(this, 1, 2);

	m_SplitWndWorld.CreateStatic(&m_SplitWnd, 2, 1,
		WS_CHILD | WS_VISIBLE | WS_BORDER, m_SplitWnd.IdFromRowCol(0, 1));

	/*m_SplitWndToolbar.CreateStatic(&m_SplitWnd, 2, 1,
		WS_CHILD | WS_VISIBLE | WS_BORDER, m_SplitWnd.IdFromRowCol(0, 0));*/

	m_SplitWndWorld.CreateView(0, 0, RUNTIME_CLASS(CWorldObjectDlg),
		CSize(520, 400), pContext);
	m_SplitWndWorld.CreateView(1, 0, RUNTIME_CLASS(CDetailObjDlg),
		CSize(520, 680), pContext);

	/*m_SplitWndToolbar.CreateView(0, 0, RUNTIME_CLASS(CToolBarDlg),
		CSize(1400, 200), pContext);
	m_SplitWndToolbar.CreateView(1, 0, RUNTIME_CLASS(CGameEditorView),
		CSize(1400, 880), pContext);*/

	m_SplitWnd.CreateView(0, 0, RUNTIME_CLASS(CGameEditorView),
		CSize(1400, 1080), pContext);

	SetActiveView((CView*)m_SplitWnd.GetPane(0, 0));

	GET_SINGLE(CViewManager)->Init((CWorldObjectDlg*)m_SplitWndWorld.GetPane(0, 0),
		(CDetailObjDlg*)m_SplitWndWorld.GetPane(1, 0));

	return TRUE;
}


void CMainFrame::OnAnimation2dEditor()
{
	//다이얼로그 창을 보여준 후에도 Main은 돌아갈 수 있도록
	//모달리스 방식의 다이얼로그를 생성한다.
	if (!m_pAnim2DEditor)
	{
		m_pAnim2DEditor = new CAnimation2DEditorDlg;

		m_pAnim2DEditor->Create(IDD_DIALOG_ANIM2DEDITOR);
	}

	m_pAnim2DEditor->ShowWindow(SW_SHOW);

}


void CMainFrame::OnPlayerEditor()
{
	if (!m_pPlayerEditor)
	{
		m_pPlayerEditor = new CPlayerEditDlg;

		m_pPlayerEditor->Create(IDD_DIALOG_PLAYER_EDITOR);
	}

	m_pPlayerEditor->ShowWindow(SW_SHOW);
}


void CMainFrame::OnScene()
{
	if (!m_pSceneEditor)
	{
		m_pSceneEditor = new CSceneEditor;

		m_pSceneEditor->Create(IDD_DIALOG_SCENEEDITOR);
	}

	m_pSceneEditor->UpdateSceneEditorSequenceFileName();
	m_pSceneEditor->ShowWindow(SW_SHOW);
}


void CMainFrame::OnAnim2dcombineedit()
{
	if (!m_pAnimBindEditor)
	{
		m_pAnimBindEditor = new CAnimation2DBindDlg;

		m_pAnimBindEditor->Create(IDD_DIALOG_ANIMCOMBINEEDIT);
	}

	m_pAnimBindEditor->ShowWindow(SW_SHOW);
}
