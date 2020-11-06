#include "pch.h"
#include "AnimationEditorView.h"
#include "EditDevice.h"
#include "RectViewer.h"
#include "Animation2DEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CAnimationEditorView, CScrollView)

BEGIN_MESSAGE_MAP(CAnimationEditorView, CScrollView)
	ON_WM_MOUSEACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CAnimationEditorView::CAnimationEditorView()
	: m_bLMouseButtonDown(false), m_iViewWidth(0), m_iViewHeight(0),
	m_bIsLoadImage(false),m_bSelectColorKey(false),m_pParentDlg(nullptr)
{
	m_ColorKey = RGB(255, 255, 255);
}

CAnimationEditorView::~CAnimationEditorView()
{
	::ReleaseDC(m_hWnd, m_hDC);
}

void CAnimationEditorView::OnDraw(CDC*)
{

}


int CAnimationEditorView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return MA_ACTIVATE;
}


void CAnimationEditorView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, CSize(1000, 1000));

	m_hDC = ::GetDC(m_hWnd);
	
	//DlgSetting
	m_pParentDlg = (CAnimation2DEditorDlg*)GetParent();

	//기본 ColorKey Setting
	//-------------------------------------------------------------------
	m_pParentDlg->UpdateColorKey(m_ColorKey);
	//-------------------------------------------------------------------
}


BOOL CAnimationEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

void CAnimationEditorView::SaveInfo(FILE* pFile)
{
	if (!pFile)
		return;

	//cut정보 저장
	//--------------------------------
	int iSize = (int)m_vecCutRect.size();
	fprintf_s(pFile, "%d\n", iSize);

	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		RECT tCurRect = m_vecCutRect[iCnt];
		fprintf_s(pFile, "%d %d %d %d\n", tCurRect.left - 1, tCurRect.top -1, tCurRect.right + 1, tCurRect.bottom + 1);
	}

	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		Vector2 tCurPivot = m_vecPivot[iCnt];
		fprintf_s(pFile, "%f %f\n", tCurPivot.x, tCurPivot.y);
	}
	//--------------------------------
}

bool CAnimationEditorView::LoadTexture(TCHAR* pFileName)
{
	//이미 로드된 이미지가 있으면 해제
	//----------------------------------
	if (m_bIsLoadImage)
	{
		m_image.Destroy();
		m_vecCutRect.clear();
		m_vecPivot.clear();
	}
	//----------------------------------

	if(FAILED(m_image.Load(pFileName)))
		return false;

	m_bIsLoadImage = true;

	return true;
}

void CAnimationEditorView::RenderImage()
{
	HDC hDC = ::GetDC(m_hWnd);

	Rectangle(hDC, -1, -1, m_iViewWidth, m_iViewHeight);
	m_image.Draw(hDC, 0, 0);

	::ReleaseDC(m_hWnd,hDC);
}

void CAnimationEditorView::SaveCurCutInfo()
{
	RECT tTemp;
	tTemp.left = m_tStart.x;
	tTemp.top = m_tStart.y;
	tTemp.right = m_tEnd.x;
	tTemp.bottom = m_tEnd.y;
	m_vecCutRect.push_back(tTemp);
}

void CAnimationEditorView::RemoveAllCutInfo()
{
	m_vecCutRect.clear();
}

void CAnimationEditorView::PrintAllCutInfo()
{
	if (m_image.IsNull())
		return;

	size_t iSize = m_vecCutRect.size();

	HBITMAP hBackBit = NULL;
	HBITMAP hOldBit = NULL;
	m_hBackDC = CreateCompatibleDC(m_hDC);

	hBackBit = CreateCompatibleBitmap(m_hDC, m_iViewWidth, m_iViewHeight);
	hOldBit = (HBITMAP)SelectObject(m_hBackDC, hBackBit);

	Rectangle(m_hBackDC, -1, -1, m_iViewWidth, m_iViewHeight);
	m_image.Draw(m_hBackDC, 0, 0);

	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		RECT tCurRect = m_vecCutRect[iCnt];

		DrawRect(m_hBackDC, tCurRect.left, tCurRect.top, tCurRect.right, tCurRect.bottom);
	}


	BitBlt(m_hDC, 0, 0, m_iViewWidth, m_iViewHeight, m_hBackDC, 0, 0, SRCCOPY);

	SelectObject(m_hBackDC, hOldBit);
	DeleteObject(hBackBit);
	DeleteDC(m_hBackDC);
}

