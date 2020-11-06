// WorldObjectDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "WorldObjectDlg.h"
#include "ViewManager.h"
#include "DetailObjDlg.h"
#include "GameObject.h"

// CWorldObjectDlg

IMPLEMENT_DYNCREATE(CWorldObjectDlg, CFormView)

CWorldObjectDlg::CWorldObjectDlg()
	: CFormView(IDD_DIALOG_WORLDOBJECT)
{

}

CWorldObjectDlg::~CWorldObjectDlg()
{
}

void CWorldObjectDlg::AddObject(const string& strName)
{
	CString	strItem = CA2CT(strName.c_str());

	HTREEITEM hItem = m_WorldObjectTree.InsertItem(strItem, 0, 0, m_RootItem, TVI_LAST);

	TreeItem	tItem;
	tItem.hItem = hItem;
	tItem.strName = strItem;

	m_vecItem.push_back(tItem);

}

void CWorldObjectDlg::DestroyEditorObject(CGameObject* pObj)
{
	CString	strName = CA2CT(pObj->GetName().c_str());

	HTREEITEM	hDelete = 0;

	size_t	iSize = m_vecItem.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (m_vecItem[i].strName == strName)
		{
			hDelete = m_vecItem[i].hItem;
			auto	iter = m_vecItem.begin() + i;
			m_vecItem.erase(iter);
			break;
		}
	}

	if (hDelete)
		m_WorldObjectTree.DeleteItem(hDelete);

	GET_SINGLE(CViewManager)->GetDetailObjectDlg()->DeleteObject(pObj);
}

void CWorldObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_WORLDOBJECT, m_WorldObjectTree);
}

BEGIN_MESSAGE_MAP(CWorldObjectDlg, CFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_WORLDOBJECT, &CWorldObjectDlg::OnTvnSelchangedTreeWorldobject)
	ON_NOTIFY(NM_CLICK, IDC_TREE_WORLDOBJECT, &CWorldObjectDlg::OnNMClickTreeWorldobject)
END_MESSAGE_MAP()


// CWorldObjectDlg 진단

#ifdef _DEBUG
void CWorldObjectDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CWorldObjectDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CWorldObjectDlg 메시지 처리기


void CWorldObjectDlg::OnTvnSelchangedTreeWorldobject(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	/*if (m_WorldObjectTree.GetSelectedCount() == 0)
		return;

	HTREEITEM	hItem = m_WorldObjectTree.GetSelectedItem();

	CString	strSelectText = m_WorldObjectTree.GetItemText(hItem);

	GET_SINGLE(CViewManager)->GetDetailObjectDlg()->SetObject(strSelectText);*/
}


void CWorldObjectDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_RootItem = m_WorldObjectTree.InsertItem(TEXT("Root"), 0, 0, TVI_ROOT, TVI_LAST);
}

// 트리컨트롤의 오브젝트를 마우스로 클릭 했을때 들어온다.
void CWorldObjectDlg::OnNMClickTreeWorldobject(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	// 마우스 위치를 이용해서 클릭된 아이템을 찾는다.
	TV_HITTESTINFO	hInfo;

	::GetCursorPos(&hInfo.pt);

	::ScreenToClient(m_WorldObjectTree.m_hWnd, &hInfo.pt);

	HTREEITEM	hCurrentItem = m_WorldObjectTree.HitTest(&hInfo);

	if (hInfo.hItem != nullptr)
	{
		m_WorldObjectTree.Select(hCurrentItem, TVGN_CARET);

		CString	strSelectText = m_WorldObjectTree.GetItemText(hInfo.hItem);

		GET_SINGLE(CViewManager)->GetDetailObjectDlg()->SetObject(strSelectText);
	}
}
