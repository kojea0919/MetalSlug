#pragma once


// CPlayerEditDlg 대화 상자

class CPlayerEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPlayerEditDlg)

public:
	CPlayerEditDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CPlayerEditDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PLAYER_EDITOR };
#endif

private:
	//추가할 AnimationName index
	//---------------------------
	int m_iUpperAnimSelIdx;
	int m_iLowerAnimSelIdx;
	//---------------------------

	//제거할 AnimationName Index
	//---------------------------
	int m_iRemoveUpperAnimIdx;
	int m_iRemoveLowerAnimIdx;
	//---------------------------

	//UpperAnimNameList 경로
	char m_strUpperFullPath[MAX_PATH];
	char m_strLowerFullPath[MAX_PATH];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CListBox m_AllAnimNameList1;
	CListBox m_AllAnimNameList2;
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListAllanim();
	afx_msg void OnLbnSelchangeListAllanim2();
	afx_msg void OnBnClickedButtonAddupperanim();
	CListBox m_UpperAnimNameList;

	CListBox m_LowerAnimNameList;
	afx_msg void OnBnClickedButtonAddloweranim();
	afx_msg void OnBnClickedButtonUppeersave();
	afx_msg void OnBnClickedButtonLowersave();
	afx_msg void OnBnClickedButtonRemoveupperanim();
	afx_msg void OnBnClickedButtonRemoveloweranim();
	afx_msg void OnLbnSelchangeListUpperanim();
	afx_msg void OnLbnSelchangeListLoweranim();
};