void CAnimationEditorView::SavePivotInfo(float fX, float fY)
{
	m_vecPivot.push_back(Vector2(fX, fY));
}

void CAnimationEditorView::ChangePivotInfo(int iIdx, float fX, float fY)
{
	m_vecPivot[iIdx].x = fX;
	m_vecPivot[iIdx].y = fY;
}

void CAnimationEditorView::RemoveAllPivot()
{
	m_vecPivot.clear();
}

void CAnimationEditorView::PrintAllPivotInfo()
{
	if (m_image.IsNull())
		return;

	size_t iLen = m_vecPivot.size();

	HBITMAP hBackBit = NULL;
	HBITMAP hOldBit = NULL;
	m_hBackDC = CreateCompatibleDC(m_hDC);

	hBackBit = CreateCompatibleBitmap(m_hDC, m_iViewWidth, m_iViewHeight);
	hOldBit = (HBITMAP)SelectObject(m_hBackDC, hBackBit);

	Rectangle(m_hBackDC, 0, 0, m_iViewWidth, m_iViewHeight);
	m_image.Draw(m_hBackDC, 0, 0);

	HPEN Pen = CreatePen(1, 3, RGB(255, 0, 0));

	HPEN OldPen = (HPEN)SelectObject(m_hBackDC, Pen);

	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
	{
		Vector2 vCurPivot = m_vecPivot[iCnt];
		RECT tTargetCutInfo = m_vecCutRect[iCnt];

		//해당 피벗을 중심으로 십자가 그리기
		//--------------------------------
		int iX = (int)((tTargetCutInfo.right - tTargetCutInfo.left) * vCurPivot.x + tTargetCutInfo.left);
		int iY = (int)((tTargetCutInfo.bottom - tTargetCutInfo.top) * vCurPivot.y + tTargetCutInfo.top);

		MoveToEx(m_hBackDC, iX, iY - 1, nullptr);
		LineTo(m_hBackDC, iX, iY - 5);

		MoveToEx(m_hBackDC, iX - 1, iY, nullptr);
		LineTo(m_hBackDC, iX - 5, iY);

		MoveToEx(m_hBackDC, iX, iY + 1, nullptr);
		LineTo(m_hBackDC, iX, iY + 5);

		MoveToEx(m_hBackDC, iX + 1, iY, nullptr);
		LineTo(m_hBackDC, iX + 5, iY);
		//--------------------------------
	}

	SelectObject(m_hBackDC, OldPen);
	DeleteObject(Pen);

	BitBlt(m_hDC, 0, 0, m_iViewWidth, m_iViewHeight, m_hBackDC, 0, 0, SRCCOPY);

	SelectObject(m_hBackDC, hOldBit);
	DeleteObject(hBackBit);
	DeleteDC(m_hBackDC);
}

void CAnimationEditorView::Clear()
{
	if (!m_image.IsNull())
	{
		m_image.Destroy();
		m_vecCutRect.clear();
		m_vecPivot.clear();
	}

	m_bSelectColorKey = false;
	m_bLMouseButtonDown = false;
}

