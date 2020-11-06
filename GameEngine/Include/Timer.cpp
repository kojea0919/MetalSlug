#include "Timer.h"

CTimer::CTimer()
	: m_fDeltaTime(0.f),m_fFPS(0.f),
	m_fFPSTime(0.f),m_iFrame(0)
{
}

CTimer::~CTimer()
{
}

bool CTimer::Init()
{
	return true;
}

void CTimer::Start()
{
	// Ÿ�̸Ӱ� 1�ʵ��� ����� �ֱ⸦ ������ ����
	QueryPerformanceFrequency(&m_tSecond);

	// Ÿ�̸��� ���� �ֱ⸦ ����
	QueryPerformanceCounter(&m_tPrevTime);
}

void CTimer::Update()
{
	LARGE_INTEGER tCurTime;

	QueryPerformanceCounter(&tCurTime);

	m_fDeltaTime = (tCurTime.QuadPart - m_tPrevTime.QuadPart) /
		(float)m_tSecond.QuadPart;

	m_tPrevTime = tCurTime;

	++m_iFrame;
	m_fFPSTime += m_fDeltaTime;

	if (m_fFPSTime >= 1.f)
	{
		m_fFPS = m_iFrame / m_fFPSTime;
		m_fFPSTime = 0.f;
		m_iFrame = 0;
	}
}
