#pragma once

#include "GameEngine.h"

//AniPath
//---------------------------------
#define df_PLAYER_UPPERANIM_FILENAME "PlayerUpperAnimList.txt"
#define df_PLAYER_LOWERANIM_FILENAME "PlayerLowerAnimList.txt"
//---------------------------------

typedef struct _tagPath
{
	TCHAR* pPath;
	char* pPathMultibyte;

	_tagPath()
	{
		pPath = new TCHAR[MAX_PATH];
		pPathMultibyte = new char[MAX_PATH];

		memset(pPath, 0, sizeof(TCHAR) * MAX_PATH);
		memset(pPathMultibyte, 0, sizeof(char) * MAX_PATH);
	}

	~_tagPath()
	{
		SAFE_DELETE_ARRAY(pPath);
		SAFE_DELETE_ARRAY(pPathMultibyte);
	}
}Path, *PPath;

class CPathManager
{
private:
	unordered_map<string, PPath>	m_mapPath;

public:
	bool Init();
	bool AddPath(const string& strName, const TCHAR* pPath,
		const string& strBasePath = ROOT_PATH);
	const TCHAR* FindPath(const string& strName);
	const char* FindPathMultibyte(const string& strName);

	DECLARE_SINGLE(CPathManager)
};

