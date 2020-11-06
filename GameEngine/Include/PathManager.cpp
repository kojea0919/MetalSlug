#include "PathManager.h"

DEFINITION_SINGLE(CPathManager)

CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
	SAFE_DELETE_MAP(m_mapPath);
}

bool CPathManager::Init()
{
	TCHAR strPath[MAX_PATH] = {};

	GetModuleFileName(0, strPath, MAX_PATH);

	// �������� �̸��� �����Ѵ�.
	size_t	iLength = lstrlen(strPath);
	for (size_t i = iLength - 1; i > 0; --i)
	{
		if (strPath[i] == '\\')
		{
			memset(&strPath[i + 1], 0,
				sizeof(TCHAR) * (MAX_PATH - 1 - i));
			break;
		}
	}

	PPath	pPath = new Path;
	lstrcpy(pPath->pPath, strPath);

#ifdef UNICODE
	// ���ڿ��� multibyte�� ��ȯ�� ������ ���´�.
	int iLength1 = WideCharToMultiByte(CP_ACP, 0, strPath, -1, nullptr, 0,
		nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, strPath, -1, pPath->pPathMultibyte,
		iLength1, nullptr, nullptr);
#else
	strcpy_s(pPath->pPathMultibyte, strlen(strPath), strPath);
#endif // UNICODE

	m_mapPath.insert(make_pair(ROOT_PATH, pPath));

	AddPath(TEXTURE_PATH, TEXT("Resource\\Texture\\"));
	AddPath(RESOURCE_PATH, TEXT("Resource\\"));
	AddPath(DATA_PATH, TEXT("Data\\"));
	AddPath(SOUND_PATH, TEXT("Resource\\Sound\\"));
	AddPath(SHADER_PATH, TEXT("Resource\\Shader\\"));
	AddPath(ANI_PATH, TEXT("Resource\\Ani\\"));
	
	return true;
}

bool CPathManager::AddPath(const string& strName,
	const TCHAR* pPath, const string& strBasePath)
{
	// ���� �̸��� �н��� ���� ��� �����.
	if (FindPath(strName))
		return false;

	PPath	pNewPath = new Path;

	const TCHAR* pBasePath = FindPath(strBasePath);

	if (pBasePath)
		lstrcpy(pNewPath->pPath, pBasePath);

	lstrcat(pNewPath->pPath, pPath);

#ifdef UNICODE
	// ���ڿ��� multibyte�� ��ȯ�� ������ ���´�.
	int iLength = WideCharToMultiByte(CP_ACP, 0, pNewPath->pPath, -1, nullptr, 0,
		nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, pNewPath->pPath, -1, pNewPath->pPathMultibyte,
		iLength, nullptr, nullptr);
#else
	strcpy_s(pPath->pPathMultibyte, strlen(pNewPath->pPath), pNewPath->pPath);
#endif // UNICODE

	m_mapPath.insert(make_pair(strName, pNewPath));

	return true;
}

const TCHAR* CPathManager::FindPath(const string& strName)
{
	auto	iter = m_mapPath.find(strName);

	if (iter == m_mapPath.end())
		return nullptr;

	return iter->second->pPath;
}

const char* CPathManager::FindPathMultibyte(const string& strName)
{
	auto	iter = m_mapPath.find(strName);

	if (iter == m_mapPath.end())
		return nullptr;

	return iter->second->pPathMultibyte;
}
