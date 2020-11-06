// CAnimation2DEditorDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "Animation2DEditorDlg.h"
#include "afxdialogex.h"
#include "AnimationEditorView.h"
#include "AnimationPlayerView.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"

// CAnimation2DEditorDlg 대화 상자

IMPLEMENT_DYNAMIC(CAnimation2DEditorDlg, CDialogEx)

CAnimation2DEditorDlg::CAnimation2DEditorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ANIM2DEDITOR, pParent)
	, m_fPivotX(0)
	, m_fPivotY(0)
	, m_fPlayRate(0)
	, m_fPlayTime(0)
	,m_iCurSelectImageIdx(-1)
{

}

CAnimation2DEditorDlg::~CAnimation2DEditorDlg()
{
}

void CAnimation2DEditorDlg::UpdateColorKey(const COLORREF & ColorKey)
{
	TCHAR Buf[10] = {};
	_itow_s(GetRValue(ColorKey), Buf, 10);
	m_RColorKey.SetWindowTextW(Buf);
	memset(Buf, 0, sizeof(TCHAR) * 10);

	_itow_s(GetGValue(ColorKey), Buf, 10);
	m_GColorKey.SetWindowTextW(Buf);
	memset(Buf, 0, sizeof(TCHAR) * 10);

	_itow_s(GetBValue(ColorKey), Buf, 10);
	m_BColorKey.SetWindowTextW(Buf);
}

void CAnimation2DEditorDlg::UpdateCutInfo(const POINT& tStart, const POINT& tEnd)
{
	TCHAR Buf[10] = {};
	_itow_s(tStart.x, Buf, 10);
	m_CutLeft.SetWindowTextW(Buf);
	memset(Buf, 0, sizeof(TCHAR) * 10);

	_itow_s(tStart.y, Buf, 10);
	m_CutTop.SetWindowTextW(Buf);
	memset(Buf, 0, sizeof(TCHAR) * 10);

	_itow_s(tEnd.x, Buf, 10);
	m_CutRight.SetWindowTextW(Buf);
	memset(Buf, 0, sizeof(TCHAR) * 10);

	_itow_s(tEnd.y, Buf, 10);
	m_CutBottom.SetWindowTextW(Buf);
	memset(Buf, 0, sizeof(TCHAR) * 10);
}

void CAnimation2DEditorDlg::InitCutInfoList()
{
	m_CutInfoList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	LVCOLUMN LvCol;
	LvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	LvCol.fmt = LVCFMT_CENTER;
	m_CutInfoList.SetColumn(0, &LvCol);
	m_CutInfoList.SetColumn(1, &LvCol);
	m_CutInfoList.SetColumn(2, &LvCol);
	m_CutInfoList.SetColumn(3, &LvCol);
	m_CutInfoList.SetColumn(4, &LvCol);

	m_CutInfoList.InsertColumn(0, TEXT("순서"),LVCFMT_CENTER,40);
	m_CutInfoList.InsertColumn(1, TEXT("Left"), LVCFMT_CENTER, 60);
	m_CutInfoList.InsertColumn(2, TEXT("Top"), LVCFMT_CENTER, 60);
	m_CutInfoList.InsertColumn(3, TEXT("Right"), LVCFMT_CENTER, 60);
	m_CutInfoList.InsertColumn(4, TEXT("Bottom"), LVCFMT_CENTER, 60);
}

void CAnimation2DEditorDlg::InitPivotInfoList()
{
	m_PivotInfoList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	LVCOLUMN LvCol;
	LvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	LvCol.fmt = LVCFMT_CENTER;
	m_PivotInfoList.SetColumn(0, &LvCol);
	m_PivotInfoList.SetColumn(1, &LvCol);
	m_PivotInfoList.SetColumn(2, &LvCol);

	m_PivotInfoList.InsertColumn(0, TEXT("순서"), LVCFMT_CENTER, 40);
	m_PivotInfoList.InsertColumn(1, TEXT("X"), LVCFMT_CENTER, 60);
	m_PivotInfoList.InsertColumn(2, TEXT("Y"), LVCFMT_CENTER, 60);
}

