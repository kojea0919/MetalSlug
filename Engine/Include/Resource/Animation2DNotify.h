#pragma once

#include "../GameEngine.h"

class CAnimation2DNotify
{
	friend class CAnimation2DSequence;

private:
	CAnimation2DNotify();
	~CAnimation2DNotify();

private:
	class CAnimation2DSequence* m_pOwner;
	string		m_strName;
	int			m_iFrame;

public:
	string GetName() const
	{
		return m_strName;
	}

public:
	void Create(const string& strName, int iFrame);
};

