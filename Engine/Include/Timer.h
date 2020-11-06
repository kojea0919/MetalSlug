#pragma once

#include "GameEngine.h"

class CTimer
{
public:
	CTimer();
	~CTimer();

private:
	LARGE_INTEGER		m_tSecond;
	LARGE_INTEGER		m_tPrevTime;

	float				m_fDeltaTime;
	float				m_fFPS;
	float				m_fFPSTime;
	int					m_iFrame;

public:
	float GetDeltaTime() const
	{
		return m_fDeltaTime;
	}

	float GetFPS() const
	{
		return m_fFPS;
	}

public:
	bool Init();
	void Start();
	void Update();
};