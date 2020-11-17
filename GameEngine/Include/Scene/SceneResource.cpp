#include "SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/Mesh.h"
#include "../Resource/Material.h"
#include "../Resource/Texture.h"
#include "../Resource/Animation2DSequence.h"
#include "../Resource/Shader.h"
#include "../Resource/Sound.h"

CSceneResource::CSceneResource()
{
}

CSceneResource::~CSceneResource()
{
	{
		auto	iter = m_mapMesh.begin();
		auto	iterEnd = m_mapMesh.end();

		for (; iter != iterEnd; ++iter)
		{
			GET_SINGLE(CResourceManager)->ReleaseMesh(iter->first);
		}
	}

	{
		auto	iter = m_mapMaterial.begin();
		auto	iterEnd = m_mapMaterial.end();

		for (; iter != iterEnd; ++iter)
		{
			GET_SINGLE(CResourceManager)->ReleaseMaterial(iter->first);
		}
	}

	{
		auto	iter = m_mapShader.begin();
		auto	iterEnd = m_mapShader.end();

		for (; iter != iterEnd; ++iter)
		{
			GET_SINGLE(CShaderManager)->ReleaseShader(iter->first);
		}
	}

	{
		auto	iter = m_mapTexture.begin();
		auto	iterEnd = m_mapTexture.end();

		for (; iter != iterEnd; ++iter)
		{
			GET_SINGLE(CResourceManager)->ReleaseTexture(iter->first);
		}
	}

	{
		auto	iter = m_mapAnim2D.begin();
		auto	iterEnd = m_mapAnim2D.end();
		
		for (; iter != iterEnd; ++iter)
		{
			GET_SINGLE(CResourceManager)->ReleaseAnimation2DSequence(iter->first);
		}
	}

	{
		auto	iter = m_mapSound.begin();
		auto	iterEnd = m_mapSound.end();

		for (; iter != iterEnd; ++iter)
		{
			GET_SINGLE(CResourceManager)->ReleaseSound(iter->first);
		}
	}
}

void CSceneResource::Start()
{
	auto iter = m_mapMaterial.begin();
	auto iterEnd = m_mapMaterial.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->Start();
	}
}

bool CSceneResource::CreateMesh(MESH_TYPE eType, const string& strName, void* pVertices, int iVtxCount, int iVtxSize, D3D11_USAGE eVtxUsage, D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void* pIndices, int iIdxCount, int iIdxSize, D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt)
{
	CMesh* pMesh = FindMesh(strName);

	if (pMesh)
	{
		SAFE_RELEASE(pMesh);
		return true;
	}

	if (!GET_SINGLE(CResourceManager)->CreateMesh(eType, strName,
		pVertices, iVtxCount, iVtxSize, eVtxUsage, ePrimitive, pIndices,
		iIdxCount, iIdxSize, eIdxUsage, eFmt))
		return false;

	pMesh = GET_SINGLE(CResourceManager)->FindMesh(strName);

	m_mapMesh.insert(make_pair(strName, pMesh));

	return true;
}

CMesh2D* CSceneResource::GetDefault2DMesh() const
{
	return GET_SINGLE(CResourceManager)->GetDefault2DMesh();
}

CMesh* CSceneResource::FindMesh(const string& strName)
{
	auto	iter = m_mapMesh.find(strName);

	if (iter == m_mapMesh.end())
	{
		CMesh* pMesh = GET_SINGLE(CResourceManager)->FindMesh(strName);

		if (!pMesh)
			return nullptr;

		m_mapMesh.insert(make_pair(strName, pMesh));

		pMesh->AddRef();

		return pMesh;
	}

	iter->second->AddRef();

	return iter->second;
}

bool CSceneResource::CreateMaterial(const string& strName)
{
	CMaterial* pMaterial = FindMaterial(strName);

	if (pMaterial)
	{
		SAFE_RELEASE(pMaterial);
		return true;
	}

	pMaterial = GET_SINGLE(CResourceManager)->CreateMaterial(strName);

	m_mapMaterial.insert(make_pair(strName, pMaterial));

	return true;
}

CMaterial* CSceneResource::FindMaterial(const string& strName)
{
	auto	iter = m_mapMaterial.find(strName);

	if (iter == m_mapMaterial.end())
	{
		CMaterial* pMaterial = GET_SINGLE(CResourceManager)->FindMaterial(strName);

		if (!pMaterial)
			return nullptr;

		m_mapMaterial.insert(make_pair(strName, pMaterial));

		pMaterial->AddRef();

		return pMaterial;
	}

	iter->second->AddRef();

	return iter->second;
}

