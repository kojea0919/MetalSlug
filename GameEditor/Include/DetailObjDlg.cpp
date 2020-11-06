// DetailObjDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "DetailObjDlg.h"
#include "GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/SceneComponent.h"
#include "Engine.h"

// CDetailObjDlg

IMPLEMENT_DYNCREATE(CDetailObjDlg, CFormView)

CDetailObjDlg::CDetailObjDlg()
	: CFormView(IDD_DIALOG_DETAILOBJ)
{

}

CDetailObjDlg::~CDetailObjDlg()
{
	SAFE_RELEASE(m_pSelectComponent);
}

void CDetailObjDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_COMPONENT, m_ComponentTree);
	DDX_Text(pDX, IDC_EDIT_SCALE_X, m_vRelativeScale.x);
	DDX_Text(pDX, IDC_EDIT_SCALE_Y, m_vRelativeScale.y);
	DDX_Text(pDX, IDC_EDIT_SCALE_Z, m_vRelativeScale.z);
	DDX_Text(pDX, IDC_EDIT_ROTATION_X, m_vRelativeRot.x);
	DDX_Text(pDX, IDC_EDIT_ROTATION_Y, m_vRelativeRot.y);
	DDX_Text(pDX, IDC_EDIT_ROTATION_Z, m_vRelativeRot.z);
	DDX_Text(pDX, IDC_EDIT_POS_X, m_vRelativePos.x);
	DDX_Text(pDX, IDC_EDIT_POS_Y, m_vRelativePos.y);
	DDX_Text(pDX, IDC_EDIT_POS_Z, m_vRelativePos.z);
}

void CDetailObjDlg::SetObject(const CString& strName)
{
	string	strConvertName = CT2CA(strName);

	// Scene에서 해당 오브젝트를 찾아서 표시해준다.
	m_pSelectObject = GET_SINGLE(CSceneManager)->GetScene()->FindObject(strConvertName);

	if (!m_pSelectObject)
		return;

	else if (!m_pSelectObject->IsActive())
	{
		SAFE_RELEASE(m_pSelectObject);
		return;
	}

	m_ComponentTree.DeleteAllItems();
	m_vecItem.clear();

	m_RootItem = m_ComponentTree.InsertItem(TEXT("Root"), 0, 0, TVI_ROOT, TVI_LAST);

	vector<HierarchyName>	vecSceneComName;

	m_pSelectObject->GetAllSceneComponentName(vecSceneComName);

	for (size_t i = 0; i < vecSceneComName.size(); ++i)
	{
		CString	strItemName = CA2CT(vecSceneComName[i].strName.c_str());
		if (vecSceneComName[i].strParent.empty())
		{
			HTREEITEM hItem = m_ComponentTree.InsertItem(strItemName, 0, 0, m_RootItem, TVI_LAST);
			TreeItem	tItem;
			tItem.hItem = hItem;
			tItem.strName = strItemName;

			m_vecItem.push_back(tItem);
		}

		else
		{
			CString	strParentName = CA2CT(vecSceneComName[i].strParent.c_str());

			HTREEITEM	hParent = 0;
			// 부모의 아이템 핸들을 찾아온다.
			for (size_t j = 0; j < m_vecItem.size(); ++j)
			{
				if (m_vecItem[j].strName == strParentName)
				{
					hParent = m_vecItem[j].hItem;
					break;
				}
			}

			HTREEITEM hItem = m_ComponentTree.InsertItem(strItemName, 0, 0, hParent, TVI_LAST);
			TreeItem	tItem;
			tItem.hItem = hItem;
			tItem.strName = strItemName;

			m_vecItem.push_back(tItem);
		}
	}

	m_pSelectObject->Release();
}

void CDetailObjDlg::Update(float fTime)
{
	if (m_pSelectComponent)
	{
		m_vRelativeScale = m_pSelectComponent->GetWorldScale();
		m_vRelativeRot = m_pSelectComponent->GetWorldRot();
		m_vRelativePos = m_pSelectComponent->GetWorldPos();

		UpdateData(FALSE);
	}
}

void CDetailObjDlg::DeleteObject(CGameObject* pObj)
{
	if (m_pSelectObject == pObj)
		m_pSelectObject = nullptr;
}

BEGIN_MESSAGE_MAP(CDetailObjDlg, CFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_COMPONENT, &CDetailObjDlg::OnTvnSelchangedTreeComponent)
	ON_EN_CHANGE(IDC_EDIT_SCALE_X, &CDetailObjDlg::OnEnChangeEditScaleX)
	ON_EN_CHANGE(IDC_EDIT_ROTATION_X, &CDetailObjDlg::OnEnChangeEditRotationX)
	ON_EN_CHANGE(IDC_EDIT_POS_X, &CDetailObjDlg::OnEnChangeEditPosX)
	ON_EN_CHANGE(IDC_EDIT_SCALE_Y, &CDetailObjDlg::OnEnChangeEditScaleY)
	ON_EN_CHANGE(IDC_EDIT_SCALE_Z, &CDetailObjDlg::OnEnChangeEditScaleZ)
	ON_EN_CHANGE(IDC_EDIT_ROTATION_Y, &CDetailObjDlg::OnEnChangeEditRotationY)
	ON_EN_CHANGE(IDC_EDIT_ROTATION_Z, &CDetailObjDlg::OnEnChangeEditRotationZ)
	ON_EN_CHANGE(IDC_EDIT_POS_Y, &CDetailObjDlg::OnEnChangeEditPosY)
	ON_EN_CHANGE(IDC_EDIT_POS_Z, &CDetailObjDlg::OnEnChangeEditPosZ)
END_MESSAGE_MAP()


// CDetailObjDlg 진단

#ifdef _DEBUG
void CDetailObjDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDetailObjDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDetailObjDlg 메시지 처리기


void CDetailObjDlg::OnTvnSelchangedTreeComponent(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	HTREEITEM	hItem = m_ComponentTree.GetSelectedItem();

	size_t	iSize = m_vecItem.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (hItem == m_vecItem[i].hItem)
		{
			string	strName = CT2CA(m_vecItem[i].strName);

			SAFE_RELEASE(m_pSelectComponent);
			m_pSelectComponent = m_pSelectObject->FindSceneComponent(strName);

			if (m_pSelectComponent)
			{
				m_vRelativeScale = m_pSelectComponent->GetWorldScale();
				m_vRelativeRot = m_pSelectComponent->GetWorldRot();
				m_vRelativePos = m_pSelectComponent->GetWorldPos();

				UpdateData(FALSE);
			}

			break;
		}
	}
}


void CDetailObjDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_RootItem = m_ComponentTree.InsertItem(TEXT("Root"), 0, 0, TVI_ROOT, TVI_LAST);

	GET_SINGLE(CEngine)->SetEditorUpdateCallback<CDetailObjDlg>(this,
		&CDetailObjDlg::Update);
}


void CDetailObjDlg::OnEnChangeEditScaleX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}




void CDetailObjDlg::OnEnChangeEditScaleY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDetailObjDlg::OnEnChangeEditScaleZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CDetailObjDlg::OnEnChangeEditRotationX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDetailObjDlg::OnEnChangeEditRotationY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDetailObjDlg::OnEnChangeEditRotationZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CDetailObjDlg::OnEnChangeEditPosX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDetailObjDlg::OnEnChangeEditPosY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDetailObjDlg::OnEnChangeEditPosZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
