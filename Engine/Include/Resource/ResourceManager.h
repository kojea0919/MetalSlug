#pragma once

#include "../GameEngine.h"

class CResourceManager
{
#ifdef _EDITOR
	friend class CPlayerEditDlg;
#endif

private:
	System* m_pSystem;
	ChannelGroup* m_pMasterGroup;

private:
	IDWriteFactory* m_pWriteFactory;

private:
	//2D 기본 사각형
	class CMesh2D* m_pDefault2DMesh;

	//기본 UI 사각형
	class CMesh2D* m_pDefaultUIMesh;

	//UI 기본 Texture
	class CTexture* m_pDefaultUITexture;

	//Mesh 관리 Map
	unordered_map<string, class CMesh*>		m_mapMesh;

	//Material 관리 Map
	unordered_map<string, class CMaterial*> m_mapMaterial;

	//Texture 관리 Map
	unordered_map<string, class CTexture*> m_mapTexture;

	//AnimationSequence관리 Map
	unordered_map<string, class CAnimation2DSequence*> m_mapAnim2D;

	//Sound,Channel관리 Map
	unordered_map<string, class CSound*>		m_mapSound;
	unordered_map<string, class ChannelGroup*>	m_mapChannelGroup;

	//Text관리 Map
	unordered_map<string, IDWriteTextFormat*>	m_mapTextFormat;
	unordered_map<UINT, ID2D1SolidColorBrush*>	m_mapBrush;

	//Sampler
	//-----------------------------------
	ID3D11SamplerState* m_pPointSmp;
	ID3D11SamplerState* m_pLinearSmp;
	ID3D11SamplerState* m_pAnisotropicSmp;
	//-----------------------------------

public:
	//2D 기본 사각형 Get함수
	class CMesh2D* GetDefault2DMesh() const;
	//UI 기본 사각형 Get함수
	class CMesh2D* GetDefaultUIMesh()	const;
	//UI 기본 Texture Get함수
	class CTexture* GetDefaultUITexture()	const;
public:
	bool Init();

public:
	bool CreateMesh(MESH_TYPE eType, const string& strName,
		void* pVertices, int iVtxCount, int iVtxSize,
		D3D11_USAGE eVtxUsage,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void* pIndices = nullptr,
		int iIdxCount = 0, int iIdxSize = 0,
		D3D11_USAGE eIdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_UNKNOWN);
	void ReleaseMesh(const string& strName);
	class CMesh* FindMesh(const string& strName);

public:
	//Material 함수
	//-------------------------------------------------
	class CMaterial* CreateMaterial(const string& strName);
	void ReleaseMaterial(const string& strName);
	class CMaterial* FindMaterial(const string& strName);
	//-------------------------------------------------
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

	void ReleaseTexture(const string& strName);

	class CTexture* FindTexture(const string& strName);

public:
	//Animation2DSequence 함수
	//-------------------------------------------------
	bool CreateAnimation2DSequence(const string& strName,
		class CTexture* pTexture, float fPlayTime = 1.f,
		float fPlayRate = 1.f);
	bool LoadAnimation2DSequence(const string& strName,
		const char* pFileName,
		const string & strPathName = ANI_PATH);
	bool LoadAnimation2DSequence(const char* pFileName,
		const string& strPathName = ANI_PATH);
	void AddAnimation2DFrame(const string& strName,
		const Vector2& vStart, const Vector2& vEnd);
	void SetAnimation2DFrame(const string& strName,
		int iCount);
	void ReleaseAnimation2DSequence(const string& strName);
	class CAnimation2DSequence* FindAnim2DSequence(const string& strName);
	void AddAnimation2DNotify(const string& strSequence,
		const string& strName, int iFrame);
	//-------------------------------------------------

public:
	//Sound함수
	//-------------------------------------------------
	bool LoadSound(const string& strGroup, bool bLoop,
		const string& strName, const char* pFileName,
		const string& strPathName = SOUND_PATH);
	class CSound* FindSound(const string& strName);
	bool CreateChannelGroup(const string& strName);
	ChannelGroup* FindChannelGroup(const string& strName);
	void ReleaseSound(const string& strName);
	void SetVolume(int iVolume);
	void SetGroupVolume(const string& strGroup, int iVolume);
	bool SoundPlay(const string& strName);
	bool SoundStop(const string& strName);
	bool SoundPause(const string& strName);
	bool SoundResume(const string& strName);
	//-------------------------------------------------

public:
	//Text함수
	//-------------------------------------------------
	bool CreateTextFormat(const string& strName, const TCHAR* pFontName, int iWeight, int iStyle, int iStretch, float fSize,
		const TCHAR* pLocalName);
	ID2D1SolidColorBrush* CreateColor(float r, float g, float b, float a);
	ID2D1SolidColorBrush* CreateColor(BYTE r, BYTE g, BYTE b, BYTE a);
	ID2D1SolidColorBrush* CreateColor(const Vector4& vColor);
	ID2D1SolidColorBrush* CreateColor(UINT iColor);
	ID2D1SolidColorBrush* FindBrush(float r, float g, float b, float a);
	ID2D1SolidColorBrush* FindBrush(BYTE r, BYTE g, BYTE b, BYTE a);
	ID2D1SolidColorBrush* FindBrush(const Vector4& vColor);
	ID2D1SolidColorBrush* FindBrush(UINT iColor);
	UINT CreateColorKey(float r, float g, float b, float a);
	UINT CreateColorKey(BYTE r, BYTE g, BYTE b, BYTE a);
	UINT CreateColorKey(const Vector4& vColor);
	IDWriteTextLayout* CreateTextLayout(const TCHAR* pText, IDWriteTextFormat* pFormat, float fWidth, float fHeight);
	IDWriteTextLayout* CreateTextLayout(const TCHAR* pText, const string& strFormat, float fWidth, float fHeight);
	IDWriteTextFormat* FindFont(const string& strName);
	//-------------------------------------------------
private:
	void InitFMOD();

private:
	ID3D11SamplerState* CreateSampler(D3D11_FILTER eFilter);

	DECLARE_SINGLE(CResourceManager)
};