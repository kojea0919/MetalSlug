// CAnimation2DBindDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "Animation2DBindDlg.h"
#include "afxdialogex.h"
#include "AnimationBinderPlayerView.h"	
#include "PathManager.h"

// CAnimation2DBindDlg 대화 상자

IMPLEMENT_DYNAMIC(CAnimation2DBindDlg, CDialogEx)

CAnimation2DBindDlg::CAnimation2DBindDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ANIMCOMBINEEDIT, pParent)
	, m_strParentAniFileName(_T(""))
	, m_strChildAniFileName(_T(""))
	, m_iCurSelectChildImageIdx(0)
	, m_iCurSelectParentImageIdx(0)
	, m_strRelativeX(_T(""))
	, m_strRelativeY(_T(""))
{
}

CAnimation2DBindDlg::~CAnimation2DBindDlg()
{
}

void CAnimation2DBindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_PARENTANINAME, m_strParentAniFileName);
	DDX_Text(pDX, IDC_STATIC_CHILDANINAME, m_strChildAniFileName);
	DDX_Control(pDX, IDC_LIST_PARENTANI, m_ParentAniList);
	DDX_Control(pDX, IDC_LIST_CHILDANI, m_ChildAniList);
	DDX_Text(pDX, IDC_EDIT_XPOS, m_strRelativeX);
	DDX_Text(pDX, IDC_EDIT_YPOS, m_strRelativeY);
}

