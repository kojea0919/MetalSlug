#pragma once



// CWorldObjectDlg 폼 보기

class CWorldObjectDlg : public CFormView
{
	DECLARE_DYNCREATE(CWorldObjectDlg)

protected:
	CWorldObjectDlg();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CWorldObjectDlg();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WORLDOBJECT };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	vector<TreeItem>	m_vecItem;

public:
	void AddObject(const string& strName);
	void DestroyEditorObject(class CGameObject* pObj);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_WorldObjectTree;
	HTREEITEM	m_RootItem;
	afx_msg void OnTvnSelchangedTreeWorldobject(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnInitialUpdate();
	afx_msg void OnNMClickTreeWorldobject(NMHDR* pNMHDR, LRESULT* pResult);
};


