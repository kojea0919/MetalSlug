#pragma once

#include "GameEngine.h"

class CCollisionManager
{
private:
	//Channel���� Vector
	vector<PCollisionChannel>		m_vecChannel;
	
	//Profile���� map
	unordered_map<string, PCollisionProfile>		m_mapProfile;

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