void CAnimationEditorView::Save(FILE* pFile)
{
	if (!pFile)
		return;

	//Cut,Pivot정보 저장
	//---------------------------------------
	int iSize = (int)m_vecCutRect.size();
	fprintf_s(pFile, "%d\n", iSize);

	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		fprintf_s(pFile, "%d %d %d %d\n", m_vecCutRect[iCnt].left, m_vecCutRect[iCnt].top, m_vecCutRect[iCnt].right, m_vecCutRect[iCnt].bottom);
	}

	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		fprintf_s(pFile, "%f %f\n", m_vecPivot[iCnt].x, m_vecPivot[iCnt].y);
	}
	//---------------------------------------
}

void CAnimationEditorView::SetStartEnd()
{
	//위치를 비교해서 왼쪽 위 좌표를 Start로 지정
	//---------------------------------------
	int iOldX = m_tStart.x;
	int iOldY = m_tStart.y;
	int iNewX = m_tEnd.x;
	int iNewY = m_tEnd.y;

	if (iOldX > iNewX)
	{
		//오른쪽 아래에서 왼쪽 위로 드래그
		if (iOldY > iNewY)
		{
			swap(iOldY, iNewY);
			swap(iOldX, iNewX);
		}
		//오른쪽 위에서 왼쪽 아래로 드래그
		else if (iOldY < iNewY)
		{
			swap(iOldX, iNewX);
		}
	}
	else
	{
		//왼쪽 아래에서 위쪽 위로
		if (iOldY > iNewY)
		{
			swap(iNewY, iOldY);
		}
	}

	m_tStart.x = iOldX;
	m_tStart.y = iOldY;
	m_tEnd.x = iNewX;
	m_tEnd.y = iNewY;
	//---------------------------------------
}

void CAnimationEditorView::DrawRect(HDC hDC, int iLeft, int iTop, int iRight, int iBottom)
{
	HBRUSH OldBrush, NullBrush;

	HPEN Pen = CreatePen(1, 1, RGB(255, 0, 255));

	HPEN OldPen = (HPEN)SelectObject(hDC, Pen);

	NullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	OldBrush = (HBRUSH)SelectObject(hDC, NullBrush);
	Rectangle(hDC, iLeft, iTop,	iRight, iBottom);
	SelectObject(hDC, OldBrush);
	SelectObject(hDC, OldPen);
	DeleteObject(NullBrush);
}

void CAnimationEditorView::DrawImageAndSelectRect()
{
	//사각형 영역 그리기
	//------------------------------------------------
	HBITMAP hBackBit = NULL;
	HBITMAP hOldBit = NULL;
	m_hBackDC = CreateCompatibleDC(m_hDC);

	hBackBit = CreateCompatibleBitmap(m_hDC, m_iViewWidth, m_iViewHeight);
	hOldBit = (HBITMAP)SelectObject(m_hBackDC, hBackBit);

	Rectangle(m_hBackDC, -1, -1, m_iViewWidth, m_iViewHeight);

	m_image.Draw(m_hBackDC, 0, 0);
	DrawRect(m_hBackDC,m_tStart.x,m_tStart.y,m_tEnd.x,m_tEnd.y);

	BitBlt(m_hDC, 0, 0, m_iViewWidth, m_iViewHeight, m_hBackDC, 0, 0, SRCCOPY);

	SelectObject(m_hBackDC, hOldBit);
	DeleteObject(hBackBit);
	DeleteDC(m_hBackDC);
	//------------------------------------------------
}

