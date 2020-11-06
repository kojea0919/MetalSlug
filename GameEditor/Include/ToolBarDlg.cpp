// ToolBarDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "ToolBarDlg.h"


// CToolBarDlg

IMPLEMENT_DYNCREATE(CToolBarDlg, CFormView)

CToolBarDlg::CToolBarDlg()
	: CFormView(IDD_DIALOG_TOOLBAR)
{

}

CToolBarDlg::~CToolBarDlg()
{
}

void CToolBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CToolBarDlg, CFormView)
END_MESSAGE_MAP()


// CToolBarDlg 진단

#ifdef _DEBUG
void CToolBarDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CToolBarDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CToolBarDlg 메시지 처리기