void CAnimation2DBindDlg::ReadParentAniFile(const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "rt");
	if (!pFile)
	{
		return;
	}

	//texture파일 이름 읽기
	//---------------------------------------
	char strTextureFileName[MAX_PATH] = {};
	fscanf_s(pFile, "%s\n", strTextureFileName,MAX_PATH);

	m_strParentAniFileName = strTextureFileName;
	UpdateData(FALSE);
	//---------------------------------------

	//texture 로드
	//---------------------------------------
	const char * pTexturePath = GET_SINGLE(CPathManager)->FindPathMultibyte(TEXTURE_PATH);
	if (!pTexturePath)
		return;
	
	char strTextureFullPath[MAX_PATH] = {};
	strcpy_s(strTextureFullPath, pTexturePath);

	strcat_s(strTextureFullPath, strTextureFileName);
	
	TCHAR ConvertName[MAX_PATH] = {};
	MultiByteToWideChar(CP_ACP, 0, strTextureFullPath, -1, ConvertName, sizeof(TCHAR) * MAX_PATH);

	m_ParentImage.Load(ConvertName);
	//---------------------------------------


	//애니메이션 이름
	//---------------------------------------
	char temp[MAX_PATH] = {};
	fscanf_s(pFile, "%s\n", temp, MAX_PATH);
	//---------------------------------------


	//Frame 정보 Setting
	//---------------------------------------
	int iSize = 0;
	fscanf_s(pFile, "%d\n", &iSize);

	//Cut정보
	//-------------------------------------
	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		RECT tCurInfo;
		fscanf_s(pFile, "%d %d %d %d\n", &tCurInfo.left, &tCurInfo.top,
			&tCurInfo.right, &tCurInfo.bottom);

		m_vecParentCutRect.push_back(tCurInfo);
	}
	//-------------------------------------

	//Pivot정보
	//-------------------------------------
	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		Vector2 vPivot;
		fscanf_s(pFile, "%f %f\n", &vPivot.x, &vPivot.y);

		m_vecParentPivot.push_back(vPivot);
	}
	//-------------------------------------

	//---------------------------------------

	fscanf_s(pFile, "%f\n", &m_fPlayRate);
	fscanf_s(pFile, "%f\n", &m_fPlayTime);

	if (!m_ChildImage.IsNull())
	{
		iSize *= (int)m_vecChildCutRect.size();
		for (int iCnt = 0; iCnt < iSize; ++iCnt)
		{
			TCHAR tBuf[20] = {};
			_itow_s(iCnt, tBuf, 10);
			m_ParentAniList.InsertItem(iCnt, tBuf);

			m_vecConnectPos.push_back(Vector2(0.f, 0.f));
			m_ParentAniList.SetItem(iCnt, 1, LVIF_TEXT, ::to_wstring(0).c_str(), 0, 0, 0, NULL);
			m_ParentAniList.SetItem(iCnt, 2, LVIF_TEXT, ::to_wstring(0).c_str(), 0, 0, 0, NULL);
		}
	}

	fclose(pFile);
}
void CAnimation2DBindDlg::ReadChildAniFile(const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "rt");
	if (!pFile)
	{
		return;
	}

	//texture파일 이름 읽기
	//---------------------------------------
	char strTextureFileName[MAX_PATH] = {};
	fscanf_s(pFile, "%s\n", strTextureFileName, MAX_PATH);

	m_strChildAniFileName = strTextureFileName;
	UpdateData(FALSE);
	//---------------------------------------

	//texture 로드
	//---------------------------------------
	const char* pTexturePath = GET_SINGLE(CPathManager)->FindPathMultibyte(TEXTURE_PATH);
	if (!pTexturePath)
		return;

	char strTextureFullPath[MAX_PATH] = {};
	strcpy_s(strTextureFullPath, pTexturePath);

	strcat_s(strTextureFullPath, strTextureFileName);

	TCHAR ConvertName[MAX_PATH] = {};
	MultiByteToWideChar(CP_ACP, 0, strTextureFullPath, -1, ConvertName, sizeof(TCHAR) * MAX_PATH);

	m_ChildImage.Load(ConvertName);
	//---------------------------------------


	//애니메이션 이름
	//---------------------------------------
	fscanf_s(pFile, "%s\n", m_strChildAnimName, MAX_PATH);
	//---------------------------------------


	//Frame 정보 Setting
	//---------------------------------------
	int iSize = 0;
	fscanf_s(pFile, "%d\n", &iSize);

	//Cut정보
	//-------------------------------------
	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		RECT tCurInfo;
		fscanf_s(pFile, "%d %d %d %d\n", &tCurInfo.left, &tCurInfo.top,
			&tCurInfo.right, &tCurInfo.bottom);

		m_vecChildCutRect.push_back(tCurInfo);

		TCHAR tBuf[20] = {};
		_itow_s(iCnt, tBuf, 10);
		m_ChildAniList.InsertItem(iCnt, tBuf);

		m_ChildAniList.SetItem(iCnt, 1, LVIF_TEXT, ::to_wstring(tCurInfo.left).c_str(), 0, 0, 0, NULL);
		m_ChildAniList.SetItem(iCnt, 2, LVIF_TEXT, ::to_wstring(tCurInfo.top).c_str(), 0, 0, 0, NULL);
		m_ChildAniList.SetItem(iCnt, 3, LVIF_TEXT, ::to_wstring(tCurInfo.right).c_str(), 0, 0, 0, NULL);
		m_ChildAniList.SetItem(iCnt, 4, LVIF_TEXT, ::to_wstring(tCurInfo.bottom).c_str(), 0, 0, 0, NULL);
	}
	//-------------------------------------


	//Pivot정보
	//-------------------------------------
	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		Vector2 vPivot;
		fscanf_s(pFile, "%f %f\n", &vPivot.x, &vPivot.y);

		m_vecChildPivot.push_back(vPivot);
	}
	//-------------------------------------

	//---------------------------------------

	if (!m_ParentImage.IsNull())
	{
		iSize *= (int)m_vecParentCutRect.size();
		for (int iCnt = 0; iCnt < iSize; ++iCnt)
		{
			TCHAR tBuf[20] = {};
			_itow_s(iCnt, tBuf, 10);
			m_ParentAniList.InsertItem(iCnt, tBuf);

			m_vecConnectPos.push_back(Vector2(0.f, 0.f));
			m_ParentAniList.SetItem(iCnt, 1, LVIF_TEXT, ::to_wstring(0).c_str(), 0, 0, 0, NULL);
			m_ParentAniList.SetItem(iCnt, 2, LVIF_TEXT, ::to_wstring(0).c_str(), 0, 0, 0, NULL);
		}
	}


	fclose(pFile);
}

