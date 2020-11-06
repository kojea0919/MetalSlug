#pragma once

#include "../GameEngine.h"

class CUIManager
{
private:
	//UI용 투영행렬
	Matrix m_matProj;

	//현재 관리중인 UI
	vector<class CUIObject*>	m_vecUI;

	//현재 열려있는 UI
	list<class CUIObject*>		m_UIOpenList;

public:
	Matrix GetProjMatrix() const
	{
		return m_matProj;
	}

public:
	bool Init();

	DECLARE_SINGLE(CUIManager)
};

