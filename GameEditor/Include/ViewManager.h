#pragma once

#include "GameEngine.h"

class CViewManager
{
private:
	class CWorldObjectDlg* m_pWorldObjectDlg;
	class CDetailObjDlg* m_pDetailObjDlg;

public:
	class CWorldObjectDlg* GetWorldObjectDlg()	const
	{
		return m_pWorldObjectDlg;
	}

	class CDetailObjDlg* GetDetailObjectDlg()	const
	{
		return m_pDetailObjDlg;
	}

public:
	bool Init(class CWorldObjectDlg* pWorldDlg,
		class CDetailObjDlg* pDetailDlg);
	void CreateObjectCallback(class CGameObject* pObj);
	void EditorClear();

	DECLARE_SINGLE(CViewManager)
};