void CAnimation2DBindDlg::InitParentAniInfoList()
{
	m_ParentAniList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	LVCOLUMN LvCol;
	LvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	LvCol.fmt = LVCFMT_CENTER;
	m_ParentAniList.SetColumn(0, &LvCol);
	m_ParentAniList.SetColumn(1, &LvCol);
	m_ParentAniList.SetColumn(2, &LvCol);

	m_ParentAniList.InsertColumn(0, TEXT("순서"), LVCFMT_CENTER, 40);
	m_ParentAniList.InsertColumn(1, TEXT("X"), LVCFMT_CENTER, 60);
	m_ParentAniList.InsertColumn(2, TEXT("Y"), LVCFMT_CENTER, 60);
}

void CAnimation2DBindDlg::InitChildAniInfoList()
{
	m_ChildAniList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	LVCOLUMN LvCol;
	LvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	LvCol.fmt = LVCFMT_CENTER;
	m_ChildAniList.SetColumn(0, &LvCol);
	m_ChildAniList.SetColumn(1, &LvCol);
	m_ChildAniList.SetColumn(2, &LvCol);
	m_ChildAniList.SetColumn(3, &LvCol);
	m_ChildAniList.SetColumn(4, &LvCol);

	m_ChildAniList.InsertColumn(0, TEXT("순서"), LVCFMT_CENTER, 40);
	m_ChildAniList.InsertColumn(1, TEXT("Left"), LVCFMT_CENTER, 60);
	m_ChildAniList.InsertColumn(2, TEXT("Top"), LVCFMT_CENTER, 60);
	m_ChildAniList.InsertColumn(3, TEXT("Right"), LVCFMT_CENTER, 60);
	m_ChildAniList.InsertColumn(4, TEXT("Bottom"), LVCFMT_CENTER, 60);
}


BEGIN_MESSAGE_MAP(CAnimation2DBindDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PARENTANILOAD, &CAnimation2DBindDlg::OnBnClickedButtonParentaniload)
	ON_BN_CLICKED(IDC_BUTTON_CHILDANILOAD, &CAnimation2DBindDlg::OnBnClickedButtonChildaniload)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CHILDANI, &CAnimation2DBindDlg::OnLvnItemchangedListChildani)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PARENTANI, &CAnimation2DBindDlg::OnLvnItemchangedListParentani)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMIN, &CAnimation2DBindDlg::OnBnClickedButtonZoomin)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMOUT, &CAnimation2DBindDlg::OnBnClickedButtonZoomout)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, &CAnimation2DBindDlg::OnBnClickedButtonInsert)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_ALLINSERT, &CAnimation2DBindDlg::OnBnClickedButtonAllinsert)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CAnimation2DBindDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CAnimation2DBindDlg 메시지 처리기


BOOL CAnimation2DBindDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Animation Player View 생성
	m_pAnimPlayerView = new CAnimationBinderPlayerView;

	m_pAnimPlayerView->Create(NULL, L"", WS_CHILD | WS_BORDER | WS_VISIBLE, CRect(10, 10, 460, 460), this, 1214);

	m_pAnimPlayerView->SetViewWidth(450);
	m_pAnimPlayerView->SetViewHeight(450);
	m_pAnimPlayerView->OnInitialUpdate();


	//List 초기화
	//----------------------------
	InitParentAniInfoList();
	InitChildAniInfoList();
	//----------------------------

	return TRUE;
}


