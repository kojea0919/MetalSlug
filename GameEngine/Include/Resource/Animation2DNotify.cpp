#include "Animation2DNotify.h"

CAnimation2DNotify::CAnimation2DNotify()
	: m_iFrame(0)
{
}

CAnimation2DNotify::~CAnimation2DNotify()
{
}

void CAnimation2DNotify::Create(const string& strName, int iFrame)
{
	m_strName = strName;
	m_iFrame = iFrame;
}