void CAnimationEditorView::CutImage()
{
	int iLeft = m_tStart.x;	
	int iTop = m_tStart.y;
	int iRight = m_tEnd.x;
	int iBottom = m_tEnd.y;

	HDC hDC = m_hDC;
	COLORREF tColorKey = m_ColorKey;
	COLORREF tPenColor = RGB(255,0,255);
	
	if (iLeft > iRight || iTop > iBottom || iRight - iLeft > m_image.GetWidth() || iBottom - iTop > m_image.GetHeight())
		return;

	//왼쪽 탐색
	//---------------------------------------------------
	for (size_t iX = iLeft; iX <= iRight; ++iX)
	{
		bool bIsSearch = false;
		for (size_t iY = iTop; iY <= iBottom; ++iY)
		{
			COLORREF tCurColor = GetPixel(hDC, (int)iX, (int)iY);

			if (tColorKey != tCurColor && tPenColor != tCurColor)
			{
				iLeft = (int)iX;
				bIsSearch = true;
				break;
			}
		}
		if (bIsSearch)
			break;
	}
	//---------------------------------------------------

	//오른쪽 탐색
	//---------------------------------------------------
	for (size_t iX = iRight; iX >= iLeft; --iX)
	{
		bool bIsSearch = false;
		for (size_t iY = iTop; iY <= iBottom; ++iY)
		{
			COLORREF tCurColor = GetPixel(hDC, (int)iX, (int)iY);
			if (tColorKey != tCurColor && tPenColor != tCurColor)
			{
				iRight = (int)iX;
				bIsSearch = true;
				break;
			}
		}
		if (bIsSearch)
			break;
	}
	//---------------------------------------------------

	//위쪽 탐색
	//---------------------------------------------------
	for (size_t iY = iTop; iY <= iBottom; ++iY)
	{
		bool bIsSearch = false;
		for (size_t iX = iLeft; iX <= iRight; ++iX)
		{
			COLORREF tCurColor = GetPixel(hDC, (int)iX, (int)iY);
			if (tColorKey != tCurColor && tPenColor != tCurColor)
			{
				iTop = (int)iY;
				bIsSearch = true;
				break;
			}
		}
		if (bIsSearch)
			break;
	}
	//---------------------------------------------------

	//아래쪽 탐색
	//---------------------------------------------------
	for (size_t iY = iBottom; iY >= iTop; --iY)
	{
		bool bIsSearch = false;
		for (size_t iX = iLeft; iX <= iRight; ++iX)
		{
			COLORREF tCurColor = GetPixel(hDC, (int)iX, (int)iY);
			if (tColorKey != tCurColor && tPenColor != tCurColor)
			{
				iBottom = (int)iY;
				bIsSearch = true;
				break;
			}
		}
		if (bIsSearch)
			break;
	}
	//---------------------------------------------------

	m_tStart.x = iLeft;
	m_tStart.y = iTop;
	m_tEnd.x = iRight;
	m_tEnd.y = iBottom;
}

void CAnimationEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//ColorKey를 설정해야하는 경우 Setting
	//------------------------------------
	if (m_bSelectColorKey)
	{
		m_ColorKey = GetPixel(m_hDC, point.x, point.y);
		
		m_pParentDlg->UpdateColorKey(m_ColorKey);
		m_bSelectColorKey = false;
	}
	//------------------------------------

	m_bLMouseButtonDown = true;

	m_tStart.x = point.x;
	m_tStart.y = point.y;
	
	CScrollView::OnLButtonDown(nFlags, point);
}


void CAnimationEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_image.IsNull())
		return;

	m_bLMouseButtonDown = false;

	m_tEnd.x = point.x;
	m_tEnd.y = point.y;

	//드래그 방향에 상관없이 왼쪽위, 오른쪽 아래점 Settting
	SetStartEnd();

	//해당 영역 이미지 자르기
	CutImage();

	//자른 이미지 출력
	DrawImageAndSelectRect();

	//Texture 출력
	m_pParentDlg->UpdateCutInfo(m_tStart, m_tEnd);

	CScrollView::OnLButtonUp(nFlags, point);
}

void CAnimationEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	//마우스가 클릭된 상태인 경우 드래그한 영역 Setting
	//------------------------------------------------
	if (m_bLMouseButtonDown)
	{
		//이미지 로드가 안된경우 pass
		//---------------------------
		if (!m_bIsLoadImage)
			return;
		//---------------------------

		m_tEnd.x = point.x;
		m_tEnd.y = point.y;

		DrawImageAndSelectRect();
	}
	//------------------------------------------------

	CScrollView::OnMouseMove(nFlags, point);
}