void CAnimation2DEditorDlg::SaveAniFile(const char * pFileName, const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "wt");

	if (!pFile)
		return;

	//저장 Format
	//-------------------
	//텍스처 이름
	//애니메이션이름
	//CutImage 수
	//Cut정보
	//PIvot정보
	//재생속도
	//재생시간
	//Loop 여부
	//-------------------

	//텍스처 파일 이름 저장
	fprintf_s(pFile, "%s\n", m_strTextureFileName);

	//애니메이션 이름 저장
	char AnimationName[64] = {};
	strcpy_s(AnimationName, CT2A(m_strAnimName.GetBuffer()));
	fprintf_s(pFile, "%s\n", AnimationName);

	//Cut,Pivot정보 저장
	m_pAnimEditView->SaveInfo(pFile);

	//재생속도 저장
	fprintf_s(pFile, "%f\n", m_fPlayRate);

	//재생시간 저장
	fprintf_s(pFile, "%f\n", m_fPlayTime);

	//Loop여부 저장
	if(m_Loop.GetCheck())
		fprintf_s(pFile, "%d\n", true);
	else
		fprintf_s(pFile, "%d\n", false);

	fclose(pFile);
}

void CAnimation2DEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FILENAME, m_FileNameStatic);
	DDX_Control(pDX, IDC_STATIC_IMAGE_WIDTH, m_ImageWidthStatic);
	DDX_Control(pDX, IDC_STATIC_IMAGE_HEIGHT, m_ImageHeightStatic);
	DDX_Control(pDX, IDC_STATIC_RCOLORKEY, m_RColorKey);
	DDX_Control(pDX, IDC_STATIC_GCOLORKEY, m_GColorKey);
	DDX_Control(pDX, IDC_STATIC_BCOLORKEY, m_BColorKey);
	DDX_Control(pDX, IDC_STATIC_CUTLEFT, m_CutLeft);
	DDX_Control(pDX, IDC_STATIC_CUTTOP, m_CutTop);
	DDX_Control(pDX, IDC_STATIC_CUTRIGHT, m_CutRight);
	DDX_Control(pDX, IDC_STATIC_CUTBOTTOM, m_CutBottom);
	DDX_Control(pDX, IDC_LIST_CUTINFO, m_CutInfoList);
	DDX_Text(pDX, IDC_EDIT_PIVOTX, m_fPivotX);
	DDX_Text(pDX, IDC_EDIT_PIVOTY, m_fPivotY);
	DDX_Control(pDX, IDC_LIST_PIVOTINFO, m_PivotInfoList);
	DDX_Text(pDX, IDC_EDIT_PLAYRATE, m_fPlayRate);
	DDX_Text(pDX, IDC_EDIT_PLAYTIME, m_fPlayTime);
	DDX_Text(pDX, IDC_EDIT_ANIMNAME, m_strAnimName);
	DDX_Control(pDX, IDC_RADIO_LOOP, m_Loop);
	DDX_Control(pDX, IDC_RADIO_STOP, m_Stop);
}


BEGIN_MESSAGE_MAP(CAnimation2DEditorDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CAnimation2DEditorDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CAnimation2DEditorDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_COLORKEY, &CAnimation2DEditorDlg::OnBnClickedButtonColorkey)
	ON_BN_CLICKED(IDC_BUTTON_CUTSAVE, &CAnimation2DEditorDlg::OnBnClickedButtonCutsave)
	ON_BN_CLICKED(IDC_BUTTON_CUTLISTCLEAR, &CAnimation2DEditorDlg::OnBnClickedButtonCutlistclear)
	ON_BN_CLICKED(IDC_BUTTON_PRINTCUTINFO, &CAnimation2DEditorDlg::OnBnClickedButtonPrintcutinfo)
	ON_BN_CLICKED(IDC_BUTTON_PIVOTALLSAVE, &CAnimation2DEditorDlg::OnBnClickedButtonPivotallsave)
	ON_BN_CLICKED(IDC_BUTTON_PIVOTSAVE, &CAnimation2DEditorDlg::OnBnClickedButtonPivotsave)
	ON_BN_CLICKED(IDC_BUTTON_PIVOTLISTCLEAR, &CAnimation2DEditorDlg::OnBnClickedButtonPivotlistclear)
	ON_BN_CLICKED(IDC_BUTTON_PRINTPIVOTINFO, &CAnimation2DEditorDlg::OnBnClickedButtonPrintpivotinfo)
	ON_BN_CLICKED(IDC_BUTTON_ANIMPLAY, &CAnimation2DEditorDlg::OnBnClickedButtonAnimplay)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_ANIMSTOP, &CAnimation2DEditorDlg::OnBnClickedButtonAnimstop)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PIVOTINFO, &CAnimation2DEditorDlg::OnNMClickListPivotinfo)

END_MESSAGE_MAP()


// CAnimation2DEditorDlg 메시지 처리기


