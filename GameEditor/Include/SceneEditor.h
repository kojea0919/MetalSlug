#pragma once


// CSceneEditor 대화 상자

class CSceneEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CSceneEditor)

public:
	CSceneEditor(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSceneEditor();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SCENEEDITOR };
#endif

public:
	void UpdateSceneEditorSequenceFileName();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAnimresourceset();
	CStatic m_AnimResourceFileName;
};
