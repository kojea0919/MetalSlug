#pragma once

#include "GameEngine.h"

class CCollisionManager
{
private:
	//Channel���� Vector
	vector<PCollisionChannel>		m_vecChannel;
	
	//Profile���� map
	unordered_map<string, PCollisionProfile>		m_mapProfile;

	//Editor�� ��� Render
	bool			m_bColliderRender;

public:
	bool GetColliderRender()	const
	{
		return m_bColliderRender;
	}

	void SetColliderRender(bool bColliderRender)
	{
		m_bColliderRender = bColliderRender;
	}

public:
	bool Init();

public:
	//Chaennel �����Լ�
	bool CreateChannel(const string& strName, Collision_Type eDefaultType);

	// Profile �����Լ�
	bool CreateProfile(const string& strName, Collision_Channel eChannel);

	//�ش� Profile�� Channel Setting
	bool SetProfileChannelState(const string& strName,
		Collision_Channel eChannel, Collision_Type eType);

public:
	PCollisionProfile FindProfile(const string& strName);


	DECLARE_SINGLE(CCollisionManager)
};

