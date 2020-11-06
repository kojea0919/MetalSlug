#pragma once
#include <afxwin.h>
#include "TextureViewer.h"

class CAnimationEditorView : public CScrollView
{
	friend class CAnimation2DEditorDlg;

protected:
	CAnimationEditorView();
	~CAnimationEditorView();
	DECLARE_DYNCREATE(CAnimationEditorView)

public:
	virtual void OnDraw(CDC* /*pDC*/);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	void SaveInfo(FILE* pFile);

public:
	//Load함수
	bool LoadTexture(TCHAR* pFileName);

public:
	void RenderImage();

public:
	//Cut정보 저장
	void SaveCurCutInfo();

	//모든 Cut정보 삭제
	void RemoveAllCutInfo();

	//모든 Cut정보 화면에 출력
	void PrintAllCutInfo();

public:
	//Pivot정보 저장
	void SavePivotInfo(float fX,float fY);

	//Pivot정보 교체
	void ChangePivotInfo(int iIdx, float fX, float fY);

	//모든 Pivot정보 삭제
	void RemoveAllPivot();

	//모든 Pivot정보 화면에 출력
	void PrintAllPivotInfo();
public:
	//Get함수
	//--------------------------
	size_t GetWidth() const
	{
		return m_image.GetWidth();
	}
	size_t GetHeight() const
	{
		return m_image.GetHeight();
	}
	DWORD GetRedColorKey()
	{
		return GetRValue(m_ColorKey);
	}
	DWORD GetGreenColorKey()
	{
		return GetGValue(m_ColorKey);
	}
	DWORD GetBlueColorKey()
	{
		return GetBValue(m_ColorKey);
	}
	const RECT & GetCutInfo(int iIdx)const
	{
		return m_vecCutRect[iIdx];
	}
	const Vector2 & GetPivotInfo(int iIdx) const
	{
		return m_vecPivot[iIdx];
	}
	const CImage& GetImage() const
	{
		return m_image;
	}
	//--------------------------

public:
	//Set함수
	//--------------------------
	void SetViewWidth(int iViewWidth)
	{
		m_iViewWidth = iViewWidth;
	}
	void SetViewHeight(int iViewHeight)
	{
		m_iViewHeight = iViewHeight;
	}
	void SetColorKeySelect()
	{
		m_bSelectColorKey = true;
	}
	//--------------------------

public:
	void Clear();

public:
	void Save(FILE* pFile);

private:
	//사각형 Setting
	void SetStartEnd();

	//사각형 Draw
	void DrawRect(HDC hDC,int iLeft,int iTop,int iRight,int iBottom);

	//이미지와 현재 선택된 사각형 출력
	void DrawImageAndSelectRect();

private:
	//Start,End 영역에서 이미지 자르기
	void CutImage();

private:
	//현재 불러온 이미지에 대한 정보
	//-----------------------------
	//이미지
	CImage		m_image;

	//ColorKey
	COLORREF	m_ColorKey;

	//로드 bool 변수
	bool		m_bIsLoadImage;
	//-----------------------------

	//DC
	//---------------------------- -
	HDC			m_hDC;
	HDC			m_hBackDC;
	//---------------------------- -


	//Texture 자르기 관련 변수
	//-----------------------------
	//마우스 클릭 bool변수
	bool		m_bLMouseButtonDown;

	//마우스 클릭 시작,끝 좌표
	//-------------------
	POINT		m_tStart;
	POINT		m_tEnd;
	//-------------------
	//-----------------------------

	//View크기
	//-----------------------------
	int			m_iViewWidth;
	int			m_iViewHeight;
	//-----------------------------

	//ColorKey를 Setting해야하는 경우
	bool		m_bSelectColorKey;

	//현재까지 저장된 Cut좌표
	//-----------------------------
	vector<RECT>	m_vecCutRect;
	//-----------------------------

	//현재까지 저장된 Pivot좌표
	//-----------------------------
	vector<Vector2>	m_vecPivot;
	//-----------------------------

	class CAnimation2DEditorDlg* m_pParentDlg;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