void CAnimation2DBindDlg::OnBnClickedButtonParentaniload()
{
	static TCHAR	szFilter[] = TEXT("ani 파일(*.ani)|*.ani;*.ANI||");
	CFileDialog filedlg(TRUE, TEXT(""), TEXT(""), OFN_OVERWRITEPROMPT, szFilter);
	filedlg.m_ofn.lpstrInitialDir = GET_SINGLE(CPathManager)->FindPath(ANI_PATH);

	if (filedlg.DoModal() == IDOK)
	{
		m_iCurSelectConnectIdx = 0;

		//전에 로드한 이미지가 있는 경우 Clear
		if (!m_ParentImage.IsNull())
		{
			m_ParentImage.Destroy();
			m_ParentAniList.DeleteAllItems();
			m_vecParentCutRect.clear();
			m_iCurSelectParentImageIdx = 0;
			m_vecConnectPos.clear();
			m_strRelativeX = CString("0");
			m_strRelativeY = CString("0");
			UpdateData(FALSE);
		}

		memset(m_strParentFullPath, 0, MAX_PATH);

		CString PathName = filedlg.GetPathName().GetBuffer();

		strcpy_s(m_strParentFullPath, CT2A(PathName));

		char strFullPath[MAX_PATH] = {};

		strcpy_s(strFullPath, CT2A(PathName));

		ReadParentAniFile(strFullPath);

		if (!m_ChildImage.IsNull())
		{
			m_pAnimPlayerView->DrawImageAndAxis(m_ParentImage, m_vecParentCutRect[0], m_vecParentPivot[0],
				m_ChildImage, m_vecChildCutRect[0], m_vecChildPivot[0], m_vecConnectPos[0]);
		}
		else
		{
			m_pAnimPlayerView->DrawImageAndAxis(m_ParentImage, m_vecParentCutRect[0], m_vecParentPivot[0],
				CImage(), RECT(), Vector2(),Vector2());
		}
	}
	//------------------------------------------
}


void CAnimation2DBindDlg::OnBnClickedButtonChildaniload()
{
	static TCHAR	szFilter[] = TEXT("ani 파일(*.ani)|*.ani;*.ANI||");
	CFileDialog filedlg(TRUE, TEXT(""), TEXT(""), OFN_OVERWRITEPROMPT, szFilter);
	filedlg.m_ofn.lpstrInitialDir = GET_SINGLE(CPathManager)->FindPath(ANI_PATH);

	if (filedlg.DoModal() == IDOK)
	{
		m_iCurSelectConnectIdx = 0;
		 
		//전에 로드한 이미지가 있는 경우 Clear
		if (!m_ChildImage.IsNull())
		{
			m_ChildImage.Destroy();
			m_ChildAniList.DeleteAllItems();
			m_vecChildCutRect.clear();
			m_vecChildPivot.clear();
			m_iCurSelectChildImageIdx = 0;
			m_ParentAniList.DeleteAllItems();
			m_vecConnectPos.clear();
		}

		memset(m_strChildAnimName, 0, MAX_PATH);

		CString PathName = filedlg.GetPathName().GetBuffer();

		char strFullPath[MAX_PATH] = {};

		strcpy_s(strFullPath, CT2A(PathName));

		ReadChildAniFile(strFullPath);

		if (!m_ParentImage.IsNull())
		{
			m_pAnimPlayerView->DrawImageAndAxis(m_ParentImage, m_vecParentCutRect[m_iCurSelectParentImageIdx], m_vecParentPivot[m_iCurSelectParentImageIdx],
				m_ChildImage, m_vecChildCutRect[0], m_vecChildPivot[0], m_vecConnectPos[m_iCurSelectConnectIdx] * (float)m_pAnimPlayerView->GetScale());
		}
		else
		{
			m_pAnimPlayerView->DrawImageAndAxis(CImage(), RECT(), Vector2(),
				m_ChildImage, m_vecChildCutRect[0], m_vecChildPivot[0],Vector2());
		}
	}
	//------------------------------------------
}

