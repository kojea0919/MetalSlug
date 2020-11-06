#pragma once

// CAnimation2DEditorDlg 대화 상자


class CAnimation2DEditorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimation2DEditorDlg)

public:
	CAnimation2DEditorDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAnimation2DEditorDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ANIM2DEDITOR };
#endif

public:
	//ColorKey Update
	//----------------------------
	void UpdateColorKey(const COLORREF & ColorKey);
	//----------------------------

	//CutInfo Update
	//----------------------------
	void UpdateCutInfo(const POINT & tStart,const POINT & tEnd);
	//----------------------------

public:
	class CAnimationEditorView* GetEditorView() const
	{
		return m_pAnimEditView;
	}

	int GetCutImageCnt() const
	{
		return m_CutInfoList.GetItemCount();
	}

private:
	void InitCutInfoList();
	void InitPivotInfoList();

private:
	void SaveAniFile(const char * pFileName, const char * pFullPath);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	//애니메이션 SettingView
	class CAnimationEditorView* m_pAnimEditView;

	//애니메이션 PlayerView
	class CAnimationPlayerView* m_pAnimPlayerView;


	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSave();
	CStatic m_FileNameStatic;
	CStatic m_ImageWidthStatic;
	CStatic m_ImageHeightStatic;
	afx_msg void OnBnClickedButtonColorkey();
	CStatic m_RColorKey;
	CStatic m_GColorKey;
	CStatic m_BColorKey;
	CStatic m_CutLeft;
	CStatic m_CutTop;
	CStatic m_CutRight;
	CStatic m_CutBottom;
	afx_msg void OnBnClickedButtonCutsave();

	//ListControl 비우는 버튼 클릭
	afx_msg void OnBnClickedButtonCutlistclear();

	//모든 Cut정보 화면에 출력
	afx_msg void OnBnClickedButtonPrintcutinfo();

	//Pivot값
	//----------------
	float m_fPivotX;
	float m_fPivotY;
	//----------------

	char m_strTextureFileName[MAX_PATH];

	int m_iCurSelectImageIdx;

	CListCtrl m_CutInfoList;
	CListCtrl m_PivotInfoList;

	//Pivot 저장 버튼
	//-------------------------------------------
	afx_msg void OnBnClickedButtonPivotallsave();
	afx_msg void OnBnClickedButtonPivotsave();
	//-------------------------------------------
	afx_msg void OnBnClickedButtonPivotlistclear();
	afx_msg void OnBnClickedButtonPrintpivotinfo();
	afx_msg void OnBnClickedButtonAnimplay();
	float m_fPlayRate;
	float m_fPlayTime;
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonAnimstop();
	afx_msg void OnNMClickListPivotinfo(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_strAnimName;
	CButton m_Loop;
	CButton m_Stop;
};