bool CSceneResource::LoadTexture(const string& strName, const TCHAR* pFileName, const string& strPathName)
{
	//해당 이름의 텍스처가 SceneResource에 없으면 ResourceManager에서 Load
	//--------------------------------------------------------------------
	CTexture* pTexture = FindTexture(strName);

	if (pTexture)
	{
		SAFE_RELEASE(pTexture);
		return true;
	}

	if (!GET_SINGLE(CResourceManager)->LoadTexture(strName, pFileName, strPathName))
		return false;
	//--------------------------------------------------------------------

	pTexture = GET_SINGLE(CResourceManager)->FindTexture(strName);

	m_mapTexture.insert(make_pair(strName, pTexture));

	return true;
}

bool CSceneResource::LoadTextureFullPath(const string& strName, const TCHAR* pFullPath)
{
	CTexture* pTexture = FindTexture(strName);

	if (pTexture)
	{
		SAFE_RELEASE(pTexture);
		return true;
	}

	if (!GET_SINGLE(CResourceManager)->LoadTextureFullPath(strName, pFullPath))
		return false;

	pTexture = GET_SINGLE(CResourceManager)->FindTexture(strName);

	m_mapTexture.insert(make_pair(strName, pTexture));

	return true;
}

bool CSceneResource::LoadTexture(const string& strName, const vector<const TCHAR*>& vecFileName, const string& strPathName)
{
	CTexture* pTexture = FindTexture(strName);

	if (pTexture)
	{
		SAFE_RELEASE(pTexture);
		return true;
	}

	if (!GET_SINGLE(CResourceManager)->LoadTexture(
		strName, vecFileName, strPathName))
		return false;

	pTexture = GET_SINGLE(CResourceManager)->FindTexture(strName);

	m_mapTexture.insert(make_pair(strName, pTexture));

	return true;
}

bool CSceneResource::LoadTextureFullPath(const string& strName, const vector<const TCHAR*>& vecFullPath)
{
	CTexture* pTexture = FindTexture(strName);

	if (pTexture)
	{
		SAFE_RELEASE(pTexture);
		return true;
	}

	if (!GET_SINGLE(CResourceManager)->LoadTextureFullPath(strName, vecFullPath))
		return false;

	pTexture = GET_SINGLE(CResourceManager)->FindTexture(strName);

	m_mapTexture.insert(make_pair(strName, pTexture));

	return true;
}

CTexture* CSceneResource::FindTexture(const string& strName)
{
	auto iter = m_mapTexture.find(strName);
	
	if (iter == m_mapTexture.end())
	{
		CTexture* pTexture = GET_SINGLE(CResourceManager)->FindTexture(strName);

		if (!pTexture)
			return nullptr;

		pTexture->AddRef();

		m_mapTexture.insert(make_pair(strName, pTexture));

		return pTexture;
	}

	CTexture* pTargetTexture = iter->second;
	
	pTargetTexture->AddRef();

	return pTargetTexture;
}

bool CSceneResource::CreateAnimation2DSequence(const string& strName, CTexture* pTexture, float fPlayTime, float fPlayRate)
{
	//해당 이름의 Animation2DSequcen이 없는 경우
	//ResourceManager에 생성 요청
	//-----------------------------------------------
	CAnimation2DSequence* pSequence = FindAnim2DSequence(strName);

	if (pSequence)
	{
		SAFE_RELEASE(pSequence);
		return true;
	}

	if (!GET_SINGLE(CResourceManager)->CreateAnimation2DSequence(
		strName, pTexture, fPlayTime, fPlayRate))
		return false;
	//-----------------------------------------------

	pSequence = GET_SINGLE(CResourceManager)->FindAnim2DSequence(strName);

	m_mapAnim2D.insert(make_pair(strName, pSequence));

	return true;
}

bool CSceneResource::LoadAnimation2DSequence(const string& strName, const char* pFileName, const string& strPathName)
{
	CAnimation2DSequence* pSequence = FindAnim2DSequence(strName);

	if (pSequence)
	{
		SAFE_RELEASE(pSequence);
		return true;
	}

	if (!GET_SINGLE(CResourceManager)->LoadAnimation2DSequence(
		strName, pFileName, strPathName))
		return false;

	pSequence = GET_SINGLE(CResourceManager)->FindAnim2DSequence(strName);

	m_mapAnim2D.insert(make_pair(strName, pSequence));

	return true;
}

void CSceneResource::AddAnimation2DFrame(const string& strName, const Vector2& vStart, const Vector2& vEnd)
{
	GET_SINGLE(CResourceManager)->AddAnimation2DFrame(strName,
		vStart, vEnd);
}