void CAnimation2DBindDlg::OnLvnItemchangedListChildani(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	m_iCurSelectChildImageIdx = pNMLV->iItem;

	if (!m_ParentImage.IsNull())
	{
		m_pAnimPlayerView->DrawImageAndAxis(m_ParentImage, m_vecParentCutRect[m_iCurSelectParentImageIdx], m_vecParentPivot[m_iCurSelectParentImageIdx],
			m_ChildImage, m_vecChildCutRect[m_iCurSelectChildImageIdx], m_vecChildPivot[m_iCurSelectChildImageIdx], m_vecConnectPos[m_iCurSelectConnectIdx] * (float)m_pAnimPlayerView->GetScale());
	}
	else
	{
		m_pAnimPlayerView->DrawImageAndAxis(CImage(), RECT(), Vector2(),
			m_ChildImage, m_vecChildCutRect[m_iCurSelectChildImageIdx], m_vecChildPivot[m_iCurSelectChildImageIdx], Vector2());
	}
	*pResult = 0;
}


void CAnimation2DBindDlg::OnLvnItemchangedListParentani(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	m_iCurSelectConnectIdx = pNMLV->iItem;
	m_iCurSelectParentImageIdx = m_iCurSelectConnectIdx / (int)m_vecChildCutRect.size();
	m_iCurSelectChildImageIdx = m_iCurSelectConnectIdx  % (int)m_vecChildCutRect.size();


	if (!m_ChildImage.IsNull())
	{
		m_pAnimPlayerView->DrawImageAndAxis(m_ParentImage, m_vecParentCutRect[m_iCurSelectParentImageIdx], m_vecParentPivot[m_iCurSelectParentImageIdx],
			m_ChildImage, m_vecChildCutRect[m_iCurSelectChildImageIdx], m_vecChildPivot[m_iCurSelectChildImageIdx],m_vecConnectPos[m_iCurSelectConnectIdx] * (float)m_pAnimPlayerView->GetScale());
	}
	else
	{
		m_pAnimPlayerView->DrawImageAndAxis(m_ParentImage, m_vecParentCutRect[m_iCurSelectParentImageIdx], m_vecParentPivot[m_iCurSelectParentImageIdx],
			CImage(), RECT(), Vector2(),Vector2());
	}

	m_strRelativeX = m_ParentAniList.GetItemText(m_iCurSelectParentImageIdx, 1);
	m_strRelativeY = m_ParentAniList.GetItemText(m_iCurSelectParentImageIdx, 2);

	UpdateData(FALSE);

	*pResult = 0;
}