BOOL CAnimation2DEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Editor View 생성
	//------------------------------------------
	m_pAnimEditView = new CAnimationEditorView;
	
	m_pAnimEditView->Create(NULL, L"", WS_CHILD | WS_BORDER | WS_VISIBLE, CRect(0, 0, 800, 750), this, 1212);

	m_pAnimEditView->OnInitialUpdate();

	m_pAnimEditView->SetViewWidth(800);
	m_pAnimEditView->SetViewHeight(750);
	//------------------------------------------

	//Player View 생성
	//------------------------------------------
	m_pAnimPlayerView = new CAnimationPlayerView;

	m_pAnimPlayerView->Create(NULL, L"", WS_CHILD | WS_BORDER | WS_VISIBLE, CRect(815, 450,1250,750), this, 1213);

	m_pAnimPlayerView->OnInitialUpdate();

	m_pAnimPlayerView->SetViewWidth(1250 - 815);
	m_pAnimPlayerView->SetViewHeight(750 - 450);
	//------------------------------------------

	//ListControl초기화
	//------------------------------------------
	InitCutInfoList();
	InitPivotInfoList();
	//------------------------------------------

	SendDlgItemMessage(IDC_BUTTON_LOAD, WM_KILLFOCUS, 0, 0);

	return TRUE;
}


void CAnimation2DEditorDlg::OnBnClickedButtonLoad()
{
	memset(m_strTextureFileName, 0, MAX_PATH);

	//파일 다이얼로그 열기
	//------------------------------------------
	static TCHAR	szFilter[] = TEXT("이미지 파일(*.BMP, *.JPG, *.PNG, *.DDS)|*.BMP;*.JPG;*.PNG;*.DDS;*.bmp;*.jpg;*.png;*.dds|모든파일(*.*)|*.*||");
	CFileDialog filedlg(TRUE, TEXT(""), TEXT(""), OFN_OVERWRITEPROMPT, szFilter);
	filedlg.m_ofn.lpstrInitialDir = GET_SINGLE(CPathManager)->FindPath(TEXTURE_PATH);

	if (filedlg.DoModal() == IDOK)
	{
		CString strPathName = filedlg.GetPathName().GetBuffer();
		CString strFileName = filedlg.GetFileName().GetBuffer();

		strcpy_s(m_strTextureFileName, CT2A(strFileName));

		//Texture Load
		//---------------------------------------
		if (!m_pAnimEditView->LoadTexture(strPathName.GetBuffer()))
		{
			return;
		}
		//---------------------------------------
		

		//ListControl Claer
		m_CutInfoList.DeleteAllItems();

		SetDlgItemTextW(IDC_STATIC_FILENAME, filedlg.GetFileName());
		SetDlgItemInt(IDC_STATIC_IMAGE_WIDTH,(UINT)m_pAnimEditView->GetWidth());
		SetDlgItemInt(IDC_STATIC_IMAGE_HEIGHT, (UINT)m_pAnimEditView->GetHeight());

		m_pAnimEditView->RenderImage();
	}
	//------------------------------------------
}


void CAnimation2DEditorDlg::OnBnClickedButtonSave()
{
	UpdateData(TRUE);
	
	//이름이 비어있는 경우 
	//------------------------------------------
	if (m_strAnimName.IsEmpty())
	{
		MessageBox(TEXT("애니메이션 이름을 입력하세요"));
		return;
	}
	//------------------------------------------

	static TCHAR	szFilter[] = TEXT("ani 파일(*.ani)|*.ani;*.ANI||");
	CFileDialog filedlg(FALSE, TEXT(""), TEXT(""), OFN_OVERWRITEPROMPT, szFilter);
	filedlg.m_ofn.lpstrInitialDir = GET_SINGLE(CPathManager)->FindPath(ANI_PATH);

	if (filedlg.DoModal() == IDOK)
	{
		CString FileName = filedlg.GetFileName().GetBuffer();
		CString PathName = filedlg.GetPathName().GetBuffer();

		char strFileName[MAX_PATH] = {};
		char strFullPath[MAX_PATH] = {};

		strcpy_s(strFileName, CT2A(FileName));
		strcpy_s(strFullPath, CT2A(PathName));
		
		SaveAniFile(strFileName,strFullPath);

		GET_SINGLE(CSceneManager)->AddSequenceResource(strFileName);
	}
	//------------------------------------------
}


void CAnimation2DEditorDlg::OnBnClickedButtonColorkey()
{
	m_pAnimEditView->SetColorKeySelect();
}