void CSceneResource::SetAnimation2DFrame(const string& strName, int iCount)
{
	GET_SINGLE(CResourceManager)->SetAnimation2DFrame(strName,
		iCount);
}

CAnimation2DSequence* CSceneResource::FindAnim2DSequence(const string& strName)
{
	auto iter = m_mapAnim2D.find(strName);;

	if (iter == m_mapAnim2D.end())
	{
		CAnimation2DSequence* pSequence = GET_SINGLE(CResourceManager)->FindAnim2DSequence(strName);

		if (!pSequence)
			return nullptr;
		
		m_mapAnim2D.insert(make_pair(strName, pSequence));

		pSequence->AddRef();

		return pSequence;
	}

	iter->second->AddRef();

	return iter->second;
}

void CSceneResource::AddAnimation2DNotify(const string& strSequence, const string& strName, int iFrame)
{
	GET_SINGLE(CResourceManager)->AddAnimation2DNotify(strSequence, strName, iFrame);
}

bool CSceneResource::LoadSound(const string& strGroup, bool bLoop, const string& strName, const char* pFileName, const string& strPathName)
{
	CSound* pSound = FindSound(strName);

	if (pSound)
	{
		SAFE_RELEASE(pSound);
		return true;
	}

	if (!GET_SINGLE(CResourceManager)->LoadSound(strGroup, bLoop,
		strName, pFileName, strPathName))
		return false;

	pSound = GET_SINGLE(CResourceManager)->FindSound(strName);

	m_mapSound.insert(make_pair(strName, pSound));

	return true;
}

CSound* CSceneResource::FindSound(const string& strName)
{
	auto	iter = m_mapSound.find(strName);

	if (iter == m_mapSound.end())
	{
		CSound* pSound = GET_SINGLE(CResourceManager)->FindSound(strName);

		if (!pSound)
			return nullptr;

		m_mapSound.insert(make_pair(strName, pSound));

		pSound->AddRef();

		return pSound;
	}

	iter->second->AddRef();

	return iter->second;
}

bool CSceneResource::CreateChannelGroup(const string& strName)
{
	if (!GET_SINGLE(CResourceManager)->CreateChannelGroup(strName))
		return false;

	return true;
}

ChannelGroup* CSceneResource::FindChannelGroup(const string& strName)
{
	return GET_SINGLE(CResourceManager)->FindChannelGroup(strName);
}

void CSceneResource::SetVolume(int iVolume)
{
	GET_SINGLE(CResourceManager)->SetVolume(iVolume);
}

void CSceneResource::SetGroupVolume(const string& strGroup, int iVolume)
{
	GET_SINGLE(CResourceManager)->SetGroupVolume(strGroup, iVolume);
}

bool CSceneResource::SoundPlay(const string& strName)
{
	return GET_SINGLE(CResourceManager)->SoundPlay(strName);
}

bool CSceneResource::SoundStop(const string& strName)
{
	return GET_SINGLE(CResourceManager)->SoundStop(strName);
}

bool CSceneResource::SoundPause(const string& strName)
{
	return GET_SINGLE(CResourceManager)->SoundPause(strName);
}

bool CSceneResource::SoundResume(const string& strName)
{
	return GET_SINGLE(CResourceManager)->SoundResume(strName);
}

