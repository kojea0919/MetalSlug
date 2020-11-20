#pragma once

#include "GameEngine.h"

class CCollisionManager
{
private:
	//Channel包府 Vector
	vector<PCollisionChannel>		m_vecChannel;
	
	//Profile包府 map
	unordered_map<string, PCollisionProfile>		m_mapProfile;

	//Editor牢 版快 Render
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
	//Chaennel 积己窃荐
	bool CreateChannel(const string& strName, Collision_Type eDefaultType);

	// Profile 积己窃荐
	bool CreateProfile(const string& strName, Collision_Channel eChannel);

	//秦寸 Profile俊 Channel Setting
	bool SetProfileChannelState(const string& strName,
		Collision_Channel eChannel, Collision_Type eType);

public:
	PCollisionProfile FindProfile(const string& strName);


	DECLARE_SINGLE(CCollisionManager)
};

