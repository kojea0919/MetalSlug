// CSceneEditor.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "SceneEditor.h"
#include "afxdialogex.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

// CSceneEditor 대화 상자

IMPLEMENT_DYNAMIC(CSceneEditor, CDialogEx)

CSceneEditor::CSceneEditor(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SCENEEDITOR, pParent)
{

}

CSceneEditor::~CSceneEditor()
{
}

void CSceneEditor::UpdateSceneEditorSequenceFileName()
{
	const char * pUpdateName = GET_SINGLE(CSceneManager)->GetScene()->GetAnimSequenceFileName();
	if (!pUpdateName)
		return;

	WCHAR ConvertName[MAX_PATH] = {};
	MultiByteToWideChar(CP_ACP, 0, pUpdateName, -1, ConvertName, sizeof(WCHAR) * MAX_PATH);

	CString a = ConvertName;

	m_AnimResourceFileName.SetWindowTextW(a.GetBuffer());
}

void CSceneEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SEQUENCEFILENAME, m_AnimResourceFileName);
}


BEGIN_MESSAGE_MAP(CSceneEditor, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ANIMRESOURCESET, &CSceneEditor::OnBnClickedButtonAnimresourceset)
END_MESSAGE_MAP()


// CSceneEditor 메시지 처리기


void CSceneEditor::OnBnClickedButtonAnimresourceset()
{
	//파일 다이얼로그 열기
	//------------------------------------------
	static TCHAR	szFilter[] = TEXT("텍스트 파일(*.txt)|*.txt;*.txt;|");
	CFileDialog filedlg(TRUE, TEXT(""), TEXT(""), OFN_OVERWRITEPROMPT, szFilter);
	filedlg.m_ofn.lpstrInitialDir = GET_SINGLE(CPathManager)->FindPath(RESOURCE_PATH);

	if (filedlg.DoModal() == IDOK)
	{
		CString strPathName = filedlg.GetPathName().GetBuffer();
		CString strFileName = filedlg.GetFileName().GetBuffer();

		//현재 Scene에서 사용할 SequenceList를 관리하는 파일 Setting
		GET_SINGLE(CSceneManager)->SetSequenceFileName(CT2A(strFileName));

		m_AnimResourceFileName.SetWindowTextW(strFileName.GetBuffer());
	}
	//------------------------------------------
}
