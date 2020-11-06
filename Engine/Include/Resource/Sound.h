#pragma once

#include "../Ref.h"

class CSound : public CRef
{
	friend class CResourceManager;

private:
	CSound();
	~CSound();

private:
	System*			m_pSystem;
	Sound*			m_pSound;
	bool			m_bLoop;
	ChannelGroup*	m_pGroup;
	Channel*		m_pChannel;
	bool			m_bPause;

public:
	bool LoadSound(System* pSystem, ChannelGroup* pGroup,
		bool bLoop, const string& strName, const char* pFileName,
		const string& strPathName = TEXTURE_PATH);
	void Play();
	void Stop();
	void Pause();
	void Resume();
};

