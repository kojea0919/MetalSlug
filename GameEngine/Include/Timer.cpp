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
	// 타이머가 1초동안 몇번의 주기를 갖는지 저장
	QueryPerformanceFrequency(&m_tSecond);

	// 타이머의 현재 주기를 저장
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
