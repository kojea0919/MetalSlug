#include "pch.h"
#include "AnimationBinderPlayerView.h"
#include "Animation2DBindDlg.h"

int CAnimationBinderPlayerView::m_iCurPrintImageIdx;

HWND CAnimationBinderPlayerView::m_hTemp;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CAnimationBinderPlayerView, CView)

BEGIN_MESSAGE_MAP(CAnimationBinderPlayerView, CView)
	ON_WM_MOUSEACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

CAnimationBinderPlayerView::CAnimationBinderPlayerView()
	: m_bPlay(false), m_pParentDlg(nullptr), m_iCurScale(1),
	m_iMaxScale(3), m_iMinScale(1)
{
}

CAnimationBinderPlayerView::~CAnimationBinderPlayerView()
{
	SelectObject(m_hBackDC, m_hOldBit);
	DeleteObject(m_hBackBit);
	DeleteDC(m_hBackDC);

	::ReleaseDC(m_hWnd, m_hDC);
}

void CAnimationBinderPlayerView::OnDraw(CDC*)
{
	////Play버튼이 눌린 경우 Draw
	//if (m_bPlay)
	//{
	//	if (m_iCurPrintImageIdx >= m_pParentDlg->GetCutImageCnt())
	//		m_iCurPrintImageIdx = 0;

	//	HBITMAP hBackBit = NULL;
	//	HBITMAP hOldBit = NULL;
	//	HDC hBackDC = CreateCompatibleDC(m_hDC);

	//	hBackBit = CreateCompatibleBitmap(m_hDC, m_iViewWidth, m_iViewHeight);
	//	hOldBit = (HBITMAP)SelectObject(hBackDC, hBackBit);

	//	Rectangle(hBackDC, 0, 0, m_iViewWidth, m_iViewHeight);

	//	//현재 출력해야할 이미지의 Cut정보와 Pivot정보 Setting
	//	//--------------------------------------------------------------------------------
	//	RECT tCurRect = m_pParentDlg->m_pAnimEditView->GetCutInfo(m_iCurPrintImageIdx);
	//	Vector2 tCurPivot = m_pParentDlg->m_pAnimEditView->GetPivotInfo(m_iCurPrintImageIdx);
	//	//--------------------------------------------------------------------------------

	//	const CImage& tCurImage = m_pParentDlg->m_pAnimEditView->GetImage();

	//	//Pivot 적용
	//	//-------------------------------
	//	int iStartX = m_iViewWidth / 2;
	//	int iStartY = m_iViewHeight / 2;

	//	int iCutSizeX = tCurRect.right - tCurRect.left;
	//	int iCutSizeY = tCurRect.bottom - tCurRect.top;

	//	iStartX = (int)(iStartX - iCutSizeX * tCurPivot.x);
	//	iStartY = (int)(iStartY - iCutSizeY * tCurPivot.y);
	//	//-------------------------------

	//	//해당 영역만큼 이미지 출력
	//	tCurImage.Draw(hBackDC, iStartX, iStartY, iCutSizeX, iCutSizeY, tCurRect.left, tCurRect.top, iCutSizeX, iCutSizeY);

	//	DrawAxis(hBackDC);

	//	BitBlt(m_hDC, 0, 0, m_iViewWidth, m_iViewHeight, hBackDC, 0, 0, SRCCOPY);

	//	SelectObject(hBackDC, hOldBit);
	//	DeleteObject(hBackBit);
	//	DeleteDC(hBackDC);
	//}
}

void CAnimationBinderPlayerView::Clear()
{
	//Timer Stop
	if (m_bPlay)
		KillTimer(1);

	m_iCurPrintImageIdx = -1;
	m_bPlay = false;


}

void CAnimationBinderPlayerView::PlayAnimation(HWND hWnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime)
{
	++m_iCurPrintImageIdx;

	::InvalidateRect(m_hTemp, nullptr, FALSE);
}

