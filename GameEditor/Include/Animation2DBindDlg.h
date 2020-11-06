#pragma once


// CAnimation2DBindDlg 대화 상자

class CAnimation2DBindDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimation2DBindDlg)

public:
	CAnimation2DBindDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAnimation2DBindDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ANIMCOMBINEEDIT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	//Get함수
	//----------------------------
	RECT GetParentCutInfo(int iIdx) const
	{
		return m_vecParentCutRect[iIdx];
	}
	Vector2 GetParentPivotInfo(int iIdx) const
	{
		return m_vecParentPivot[iIdx];
	}
	RECT GetChildCutInfo(int iIdx) const
	{
		return m_vecChildCutRect[iIdx];
	}
	Vector2 GetChildPivotInfo(int iIdx) const
	{
		return m_vecChildPivot[iIdx];
	}
	//----------------------------
private:
	void ReadParentAniFile(const char* pFullPath);

	void ReadChildAniFile(const char* pFullPath);

private:
	void InitParentAniInfoList();
	void InitChildAniInfoList();

private:
	CImage			m_ParentImage;
	CImage			m_ChildImage;

	//Parent Cut좌표
	//-----------------------------
	vector<RECT>	m_vecParentCutRect;
	//-----------------------------

	//Child Cut좌표
	//-----------------------------
	vector<RECT>	m_vecChildCutRect;
	//-----------------------------
	
	//Parent Pivot
	//-----------------------------
	vector<Vector2>	m_vecParentPivot;
	//-----------------------------

	//Child Pivot
	//-----------------------------
	vector<Vector2>	m_vecChildPivot;
	//-----------------------------

	//Child Connect Pos
	//-----------------------------
	vector<Vector2> m_vecConnectPos;
	//-----------------------------

	//재생 속도, 시간
	//-----------------------------
	float			m_fPlayRate;
	float			m_fPlayTime;
	//-----------------------------

	//현재 선택중인 Image index
	int				m_iCurSelectParentImageIdx;
	int				m_iCurSelectChildImageIdx;
	int				m_iCurSelectConnectIdx;

	char m_strParentFullPath[MAX_PATH];


	//자식 애니메이션 이름
	char m_strChildAnimName[MAX_PATH];

public:
	//애니메이션 Player View
	class CAnimationBinderPlayerView* m_pAnimPlayerView;

	virtual BOOL OnInitDialog();
	CString m_strParentAniFileName;
	CString m_strChildAniFileName;
	afx_msg void OnBnClickedButtonParentaniload();
	afx_msg void OnBnClickedButtonChildaniload();
	CListCtrl m_ParentAniList;
	CListCtrl m_ChildAniList;
	afx_msg void OnLvnItemchangedListChildani(NMHDR* pNMHDR, LRESULT* pResult);
	CString m_strRelativeX;
	CString m_strRelativeY;
	afx_msg void OnLvnItemchangedListParentani(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonZoomin();
	afx_msg void OnBnClickedButtonZoomout();
	afx_msg void OnBnClickedButtonInsert();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonAllinsert();
	afx_msg void OnBnClickedButtonSave();
};