void CAnimation2DEditorDlg::OnBnClickedButtonCutsave()
{
	//현재 Cut정보 List에 저장
	//---------------------------------
	int iSize = m_CutInfoList.GetItemCount();
	
	TCHAR tBuf[20] = {};
	_itow_s(iSize, tBuf, 10);
	m_CutInfoList.InsertItem(iSize, tBuf);

	CString str;
	m_CutLeft.GetWindowTextW(str);
	m_CutInfoList.SetItem(iSize, 1,LVIF_TEXT, T2W(str.GetBuffer()), 0, 0, 0, NULL);

	m_CutTop.GetWindowTextW(str);
	m_CutInfoList.SetItem(iSize, 2, LVIF_TEXT, T2W(str.GetBuffer()), 0, 0, 0, NULL);

	m_CutRight.GetWindowTextW(str);
	m_CutInfoList.SetItem(iSize, 3, LVIF_TEXT, T2W(str.GetBuffer()), 0, 0, 0, NULL);

	m_CutBottom.GetWindowTextW(str);
	m_CutInfoList.SetItem(iSize, 4, LVIF_TEXT, T2W(str.GetBuffer()), 0, 0, 0, NULL);

	//---------------------------------

	m_pAnimEditView->SaveCurCutInfo();
}


void CAnimation2DEditorDlg::OnBnClickedButtonCutlistclear()
{
	m_pAnimPlayerView->StopAnimation();

	//List Control Clear
	m_CutInfoList.DeleteAllItems();

	//View에서 저장하던 데이터 삭제
	m_pAnimEditView->RemoveAllCutInfo();

	m_iCurSelectImageIdx = -1;
}


void CAnimation2DEditorDlg::OnBnClickedButtonPrintcutinfo()
{
	m_pAnimEditView->PrintAllCutInfo();
}



void CAnimation2DEditorDlg::OnBnClickedButtonPivotallsave()
{
	//현재 Pivot정보 List에 일관 저장
	//---------------------------------

	//CutInfo수만큼 저장
	//------------------------------------------
	int iSize = m_CutInfoList.GetItemCount();

	UpdateData(TRUE);
	wstring strPivotX = std::to_wstring(m_fPivotX).c_str();
	wstring strPivotY = std::to_wstring(m_fPivotY).c_str();

	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		TCHAR tBuf[20] = {};
		_itow_s(iCnt, tBuf, 10);
		m_PivotInfoList.InsertItem(iCnt, tBuf);

		m_PivotInfoList.SetItem(iCnt, 1, LVIF_TEXT, strPivotX.c_str(), 0, 0, 0, NULL);

		m_PivotInfoList.SetItem(iCnt, 2, LVIF_TEXT, strPivotY.c_str(), 0, 0, 0, NULL);

		m_pAnimEditView->SavePivotInfo(m_fPivotX, m_fPivotY);
	}

	//------------------------------------------

	//---------------------------------
}


void CAnimation2DEditorDlg::OnBnClickedButtonPivotsave()
{
	int iSize = m_PivotInfoList.GetItemCount();

	UpdateData(TRUE);
	wstring strPivotX = std::to_wstring(m_fPivotX).c_str();
	wstring strPivotY = std::to_wstring(m_fPivotY).c_str();

	TCHAR tBuf[20] = {};
	_itow_s(iSize, tBuf, 10);
	m_PivotInfoList.InsertItem(iSize, tBuf);

	m_PivotInfoList.SetItem(iSize, 1, LVIF_TEXT, strPivotX.c_str(), 0, 0, 0, NULL);

	m_PivotInfoList.SetItem(iSize, 2, LVIF_TEXT, strPivotY.c_str(), 0, 0, 0, NULL);


	m_pAnimEditView->SavePivotInfo(m_fPivotX, m_fPivotY);
}


void CAnimation2DEditorDlg::OnBnClickedButtonPivotlistclear()
{
	m_pAnimPlayerView->StopAnimation();

	m_PivotInfoList.DeleteAllItems();

	m_pAnimEditView->RemoveAllPivot();

	m_iCurSelectImageIdx = -1;
}


void CAnimation2DEditorDlg::OnBnClickedButtonPrintpivotinfo()
{
	m_pAnimEditView->PrintAllPivotInfo();
}


void CAnimation2DEditorDlg::OnBnClickedButtonAnimplay()
{
	UpdateData(true);

	if(m_fPlayRate || m_fPlayTime)
		m_pAnimPlayerView->SetPlayAnimationTimer();
}