BOOL CAnimation2DBindDlg::PreTranslateMessage(MSG* pMsg)
{
	int iDiffX = 0;
	int iDiffY = 0;
	if (pMsg->message == WM_KEYDOWN)
	{
		//이미지가 없으면 pass
		if (m_ParentImage.IsNull() || m_ChildImage.IsNull())
			return CDialogEx::PreTranslateMessage(pMsg);

		switch (pMsg->wParam)
		{
		case VK_LEFT:
			--iDiffX;
			break;
		case VK_RIGHT:
			++iDiffX;
			break;
		case VK_UP:
			--iDiffY;
			break;
		case VK_DOWN:
			++iDiffY;
			break;
		}

		m_vecConnectPos[m_iCurSelectConnectIdx].x += iDiffX;
		m_vecConnectPos[m_iCurSelectConnectIdx].y += iDiffY;

		m_pAnimPlayerView->DrawImageAndAxis(m_ParentImage, m_vecParentCutRect[m_iCurSelectParentImageIdx], m_vecParentPivot[m_iCurSelectParentImageIdx],
			m_ChildImage, m_vecChildCutRect[m_iCurSelectChildImageIdx], m_vecChildPivot[m_iCurSelectChildImageIdx], m_vecConnectPos[m_iCurSelectConnectIdx] * (float)m_pAnimPlayerView->GetScale());

		m_strRelativeX = CA2T(to_string(m_vecConnectPos[m_iCurSelectConnectIdx].x).c_str());
		m_strRelativeY = CA2T(to_string(m_vecConnectPos[m_iCurSelectConnectIdx].y).c_str());

		UpdateData(FALSE);

		return TRUE;
	}
	//-----------------------------

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CAnimation2DBindDlg::OnBnClickedButtonZoomin()
{
	if (m_ParentImage.IsNull() || m_ChildImage.IsNull())
		return;

	m_pAnimPlayerView->IncreaseScale();

	m_pAnimPlayerView->DrawImageAndAxis(m_ParentImage, m_vecParentCutRect[m_iCurSelectParentImageIdx], m_vecParentPivot[m_iCurSelectParentImageIdx],
		m_ChildImage, m_vecChildCutRect[m_iCurSelectChildImageIdx], m_vecChildPivot[m_iCurSelectChildImageIdx], m_vecConnectPos[m_iCurSelectConnectIdx] * (float)m_pAnimPlayerView->GetScale());
}


void CAnimation2DBindDlg::OnBnClickedButtonZoomout()
{
	if (m_ParentImage.IsNull() || m_ChildImage.IsNull())
		return;

	m_pAnimPlayerView->DecreaseScale();

	m_pAnimPlayerView->DrawImageAndAxis(m_ParentImage, m_vecParentCutRect[m_iCurSelectParentImageIdx], m_vecParentPivot[m_iCurSelectParentImageIdx],
		m_ChildImage, m_vecChildCutRect[m_iCurSelectChildImageIdx], m_vecChildPivot[m_iCurSelectChildImageIdx], m_vecConnectPos[m_iCurSelectConnectIdx] * (float)m_pAnimPlayerView->GetScale());
}


void CAnimation2DBindDlg::OnBnClickedButtonInsert()
{
	if (m_ChildImage.IsNull())
		return;
	
	m_ParentAniList.SetItemText(m_iCurSelectConnectIdx, 1, m_strRelativeX);
	m_ParentAniList.SetItemText(m_iCurSelectConnectIdx, 2, m_strRelativeY);

	m_vecConnectPos[m_iCurSelectConnectIdx] = Vector2((float)_ttoi(m_strRelativeX), (float)_ttoi(m_strRelativeY));
}


void CAnimation2DBindDlg::OnClose()
{
	m_ParentImage.Destroy();
	m_ChildImage.Destroy();

	m_vecParentCutRect.clear();
	m_vecParentPivot.clear();
	m_vecChildCutRect.clear();
	m_vecChildPivot.clear();
	m_vecConnectPos.clear();

	m_iCurSelectChildImageIdx = 0;
	m_iCurSelectParentImageIdx = 0;

	m_strParentAniFileName = "";
	m_strChildAniFileName = "";

	m_ParentAniList.DeleteAllItems();
	m_ChildAniList.DeleteAllItems();

	m_strRelativeX = "";
	m_strRelativeY = "";

	UpdateData(FALSE);

	CDialogEx::OnClose();
}


void CAnimation2DBindDlg::OnBnClickedButtonAllinsert()
{
	if (m_ChildImage.IsNull())
		return;

	int iSize = (int)m_vecConnectPos.size();
	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		m_ParentAniList.SetItemText(iCnt, 1, m_strRelativeX);
		m_ParentAniList.SetItemText(iCnt, 2, m_strRelativeY);

		m_vecConnectPos[iCnt] = Vector2((float)_ttoi(m_strRelativeX), (float)_ttoi(m_strRelativeY));
	}

}


void CAnimation2DBindDlg::OnBnClickedButtonSave()
{
	//file open
	//----------------------------------
	FILE* pFile = nullptr;
	fopen_s(&pFile, m_strParentFullPath, "at+");
	if (!pFile)
		return;
	//----------------------------------

	int a = 1;
	//자식 애니메이션 이름 저장
	//----------------------------------

	fprintf(pFile, "%s\n", m_strChildAnimName);
	//----------------------------------

	//Connect정보 저장
	//----------------------------------
	int iWidth = (int)m_vecChildCutRect.size();
	int iHeight = (int)m_vecParentCutRect.size();

	fprintf_s(pFile, "%d %d\n", iWidth, iHeight);

	int iSize = (int)m_vecConnectPos.size();

	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		fprintf_s(pFile, "%d %d\n", (int)m_vecConnectPos[iCnt].x, (int)m_vecConnectPos[iCnt].y * -1);
	}
	//----------------------------------

	fclose(pFile);

	MessageBox(TEXT("저장 성공"));
}