void CAnimationBinderPlayerView::StopAnimation()
{
	KillTimer(1);

	m_bPlay = false;
}

void CAnimationBinderPlayerView::DrawImageAndAxis(const CImage& tParentImage, const RECT& tParentCutInfo, const Vector2& tParentPivotInfo,
	const CImage& tChildImage, const RECT& tChildCutInfo, const Vector2& tChildPivotInfo, const Vector2& tChildRelativePos)
{
	Rectangle(m_hBackDC, 0, 0, m_iViewWidth, m_iViewHeight);

	if (!tParentImage.IsNull())
	{
		//Pivot 적용
		//-------------------------------
		int iStartX = m_iViewWidth / 2;
		int iStartY = m_iViewHeight / 2;

		int iCutSizeX = (tParentCutInfo.right - tParentCutInfo.left) * m_iCurScale;
		int iCutSizeY = (tParentCutInfo.bottom - tParentCutInfo.top) * m_iCurScale;

		iStartX = (int)(iStartX - iCutSizeX * tParentPivotInfo.x);
		iStartY = (int)(iStartY - iCutSizeY * tParentPivotInfo.y);
		//-------------------------------

		//해당 영역만큼 이미지 출력
		tParentImage.Draw(m_hBackDC, iStartX, iStartY, iCutSizeX, iCutSizeY, tParentCutInfo.left, tParentCutInfo.top, iCutSizeX / m_iCurScale, iCutSizeY / m_iCurScale);
	}

	if (!tChildImage.IsNull())
	{
		//Pivot 적용
		//-------------------------------
		int iStartX = m_iViewWidth / 2;
		int iStartY = m_iViewHeight / 2;

		int iCutSizeX = (tChildCutInfo.right - tChildCutInfo.left) * m_iCurScale;
		int iCutSizeY = (tChildCutInfo.bottom - tChildCutInfo.top) * m_iCurScale;

		iStartX = (int)((iStartX - iCutSizeX * tChildPivotInfo.x) + tChildRelativePos.x);
		iStartY = (int)((iStartY - iCutSizeY * tChildPivotInfo.y) + tChildRelativePos.y);
		//-------------------------------

		//해당 영역만큼 이미지 출력
		tChildImage.Draw(m_hBackDC, iStartX, iStartY, iCutSizeX, iCutSizeY, tChildCutInfo.left, tChildCutInfo.top, iCutSizeX / m_iCurScale, iCutSizeY / m_iCurScale);
	}

	DrawAxis(m_hBackDC);

	BitBlt(m_hDC, 0, 0, m_iViewWidth, m_iViewHeight, m_hBackDC, 0, 0, SRCCOPY);
}

void CAnimationBinderPlayerView::DrawAxis(HDC hDC)
{
	HBRUSH OldBrush, NullBrush;

	HPEN Pen = CreatePen(0, 1, RGB(255, 0, 255));

	HPEN OldPen = (HPEN)SelectObject(hDC, Pen);

	NullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	OldBrush = (HBRUSH)SelectObject(hDC, NullBrush);

	//x축
	//---------------------------------------------
	int iViewHeightHalf = m_iViewHeight / 2;
	MoveToEx(hDC, 0, iViewHeightHalf, NULL);
	LineTo(hDC, m_iViewWidth, iViewHeightHalf);
	//---------------------------------------------

	//y축
	//---------------------------------------------
	int iViewWidthHalf = m_iViewWidth / 2;
	MoveToEx(hDC, iViewWidthHalf, 0, NULL);
	LineTo(hDC, iViewWidthHalf, m_iViewHeight);
	//---------------------------------------------

	SelectObject(hDC, OldBrush);
	SelectObject(hDC, OldPen);
	DeleteObject(NullBrush);
}