void CAnimation2DEditorDlg::OnClose()
{
	m_CutInfoList.DeleteAllItems();
	m_PivotInfoList.DeleteAllItems();

	m_fPivotX = 0.f;
	m_fPivotY = 0.f;

	m_fPlayRate = 1.f;
	m_fPlayTime = 1.f;

	m_FileNameStatic.SetWindowTextW(TEXT(""));
	m_ImageWidthStatic.SetWindowTextW(TEXT(""));
	m_ImageHeightStatic.SetWindowTextW(TEXT(""));

	m_RColorKey.SetWindowTextW(TEXT("255"));
	m_GColorKey.SetWindowTextW(TEXT("255"));
	m_BColorKey.SetWindowTextW(TEXT("255"));
	m_CutLeft.SetWindowTextW(TEXT(""));
	m_CutTop.SetWindowTextW(TEXT(""));
	m_CutRight.SetWindowTextW(TEXT(""));
	m_CutBottom.SetWindowTextW(TEXT(""));
	
	m_pAnimEditView->Clear();
	m_pAnimPlayerView->Clear();
	
	m_iCurSelectImageIdx = -1;

	UpdateData(false);

	CDialogEx::OnClose();
}


void CAnimation2DEditorDlg::OnBnClickedButtonAnimstop()
{
	m_pAnimPlayerView->StopAnimation();
}

void CAnimation2DEditorDlg::OnNMClickListPivotinfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	//Pivot Listcontorl 클릭시 AnimPlayerView에 해당 이미지 출력
	//-------------------------------------------------------

	m_iCurSelectImageIdx= pNMItemActivate->iItem;

	m_pAnimPlayerView->DrawImageAndAxis(m_iCurSelectImageIdx);

	//-------------------------------------------------------

	*pResult = 0;
}

BOOL CAnimation2DEditorDlg::PreTranslateMessage(MSG* pMsg)
{
	//방향키 입력시 
	//-----------------------------
	int iDiffX = 0;
	int iDiffY = 0;
	if (pMsg->message == WM_KEYDOWN)
	{
		//이미지가 없으면 pass
		if (GetCutImageCnt() == 0 || m_iCurSelectImageIdx == -1)
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

		RECT tCurRect = m_pAnimEditView->GetCutInfo(m_iCurSelectImageIdx);
		Vector2 tCurPivot = m_pAnimEditView->GetPivotInfo(m_iCurSelectImageIdx);

		//이미지 중심 좌표
		//------------------------------------------------------
		int iCutWidthHalfSize = (tCurRect.right - tCurRect.left) / 2;
		int iCutHeightHalfSize = (tCurRect.bottom - tCurRect.top) / 2;
		//------------------------------------------------------

		//현재 피벗 좌표
		//------------------------------------------------------
		int iStartX = m_pAnimPlayerView->GetViewWidth() / 2;
		int iStartY = m_pAnimPlayerView->GetViewHeight() / 2;

		int iCutSizeX = tCurRect.right - tCurRect.left;
		int iCutSizeY = tCurRect.bottom - tCurRect.top;

		int iPivotX = (int)(iStartX - iCutSizeX * tCurPivot.x);
		int iPivotY = (int)(iStartY - iCutSizeY * tCurPivot.y);
		//------------------------------------------------------

		//입력 적용
		//------------------------------------------------------
		iPivotX += iDiffX;
		iPivotY += iDiffY;
		//------------------------------------------------------

		//새로운 PIvot Rate 계산
		//------------------------------------------------------
		float fNewPivotX = (iStartX - iPivotX) / (float)iCutSizeX;
		float fNewPivotY = (iStartY - iPivotY) / (float)iCutSizeY;
		//------------------------------------------------------

		//Control에 Setting
		//------------------------------------------------------
		wstring strPivotX = std::to_wstring(fNewPivotX);
		wstring strPivotY = std::to_wstring(fNewPivotY);
		m_PivotInfoList.SetItem(m_iCurSelectImageIdx, 1, LVIF_TEXT, strPivotX.c_str(), 0, 0, 0, NULL);
		m_PivotInfoList.SetItem(m_iCurSelectImageIdx, 2, LVIF_TEXT, strPivotY.c_str(), 0, 0, 0, NULL);
		//------------------------------------------------------

		//AnimEditView에서 값을 바꿔주기
		//------------------------------------------------------
		m_pAnimEditView->ChangePivotInfo(m_iCurSelectImageIdx, fNewPivotX, fNewPivotY);
		//------------------------------------------------------

		//바뀐 pivot으로 다시 그리기
		//------------------------------------------------------
		m_pAnimPlayerView->DrawImageAndAxis(m_iCurSelectImageIdx);
		//------------------------------------------------------

		return TRUE;
	}
	//-----------------------------

	return CDialogEx::PreTranslateMessage(pMsg);
}
