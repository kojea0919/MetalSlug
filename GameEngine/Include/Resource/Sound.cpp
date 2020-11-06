#include "Sound.h"
#include "../PathManager.h"

CSound::CSound()
	: m_pSound(nullptr), m_pChannel(nullptr),
	m_bPause(false)
{
}

CSound::~CSound()
{
	if (m_pSound)
		m_pSound->release();
}

bool CSound::LoadSound(System* pSystem, ChannelGroup* pGroup, bool bLoop, const string& strName, const char* pFileName, const string& strPathName)
{
	SetName(strName);

	m_pSystem = pSystem;
	m_pGroup = pGroup;
	m_bLoop = bLoop;

	char strFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindPathMultibyte(strPathName);

	if (pPath)
		strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFileName);

	FMOD_MODE	eMode = FMOD_DEFAULT;

	if (bLoop)
		eMode = FMOD_LOOP_NORMAL;

	FMOD_RESULT test = m_pSystem->createSound(strFullPath, eMode, nullptr, &m_pSound);

	return true;

}

void CSound::Play()
{
	m_pSystem->playSound(m_pSound, m_pGroup, false,
		&m_pChannel);
}

void CSound::Stop()
{
	if (m_pChannel)
	{
		bool	bPlaying = false;
		m_pChannel->isPlaying(&bPlaying);

		if (bPlaying)
		{
			m_pChannel->stop();
			m_pChannel = nullptr;
		}
	}
}

void CSound::Pause()
{
	if (!m_pChannel || m_bPause)
		return;

	bool	bPlaying = false;

	m_pChannel->isPlaying(&bPlaying);

	if (bPlaying)
		m_pChannel->setPaused(true);

	m_bPause = true;
}

void CSound::Resume()
{
	if (!m_pChannel || !m_bPause)
		return;

	bool	bPlaying = false;

	m_pChannel->isPlaying(&bPlaying);

	if (!bPlaying)
		m_pChannel->setPaused(false);

	m_bPause = false;
}
