#pragma once



// CDetailObjDlg 폼 보기
#include "pch.h"
#include "GameEngine.h"

class CDetailObjDlg : public CFormView
{
	DECLARE_DYNCREATE(CDetailObjDlg)

protected:
	CDetailObjDlg();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CDetailObjDlg();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DETAILOBJ };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

private:
	//World창에서 클릭된 Object
	class CGameObject* m_pSelectObject;

	//Detail창에서 클릭된 Component
	class CSceneComponent* m_pSelectComponent;

public:
	void SetObject(const CString& strName);
	void Update(float fTime);
	void DeleteObject(class CGameObject* pObj);

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_ComponentTree;
	HTREEITEM	m_RootItem;
	vector<TreeItem>	m_vecItem;
	afx_msg void OnTvnSelchangedTreeComponent(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnInitialUpdate();
	afx_msg void OnEnChangeEditScaleX();
	Vector3 m_vRelativeScale;
	Vector3 m_vRelativeRot;
	afx_msg void OnEnChangeEditRotationX();
	Vector3 m_vRelativePos;
	afx_msg void OnEnChangeEditPosX();
	afx_msg void OnEnChangeEditScaleY();
	afx_msg void OnEnChangeEditScaleZ();
	afx_msg void OnEnChangeEditRotationY();
	afx_msg void OnEnChangeEditRotationZ();
	afx_msg void OnEnChangeEditPosY();
	afx_msg void OnEnChangeEditPosZ();
};


