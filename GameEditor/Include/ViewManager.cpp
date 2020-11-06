#include "pch.h"
#include "ViewManager.h"
#include "Scene/SceneManager.h"
#include "WorldObjectDlg.h"
#include "DetailObjDlg.h"
#include "GameObject.h"
#include "Engine.h"

DEFINITION_SINGLE(CViewManager)

CViewManager::CViewManager()	:
	m_pWorldObjectDlg(nullptr),
	m_pDetailObjDlg(nullptr)
{
}

CViewManager::~CViewManager()
{
}

bool CViewManager::Init(CWorldObjectDlg* pWorldDlg, 
	CDetailObjDlg* pDetailDlg)
{
	m_pWorldObjectDlg = pWorldDlg;
	m_pDetailObjDlg = pDetailDlg;

	GET_SINGLE(CSceneManager)->SetEditorCreateFunction<CViewManager>(this, &CViewManager::CreateObjectCallback);
	GET_SINGLE(CEngine)->SetEditorClearCallback<CViewManager>(this, &CViewManager::EditorClear);

	return true;
}

void CViewManager::CreateObjectCallback(CGameObject* pObj)
{
	m_pWorldObjectDlg->AddObject(pObj->GetName());

	pObj->SetDestroyEditorFunction<CWorldObjectDlg>(m_pWorldObjectDlg,
		&CWorldObjectDlg::DestroyEditorObject);
}

void CViewManager::EditorClear()
{
}