void CSceneResource::Save(FILE* pFile)
{
	//Material 저장
	//-----------------------------------------------

	//해당 Scene에서 사용하는 Material개수 저장
	int	iCount = (int)m_mapMaterial.size();
	fwrite(&iCount, sizeof(iCount), 1, pFile);

	{
		//각 Material 저장
		//----------------------------------------------
		auto	iter = m_mapMaterial.begin();
		auto	iterEnd = m_mapMaterial.end();

		for (; iter != iterEnd; ++iter)
		{
			//Material 이름 저장
			int	iLength = (int)iter->first.length();
			fwrite(&iLength, sizeof(iLength), 1, pFile);
			fwrite(iter->first.c_str(), 1, iLength, pFile);

			//Material 저장
			iter->second->Save(pFile);
		}
		//----------------------------------------------
	}
	//-----------------------------------------------


	//Mesh 저장
	//-----------------------------------------------
	
	//해당 Scene에서 사용하는 Mesh개수 저장
	iCount = (int)m_mapMesh.size();
	fwrite(&iCount, sizeof(iCount), 1, pFile);

	{
		//각 Mesh 저장
		//----------------------------------------------
		auto	iter = m_mapMesh.begin();
		auto	iterEnd = m_mapMesh.end();

		for (; iter != iterEnd; ++iter)
		{
			//Mesh이름 저장
			//------------------------------------------
			int	iLength = (int)iter->first.length();
			fwrite(&iLength, sizeof(iLength), 1, pFile);
			fwrite(iter->first.c_str(), 1, iLength, pFile);
			//------------------------------------------

			//Mesh 저장
			iter->second->Save(pFile);
		}
		//----------------------------------------------
	}
	//-----------------------------------------------


	//Shader 저장
	//-----------------------------------------------

	//해당 Scene에서 사용하는 Shader개수 저장
	iCount = (int)m_mapShader.size();
	fwrite(&iCount, sizeof(iCount), 1, pFile);

	{
		//각 Shader 저장
		//----------------------------------------------
		auto	iter = m_mapShader.begin();
		auto	iterEnd = m_mapShader.end();

		for (; iter != iterEnd; ++iter)
		{
			//Shader이름 저장
			//------------------------------------------
			int	iLength = (int)iter->first.length();
			fwrite(&iLength, sizeof(iLength), 1, pFile);
			fwrite(iter->first.c_str(), 1, iLength, pFile);
			//------------------------------------------

			//Shader 저장
			iter->second->Save(pFile);
		}
		//----------------------------------------------
	}
	//-----------------------------------------------


	//AnimationSequence 저장
	//-----------------------------------------------

	//해당 Scene에서 사용하는 AnimationSequence개수 저장
	iCount = (int)m_mapAnim2D.size();
	fwrite(&iCount, sizeof(iCount), 1, pFile);

	{
		//각 AnimationSequence 저장
		//----------------------------------------------
		auto	iter = m_mapAnim2D.begin();
		auto	iterEnd = m_mapAnim2D.end();

		for (; iter != iterEnd; ++iter)
		{
			//AnimationSequence이름 저장
			//------------------------------------------
			int	iLength = (int)iter->first.length();
			fwrite(&iLength, sizeof(iLength), 1, pFile);
			fwrite(iter->first.c_str(), 1, iLength, pFile);
			//------------------------------------------

			//AnimationSequence 저장
			iter->second->Save(pFile);
		}
		//----------------------------------------------
	}
	//-----------------------------------------------
}

void CSceneResource::Load(FILE* pFile)
{
	//Material Load
	//-----------------------------------------------
	int	iCount = 0;
	fread(&iCount, sizeof(iCount), 1, pFile);

	for (int i = 0; i < iCount; ++i)
	{
		int	iLength = 0;
		char	strName[256] = {};
		fread(&iLength, sizeof(iLength), 1, pFile);
		fread(strName, 1, iLength, pFile);

		CMaterial* pMaterial = GET_SINGLE(CResourceManager)->CreateMaterial(strName);

		pMaterial->Load(pFile);

		m_mapMaterial.insert(make_pair(strName, pMaterial));
	}
	//-----------------------------------------------

	//Mesh Load
	//-----------------------------------------------
	fread(&iCount, sizeof(iCount), 1, pFile);

	for (int i = 0; i < iCount; ++i)
	{
		int	iLength = 0;
		char	strName[256] = {};
		fread(&iLength, sizeof(iLength), 1, pFile);
		fread(strName, 1, iLength, pFile);

		CMesh* pMesh = GET_SINGLE(CResourceManager)->FindMesh(strName);

		pMesh->Load(pFile);

		m_mapMesh.insert(make_pair(strName, pMesh));
	}
	//-----------------------------------------------

	//Shader Load
	//-----------------------------------------------
	fread(&iCount, sizeof(iCount), 1, pFile);

	for (int i = 0; i < iCount; ++i)
	{
		int	iLength = 0;
		char	strName[256] = {};
		fread(&iLength, sizeof(iLength), 1, pFile);
		fread(strName, 1, iLength, pFile);

		CShader* pShader = GET_SINGLE(CShaderManager)->FindShader(strName);

		pShader->Load(pFile);

		m_mapShader.insert(make_pair(strName, pShader));
	}
	//-----------------------------------------------

	//AnimationSequence Load
	//-----------------------------------------------
	fread(&iCount, sizeof(iCount), 1, pFile);

	for (int i = 0; i < iCount; ++i)
	{
		int	iLength = 0;
		char	strName[256] = {};
		fread(&iLength, sizeof(iLength), 1, pFile);
		fread(strName, 1, iLength, pFile);

		CAnimation2DSequence* pAnimation2DSequence = GET_SINGLE(CResourceManager)->FindAnim2DSequence(strName);

		pAnimation2DSequence->Load(pFile);

		m_mapAnim2D.insert(make_pair(strName, pAnimation2DSequence));
	}
	//-----------------------------------------------
}
