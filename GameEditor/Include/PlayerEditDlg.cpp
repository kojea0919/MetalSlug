// PlayerEditDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "PlayerEditDlg.h"
#include "afxdialogex.h"
#include "resource/ResourceManager.h"
#include "PathManager.h"

// CPlayerEditDlg 대화 상자

IMPLEMENT_DYNAMIC(CPlayerEditDlg, CDialogEx)

CPlayerEditDlg::CPlayerEditDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PLAYER_EDITOR, pParent),
	m_iUpperAnimSelIdx(-1),m_iLowerAnimSelIdx(-1),
	m_iRemoveUpperAnimIdx(-1),m_iRemoveLowerAnimIdx(-1)
{
	memset(m_strUpperFullPath, 0, MAX_PATH);
	memset(m_strLowerFullPath, 0, MAX_PATH);
}

CPlayerEditDlg::~CPlayerEditDlg()
{
}

void CPlayerEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ALLANIM, m_AllAnimNameList1);
	DDX_Control(pDX, IDC_LIST_ALLANIM2, m_AllAnimNameList2);
	DDX_Control(pDX, IDC_LIST_UPPERANIM, m_UpperAnimNameList);
	DDX_Control(pDX, IDC_LIST_LOWERANIM, m_LowerAnimNameList);
}


BEGIN_MESSAGE_MAP(CPlayerEditDlg, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST_ALLANIM, &CPlayerEditDlg::OnLbnSelchangeListAllanim)
	ON_LBN_SELCHANGE(IDC_LIST_ALLANIM2, &CPlayerEditDlg::OnLbnSelchangeListAllanim2)
	ON_BN_CLICKED(IDC_BUTTON_ADDUPPERANIM, &CPlayerEditDlg::OnBnClickedButtonAddupperanim)
	ON_BN_CLICKED(IDC_BUTTON_ADDLOWERANIM, &CPlayerEditDlg::OnBnClickedButtonAddloweranim)
	ON_BN_CLICKED(IDC_BUTTON_UPPEERSAVE, &CPlayerEditDlg::OnBnClickedButtonUppeersave)
	ON_BN_CLICKED(IDC_BUTTON_LOWERSAVE, &CPlayerEditDlg::OnBnClickedButtonLowersave)
	ON_BN_CLICKED(IDC_BUTTON_REMOVEUPPERANIM, &CPlayerEditDlg::OnBnClickedButtonRemoveupperanim)
	ON_BN_CLICKED(IDC_BUTTON_REMOVELOWERANIM, &CPlayerEditDlg::OnBnClickedButtonRemoveloweranim)
	ON_LBN_SELCHANGE(IDC_LIST_UPPERANIM, &CPlayerEditDlg::OnLbnSelchangeListUpperanim)
	ON_LBN_SELCHANGE(IDC_LIST_LOWERANIM, &CPlayerEditDlg::OnLbnSelchangeListLoweranim)
END_MESSAGE_MAP()


// CPlayerEditDlg 메시지 처리기


BOOL CPlayerEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//AllAnimNameList에 이름 현재 관리중인 애니메이션 이름 Setting
	//-----------------------------------------------------------------------
	auto iter = GET_SINGLE(CResourceManager)->m_mapAnim2D.begin();
	auto iterEnd = GET_SINGLE(CResourceManager)->m_mapAnim2D.end();

	for (; iter != iterEnd; ++iter)
	{
		TCHAR ConvertName[MAX_PATH] = {};
		MultiByteToWideChar(CP_ACP, 0, iter->first.c_str(), -1, ConvertName, sizeof(TCHAR) * MAX_PATH);
		m_AllAnimNameList1.AddString(ConvertName);
		m_AllAnimNameList2.AddString(ConvertName);
	}
	//-----------------------------------------------------------------------

	//현재 플레이어가 사용중인 애니메이션 이름 Load
	//-----------------------------------------------------------------------
	
	const char* pPath = GET_SINGLE(CPathManager)->FindPathMultibyte(ANI_PATH);
	if (!pPath)
	{
		return FALSE;
	}
	strcpy_s(m_strUpperFullPath, pPath);

	strcat_s(m_strUpperFullPath, df_PLAYER_UPPERANIM_FILENAME);
	
	strcpy_s(m_strLowerFullPath, pPath);

	strcat_s(m_strLowerFullPath, df_PLAYER_LOWERANIM_FILENAME);

	//Upper Load
	//-------------------------------------
	FILE* pFile = nullptr;
	fopen_s(&pFile, m_strUpperFullPath, "rt");
	if (!pFile)
		return FALSE;

	int iSize = 0;
	fscanf_s(pFile, "%d\n", &iSize);

	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		char strAnimName[MAX_PATH] = {};
		fscanf_s(pFile, "%s\n", strAnimName, MAX_PATH);

		WCHAR ConvertName[MAX_PATH] = {};
		MultiByteToWideChar(CP_ACP, 0, strAnimName, -1, ConvertName, sizeof(TCHAR) * MAX_PATH);

		m_UpperAnimNameList.AddString(ConvertName);
	}

	fclose(pFile);

	//-------------------------------------


	//Lower Load
	//-------------------------------------
	fopen_s(&pFile, m_strLowerFullPath, "rt");
	if (!pFile)
		return FALSE;

	iSize = 0;
	fscanf_s(pFile, "%d\n", &iSize);

	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		char strAnimName[MAX_PATH] = {};
		fscanf_s(pFile, "%s\n", strAnimName, MAX_PATH);

		WCHAR ConvertName[MAX_PATH] = {};
		MultiByteToWideChar(CP_ACP, 0, strAnimName, -1, ConvertName, sizeof(TCHAR) * MAX_PATH);

		m_LowerAnimNameList.AddString(ConvertName);
	}

	fclose(pFile);
	//-------------------------------------

	//-----------------------------------------------------------------------

	return TRUE;
}