void CAnimationBinderPlayerView::SetPlayAnimationTimer()
{
	////이미지수 
	//int iImageCnt = m_pParentDlg->GetCutImageCnt();

	////현재 애니메이션의 속도
	//float fAnimPlayRate = m_pParentDlg->m_fPlayRate;

	////현재 애니메이션 재생 시간
	//float fAnimPlayTime = m_pParentDlg->m_fPlayTime;

	////주기 Setting
	//float fTimer = fAnimPlayTime / iImageCnt / fAnimPlayRate;

	//m_iCurPrintImageIdx = -1;

	//m_bPlay = true;

	//::SetTimer(m_pParentDlg->m_hWnd, 1, (int)(fTimer * 1000.f), CAnimationBinderPlayerView::PlayAnimation);
}

int CAnimationBinderPlayerView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return MA_ACTIVATE;
}


void CAnimationBinderPlayerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	//DlgSetting
	//m_pParentDlg = (CAnimation2DEditorDlg*)GetParent();

	m_hDC = ::GetDC(m_hWnd);

	m_hTemp = m_hWnd;

	m_hBackDC = CreateCompatibleDC(m_hDC);

	//BackBuffer의 픽셀 정보를 저장하기 위한 비트맵 생성
	m_hBackBit = CreateCompatibleBitmap(m_hDC, m_iViewWidth, m_iViewHeight);

	//생성한 비트맵을 BackDC에 Set
	m_hOldBit = (HBITMAP)SelectObject(m_hBackDC, m_hBackBit);
}

BOOL CAnimationBinderPlayerView::PreTranslateMessage(MSG* pMsg)
{
	////방향키 입력시 
	////-----------------------------
	//int iDiffX = 0;
	//int iDiffY = 0;
	//if (pMsg->message == WM_KEYDOWN)
	//{
	//	//이미지가 없으면 pass
	//	if (m_pParentDlg->GetCutImageCnt() == 0)
	//		return CView::PreTranslateMessage(pMsg);

	//	switch (pMsg->wParam)
	//	{
	//	case VK_LEFT:
	//		--iDiffX;
	//		break;
	//	case VK_RIGHT:
	//		++iDiffX;
	//		break;
	//	case VK_UP:
	//		--iDiffY;
	//		break;
	//	case VK_DOWN:
	//		++iDiffY;
	//		break;
	//	}

	//	RECT tCurRect = m_pParentDlg->m_pAnimEditView->GetCutInfo(m_iCurPrintImageIdx);
	//	Vector2 tCurPivot = m_pParentDlg->m_pAnimEditView->GetPivotInfo(m_iCurPrintImageIdx);

	//	//이미지 중심 좌표
	//	//------------------------------------------------------
	//	int iCutWidthHalfSize = (tCurRect.right - tCurRect.left) / 2;
	//	int iCutHeightHalfSize = (tCurRect.bottom - tCurRect.top) / 2;
	//	//------------------------------------------------------

	//	//현재 피벗 좌표
	//	//------------------------------------------------------
	//	int iStartX = m_iViewWidth / 2;
	//	int iStartY = m_iViewHeight / 2;

	//	int iCutSizeX = tCurRect.right - tCurRect.left;
	//	int iCutSizeY = tCurRect.bottom - tCurRect.top;

	//	int iPivotX = (int)(iStartX - iCutSizeX * tCurPivot.x);
	//	int iPivotY = (int)(iStartY - iCutSizeY * tCurPivot.y);
	//	//------------------------------------------------------

	//	//입력 적용
	//	//------------------------------------------------------
	//	iPivotX += iDiffX;
	//	iPivotY += iDiffY;
	//	//------------------------------------------------------

	//	//새로운 PIvot Rate 계산
	//	//------------------------------------------------------
	//	float fNewPivotX = (iPivotX - iStartX) / (float)iCutSizeX;
	//	float fNewPivotY = (iPivotY - iStartY) / (float)iCutSizeY;
	//	//------------------------------------------------------


	//}
	////-----------------------------


	return CView::PreTranslateMessage(pMsg);
}