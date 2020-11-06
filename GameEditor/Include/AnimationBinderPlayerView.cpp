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
	////Play��ư�� ���� ��� Draw
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

	//	//���� ����ؾ��� �̹����� Cut������ Pivot���� Setting
	//	//--------------------------------------------------------------------------------
	//	RECT tCurRect = m_pParentDlg->m_pAnimEditView->GetCutInfo(m_iCurPrintImageIdx);
	//	Vector2 tCurPivot = m_pParentDlg->m_pAnimEditView->GetPivotInfo(m_iCurPrintImageIdx);
	//	//--------------------------------------------------------------------------------

	//	const CImage& tCurImage = m_pParentDlg->m_pAnimEditView->GetImage();

	//	//Pivot ����
	//	//-------------------------------
	//	int iStartX = m_iViewWidth / 2;
	//	int iStartY = m_iViewHeight / 2;

	//	int iCutSizeX = tCurRect.right - tCurRect.left;
	//	int iCutSizeY = tCurRect.bottom - tCurRect.top;

	//	iStartX = (int)(iStartX - iCutSizeX * tCurPivot.x);
	//	iStartY = (int)(iStartY - iCutSizeY * tCurPivot.y);
	//	//-------------------------------

	//	//�ش� ������ŭ �̹��� ���
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
		//Pivot ����
		//-------------------------------
		int iStartX = m_iViewWidth / 2;
		int iStartY = m_iViewHeight / 2;

		int iCutSizeX = (tParentCutInfo.right - tParentCutInfo.left) * m_iCurScale;
		int iCutSizeY = (tParentCutInfo.bottom - tParentCutInfo.top) * m_iCurScale;

		iStartX = (int)(iStartX - iCutSizeX * tParentPivotInfo.x);
		iStartY = (int)(iStartY - iCutSizeY * tParentPivotInfo.y);
		//-------------------------------

		//�ش� ������ŭ �̹��� ���
		tParentImage.Draw(m_hBackDC, iStartX, iStartY, iCutSizeX, iCutSizeY, tParentCutInfo.left, tParentCutInfo.top, iCutSizeX / m_iCurScale, iCutSizeY / m_iCurScale);
	}

	if (!tChildImage.IsNull())
	{
		//Pivot ����
		//-------------------------------
		int iStartX = m_iViewWidth / 2;
		int iStartY = m_iViewHeight / 2;

		int iCutSizeX = (tChildCutInfo.right - tChildCutInfo.left) * m_iCurScale;
		int iCutSizeY = (tChildCutInfo.bottom - tChildCutInfo.top) * m_iCurScale;

		iStartX = (int)((iStartX - iCutSizeX * tChildPivotInfo.x) + tChildRelativePos.x);
		iStartY = (int)((iStartY - iCutSizeY * tChildPivotInfo.y) + tChildRelativePos.y);
		//-------------------------------

		//�ش� ������ŭ �̹��� ���
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

	//x��
	//---------------------------------------------
	int iViewHeightHalf = m_iViewHeight / 2;
	MoveToEx(hDC, 0, iViewHeightHalf, NULL);
	LineTo(hDC, m_iViewWidth, iViewHeightHalf);
	//---------------------------------------------

	//y��
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
	////�̹����� 
	//int iImageCnt = m_pParentDlg->GetCutImageCnt();

	////���� �ִϸ��̼��� �ӵ�
	//float fAnimPlayRate = m_pParentDlg->m_fPlayRate;

	////���� �ִϸ��̼� ��� �ð�
	//float fAnimPlayTime = m_pParentDlg->m_fPlayTime;

	////�ֱ� Setting
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

	//BackBuffer�� �ȼ� ������ �����ϱ� ���� ��Ʈ�� ����
	m_hBackBit = CreateCompatibleBitmap(m_hDC, m_iViewWidth, m_iViewHeight);

	//������ ��Ʈ���� BackDC�� Set
	m_hOldBit = (HBITMAP)SelectObject(m_hBackDC, m_hBackBit);
}

BOOL CAnimationBinderPlayerView::PreTranslateMessage(MSG* pMsg)
{
	////����Ű �Է½� 
	////-----------------------------
	//int iDiffX = 0;
	//int iDiffY = 0;
	//if (pMsg->message == WM_KEYDOWN)
	//{
	//	//�̹����� ������ pass
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

	//	//�̹��� �߽� ��ǥ
	//	//------------------------------------------------------
	//	int iCutWidthHalfSize = (tCurRect.right - tCurRect.left) / 2;
	//	int iCutHeightHalfSize = (tCurRect.bottom - tCurRect.top) / 2;
	//	//------------------------------------------------------

	//	//���� �ǹ� ��ǥ
	//	//------------------------------------------------------
	//	int iStartX = m_iViewWidth / 2;
	//	int iStartY = m_iViewHeight / 2;

	//	int iCutSizeX = tCurRect.right - tCurRect.left;
	//	int iCutSizeY = tCurRect.bottom - tCurRect.top;

	//	int iPivotX = (int)(iStartX - iCutSizeX * tCurPivot.x);
	//	int iPivotY = (int)(iStartY - iCutSizeY * tCurPivot.y);
	//	//------------------------------------------------------

	//	//�Է� ����
	//	//------------------------------------------------------
	//	iPivotX += iDiffX;
	//	iPivotY += iDiffY;
	//	//------------------------------------------------------

	//	//���ο� PIvot Rate ���
	//	//------------------------------------------------------
	//	float fNewPivotX = (iPivotX - iStartX) / (float)iCutSizeX;
	//	float fNewPivotY = (iPivotY - iStartY) / (float)iCutSizeY;
	//	//------------------------------------------------------


	//}
	////-----------------------------


	return CView::PreTranslateMessage(pMsg);
}