void CPlayerEditDlg::OnLbnSelchangeListAllanim()
{
	m_iUpperAnimSelIdx = m_AllAnimNameList1.GetCurSel();
}


void CPlayerEditDlg::OnLbnSelchangeListAllanim2()
{
	m_iLowerAnimSelIdx = m_AllAnimNameList2.GetCurSel();
}


void CPlayerEditDlg::OnBnClickedButtonAddupperanim()
{
	//선택된게 없는 경우 pass
	//--------------------------
	if (m_iUpperAnimSelIdx == -1)
		return;
	//--------------------------

	//선택된 애니메이션 이름을 오른족 List에 추가
	//--------------------------
	WCHAR pAnimName[MAX_PATH] = {};
	m_AllAnimNameList1.GetText(m_iUpperAnimSelIdx, pAnimName);

	//해당 이름이 있으면 pass
	if (m_UpperAnimNameList.FindStringExact(-1, pAnimName) != LB_ERR)
	{
		return;
	}

	m_UpperAnimNameList.AddString(pAnimName);
	//--------------------------
}


void CPlayerEditDlg::OnBnClickedButtonAddloweranim()
{
	//선택된게 없는 경우 pass
	//--------------------------
	if (m_iLowerAnimSelIdx == -1)
		return;
	//--------------------------

	//선택된 애니메이션 이름을 오른족 List에 추가
	//--------------------------
	WCHAR pAnimName[MAX_PATH] = {};
	m_AllAnimNameList2.GetText(m_iLowerAnimSelIdx, pAnimName);

	//해당 이름이 있으면 pass
	if (m_LowerAnimNameList.FindStringExact(-1, pAnimName) != LB_ERR)
		return;

	m_LowerAnimNameList.AddString(pAnimName);
	//--------------------------
}


void CPlayerEditDlg::OnBnClickedButtonUppeersave()
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, m_strUpperFullPath, "wt");
	if (!pFile)
	{
		MessageBox(TEXT("저장 실패"));
		return;
	}

	//선택한 애니메이션 이름 저장
	//------------------------------------
	int iSize = m_UpperAnimNameList.GetCount();

	fprintf_s(pFile, "%d\n", iSize);
	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		CString CurStr;
		m_UpperAnimNameList.GetText(iCnt, CurStr);

		char strAnimName[MAX_PATH] = {};
		strcpy_s(strAnimName, CT2A(CurStr.GetBuffer()));
		fprintf_s(pFile, "%s\n", strAnimName);
	}
	//------------------------------------

	fclose(pFile);
	
	MessageBox(TEXT("저장 성공"));
}


void CPlayerEditDlg::OnBnClickedButtonLowersave()
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, m_strLowerFullPath, "wt");
	if (!pFile)
	{
		MessageBox(TEXT("저장 실패"));
		return;
	}

	//선택한 애니메이션 이름 저장
	//------------------------------------
	int iSize = m_LowerAnimNameList.GetCount();

	fprintf_s(pFile, "%d\n", iSize);
	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		CString CurStr;
		m_LowerAnimNameList.GetText(iCnt, CurStr);

		char strAnimName[MAX_PATH] = {};
		strcpy_s(strAnimName, CT2A(CurStr.GetBuffer()));
		fprintf_s(pFile, "%s\n", strAnimName);
	}
	//------------------------------------

	fclose(pFile);

	MessageBox(TEXT("저장 성공"));
}


void CPlayerEditDlg::OnBnClickedButtonRemoveupperanim()
{
	//선택된게 없는경우 pass
	//-------------------------
	if (m_iRemoveUpperAnimIdx == -1)
		return;
	//-------------------------

	m_UpperAnimNameList.DeleteString((UINT)m_iRemoveUpperAnimIdx);
}


void CPlayerEditDlg::OnBnClickedButtonRemoveloweranim()
{
	//선택된게 없는경우 pass
	//-------------------------
	if (m_iRemoveLowerAnimIdx == -1)
		return;
	//-------------------------

	m_LowerAnimNameList.DeleteString((UINT)m_iRemoveLowerAnimIdx);
}


void CPlayerEditDlg::OnLbnSelchangeListUpperanim()
{
	m_iRemoveUpperAnimIdx = m_UpperAnimNameList.GetCurSel();
}


void CPlayerEditDlg::OnLbnSelchangeListLoweranim()
{
	m_iRemoveLowerAnimIdx = m_LowerAnimNameList.GetCurSel();
}
