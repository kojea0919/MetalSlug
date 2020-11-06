#pragma once

#include "../GameEngine.h"

class CUIManager
{
private:
	//UI�� �������
	Matrix m_matProj;

	//���� �������� UI
	vector<class CUIObject*>	m_vecUI;

	//���� �����ִ� UI
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

