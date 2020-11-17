#pragma once

#include "../GameEngine.h"

class CSceneResource
{
	friend class CScene;

private:
	CSceneResource();
	~CSceneResource();

private:
	unordered_map<string, class CMesh*>	m_mapMesh;

	//Shader관리 Map
	unordered_map<string, class CShader*>	m_mapShader;

	//Material관리 Map
	unordered_map<string, class CMaterial*>	m_mapMaterial;

	//Texture관리 Map
	unordered_map<string, class CTexture* > m_mapTexture;

	//Animation2DSequence관리 Map
	unordered_map<string, class CAnimation2DSequence*> m_mapAnim2D;

	//Sound관리 Map
	unordered_map<string, class CSound*>	m_mapSound;

public:
	void Start();

public:
	bool CreateMesh(MESH_TYPE eType, const string& strName,
		void* pVertices, int iVtxCount, int iVtxSize,
		D3D11_USAGE eVtxUsage,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void* pIndices = nullptr,
		int iIdxCount = 0, int iIdxSize = 0,
		D3D11_USAGE eIdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_UNKNOWN);
	class CMesh2D* GetDefault2DMesh()	const;
	class CMesh* FindMesh(const string& strName);

public:
	bool CreateMaterial(const string& strName);

	//Material 탐색함수
	class CMaterial* FindMaterial(const string& strName);

public:
	//해당 이름을 가진 Texture Load
	//-------------------------------------------------
	bool LoadTexture(const string& strName,
		const TCHAR* pFileName,
		const string& strPathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const string& strName,
		const TCHAR* pFullPath);
	//-------------------------------------------------

	//vector에 들어있는 모든 Texture Load
	//-------------------------------------------------
	bool LoadTexture(const string& strName,
		const vector<const TCHAR*>& vecFileName,
		const string& strPathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const string& strName,
		const vector<const TCHAR*>& vecFullPath);
	//-------------------------------------------------

	//Texture 탐색함수
	class CTexture* FindTexture(const string& strName);

public:
	//Animation2DSequence 생성, Load함수
	//-----------------------------------------------
	bool CreateAnimation2DSequence(const string& strName,
		class CTexture* pTexture, float fPlayTime = 1.f,
		float fPlayRate = 1.f);
	bool LoadAnimation2DSequence(const string& strName,
		const char* pFileName,
		const string& strPathName = RESOURCE_PATH);
	//-----------------------------------------------

	//Animation2DSequcen 시작 끝 Set함수
	//-----------------------------------------------
	void AddAnimation2DFrame(const string& strName,
		const Vector2& vStart, const Vector2& vEnd);
	//-----------------------------------------------

	void SetAnimation2DFrame(const string& strName,
		int iCount);

	//Animation2DSequcen 탐색함수
	class CAnimation2DSequence* FindAnim2DSequence(const string& strName);

	void AddAnimation2DNotify(const string& strSequence,
		const string& strName, int iFrame);

public:
	bool LoadSound(const string& strGroup, bool bLoop,
		const string& strName, const char* pFileName,
		const string& strPathName = SOUND_PATH);
	class CSound* FindSound(const string& strName);
	bool CreateChannelGroup(const string& strName);
	ChannelGroup* FindChannelGroup(const string& strName);
	void SetVolume(int iVolume);
	void SetGroupVolume(const string& strGroup, int iVolume);
	bool SoundPlay(const string& strName);
	bool SoundStop(const string& strName);
	bool SoundPause(const string& strName);
	bool SoundResume(const string& strName);


public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);
};

