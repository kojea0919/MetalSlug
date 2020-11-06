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
	//Load�Լ�
	bool LoadTexture(TCHAR* pFileName);

public:
	void RenderImage();

public:
	//Cut���� ����
	void SaveCurCutInfo();

	//��� Cut���� ����
	void RemoveAllCutInfo();

	//��� Cut���� ȭ�鿡 ���
	void PrintAllCutInfo();

public:
	//Pivot���� ����
	void SavePivotInfo(float fX,float fY);

	//Pivot���� ��ü
	void ChangePivotInfo(int iIdx, float fX, float fY);

	//��� Pivot���� ����
	void RemoveAllPivot();

	//��� Pivot���� ȭ�鿡 ���
	void PrintAllPivotInfo();
public:
	//Get�Լ�
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
	//Set�Լ�
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
	//�簢�� Setting
	void SetStartEnd();

	//�簢�� Draw
	void DrawRect(HDC hDC,int iLeft,int iTop,int iRight,int iBottom);

	//�̹����� ���� ���õ� �簢�� ���
	void DrawImageAndSelectRect();

private:
	//Start,End �������� �̹��� �ڸ���
	void CutImage();

private:
	//���� �ҷ��� �̹����� ���� ����
	//-----------------------------
	//�̹���
	CImage		m_image;

	//ColorKey
	COLORREF	m_ColorKey;

	//�ε� bool ����
	bool		m_bIsLoadImage;
	//-----------------------------

	//DC
	//---------------------------- -
	HDC			m_hDC;
	HDC			m_hBackDC;
	//---------------------------- -


	//Texture �ڸ��� ���� ����
	//-----------------------------
	//���콺 Ŭ�� bool����
	bool		m_bLMouseButtonDown;

	//���콺 Ŭ�� ����,�� ��ǥ
	//-------------------
	POINT		m_tStart;
	POINT		m_tEnd;
	//-------------------
	//-----------------------------

	//Viewũ��
	//-----------------------------
	int			m_iViewWidth;
	int			m_iViewHeight;
	//-----------------------------

	//ColorKey�� Setting�ؾ��ϴ� ���
	bool		m_bSelectColorKey;

	//������� ����� Cut��ǥ
	//-----------------------------
	vector<RECT>	m_vecCutRect;
	//-----------------------------

	//������� ����� Pivot��ǥ
	//-----------------------------
	vector<Vector2>	m_vecPivot;
	//-----------------------------

	class CAnimation2DEditorDlg* m_pParentDlg;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

