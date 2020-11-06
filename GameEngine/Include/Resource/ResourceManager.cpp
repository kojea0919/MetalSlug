#include "ResourceManager.h"
#include "ShaderManager.h"
#include "Mesh.h"
#include "Mesh2D.h"
#include "StaticMesh.h"
#include "Material.h"
#include "Texture.h"
#include "../Device.h"
#include "Animation2DSequence.h"
#include "Sound.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager()
	: m_pDefault2DMesh(nullptr),
	m_pPointSmp(nullptr),m_pLinearSmp(nullptr),
	m_pAnisotropicSmp(nullptr),m_pDefaultUIMesh(nullptr),
	m_pDefaultUITexture(nullptr)
{

}

CResourceManager::~CResourceManager()
{
	DESTROY_SINGLE(CShaderManager);

	SAFE_RELEASE(m_pDefaultUITexture);
	SAFE_RELEASE(m_pPointSmp);
	SAFE_RELEASE(m_pLinearSmp);
	SAFE_RELEASE(m_pAnisotropicSmp);
	SAFE_RELEASE(m_pDefault2DMesh);
	SAFE_RELEASE(m_pDefaultUIMesh);
	SAFE_RELEASE_MAP(m_mapAnim2D);
	SAFE_RELEASE_MAP(m_mapMesh);
	SAFE_RELEASE_MAP(m_mapMaterial);
	SAFE_RELEASE_MAP(m_mapTexture);
	SAFE_RELEASE_MAP(m_mapSound);

	{
		auto	iter = m_mapChannelGroup.begin();
		auto	iterEnd = m_mapChannelGroup.end();

		for (; iter != iterEnd; ++iter)
		{
			iter->second->release();
		}
	}

	if (m_pSystem)
	{
		m_pSystem->close();
		m_pSystem->release();
	}
}

CMesh2D* CResourceManager::GetDefault2DMesh() const
{
	m_pDefault2DMesh->AddRef();
	return m_pDefault2DMesh;
}

CMesh2D* CResourceManager::GetDefaultUIMesh() const
{
	m_pDefaultUIMesh->AddRef();
	return m_pDefaultUIMesh;
}

CTexture* CResourceManager::GetDefaultUITexture() const
{
	m_pDefaultUITexture->AddRef();
	return m_pDefaultUITexture;
}

bool CResourceManager::Init()
{
	InitFMOD();

	//ShaderManager 초기화
	//--------------------------------------
	if (!GET_SINGLE(CShaderManager)->Init())
		return false;
	//--------------------------------------

	
	//2D 기본 사각형 생성
	//--------------------------------------
	m_pDefault2DMesh = new CMesh2D;

	//정점,인덱스 버퍼 Setting
	if (!m_pDefault2DMesh->CreateMesh())
		return false;

	//Material정보 Setting
	//---------------------------------------------------
	CMaterial* pDefaultcolorMtrl = CreateMaterial("Default2D");
	pDefaultcolorMtrl->SetShader("Standard2DShader");

	m_pDefault2DMesh->SetMaterial(pDefaultcolorMtrl);
	SAFE_RELEASE(pDefaultcolorMtrl);
	//---------------------------------------------------

	//--------------------------------------



	//UI 기본 사각형 생성
	//--------------------------------------
	m_pDefaultUIMesh = new CMesh2D;

	if (!m_pDefaultUIMesh->CreateMesh())
		return false;

	CMaterial* pDefaultUIMtrl = CreateMaterial("DefaultUI");

	pDefaultUIMtrl->SetShader("UIShader");

	m_pDefaultUIMesh->SetMaterial(pDefaultUIMtrl);
	SAFE_RELEASE(pDefaultUIMtrl);

	//--------------------------------------


	//Sampler Setting
	//----------------------------------------------------

	//확대, 축소, Mipmap모두 점 보간을 사용하는 Sampler
	m_pPointSmp = CreateSampler(D3D11_FILTER_MIN_MAG_MIP_POINT);

	//확대, 축소, Mipmap모두 선형 보간을 사용하는 Sampler
	m_pLinearSmp = CreateSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR);

	//확대, 축소, Mipmap모두 비등방 필터링을 사용하는 Sampler
	m_pAnisotropicSmp = CreateSampler(D3D11_FILTER_ANISOTROPIC);

	//https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-vssetsamplers
	CONTEXT->VSSetSamplers(0, 1, &m_pPointSmp);
	CONTEXT->PSSetSamplers(0, 1, &m_pPointSmp);
	CONTEXT->DSSetSamplers(0, 1, &m_pPointSmp);
	CONTEXT->HSSetSamplers(0, 1, &m_pPointSmp);
	CONTEXT->GSSetSamplers(0, 1, &m_pPointSmp);
	CONTEXT->CSSetSamplers(0, 1, &m_pPointSmp);

	CONTEXT->VSSetSamplers(1, 1, &m_pLinearSmp);
	CONTEXT->PSSetSamplers(1, 1, &m_pLinearSmp);
	CONTEXT->DSSetSamplers(1, 1, &m_pLinearSmp);
	CONTEXT->HSSetSamplers(1, 1, &m_pLinearSmp);
	CONTEXT->GSSetSamplers(1, 1, &m_pLinearSmp);
	CONTEXT->CSSetSamplers(1, 1, &m_pLinearSmp);

	CONTEXT->VSSetSamplers(2, 1, &m_pAnisotropicSmp);
	CONTEXT->PSSetSamplers(2, 1, &m_pAnisotropicSmp);
	CONTEXT->DSSetSamplers(2, 1, &m_pAnisotropicSmp);
	CONTEXT->HSSetSamplers(2, 1, &m_pAnisotropicSmp);
	CONTEXT->GSSetSamplers(2, 1, &m_pAnisotropicSmp);
	CONTEXT->CSSetSamplers(2, 1, &m_pAnisotropicSmp);

	//----------------------------------------------------

	Vector3	vRectPos[5] =
	{
		Vector3(-0.5f, 0.5f, 0.f),
		Vector3(0.5f, 0.5f, 0.f),
		Vector3(0.5f, -0.5f, 0.f),
		Vector3(-0.5f, -0.5f, 0.f),
		Vector3(-0.5f, 0.5f, 0.f)
	};

	CreateMesh(MESH_TYPE::MESH2D, "ColliderRect", vRectPos, 5, sizeof(Vector3),
		D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	CMaterial* pMaterial = CreateMaterial("Collider");

	pMaterial->SetShader("ColliderShader");

	SAFE_RELEASE(pMaterial);

	//원 Mesh Setting
	//----------------------------------------------------
	Vector3	vSphere2D[361];

	vSphere2D[0] = Vector3(1.f, 0.f, 0.f);

	for (int i = 1; i <= 360; ++i)
	{
		vSphere2D[i].x = cosf(DegreeToRadian((float)i));
		vSphere2D[i].y = sinf(DegreeToRadian((float)i));
	}

	CreateMesh(MESH_TYPE::MESH2D, "ColliderSphere2D", vSphere2D,
		361, sizeof(Vector3),
		D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	//----------------------------------------------------

	//기본 UI Texture Load
	//----------------------------------------------------
	m_pDefaultUITexture = new CTexture;

	m_pDefaultUITexture->LoadTexture("DefaultUI", TEXT("DefaultUI.png"));

	LoadTexture("EngineMouseDefault", TEXT("MouseDefault.png"));

	CTexture* pTexture = FindTexture("EngineMouseDefault");

	CreateAnimation2DSequence("EngineMouseDefault",
		pTexture);

	SetAnimation2DFrame("EngineMouseDefault", 1);

	SAFE_RELEASE(pTexture);

	vector<const TCHAR*>		vecFileName;

	for (int iCnt = 0; iCnt < 13; ++iCnt)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];
		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("%d.png"), iCnt);

		vecFileName.push_back(pFileName);
	}

	LoadTexture("MouseIdle", vecFileName);

	SAFE_DELETE_ARRAY_VECLIST(vecFileName);

	pTexture = FindTexture("MouseIdle");

	CreateAnimation2DSequence("MouseIdle",
		pTexture);

	SetAnimation2DFrame("MouseIdle",
		(int)pTexture->GetImageCount());

	SAFE_RELEASE(pTexture);

	//----------------------------------------------------

	return true;
}



bool CResourceManager::CreateMesh(MESH_TYPE eType, const string& strName, void* pVertices, int iVtxCount, int iVtxSize, D3D11_USAGE eVtxUsage, D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void* pIndices, int iIdxCount, int iIdxSize, D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt)
{
	//없는 경우만 생성
	//-------------------------------
	CMesh* pMesh = FindMesh(strName);

	if (pMesh)
	{
		SAFE_RELEASE(pMesh);
		return true;
	}
	//-------------------------------

	switch (eType)
	{
	case MESH_TYPE::MESH2D:
		pMesh = new CMesh2D;
		break;
	case MESH_TYPE::STATICMESH:
		pMesh = new CStaticMesh;
		break;
	case MESH_TYPE::ANIMATIONMESH:
		break;
	}

	pMesh->SetName(strName);

	if (!pMesh->CreateMesh(pVertices, iVtxCount, iVtxSize, eVtxUsage, ePrimitive,
		pIndices, iIdxCount, iIdxSize, eIdxUsage, eFmt))
	{
		SAFE_RELEASE(pMesh);
		return false;
	}

	m_mapMesh.insert(make_pair(strName, pMesh));

	return false;
}

void CResourceManager::ReleaseMesh(const string& strName)
{
	auto iter = m_mapMesh.find(strName);

	if (iter == m_mapMesh.end())
		return;

	if (iter->second->Release() == 0)
		m_mapMesh.erase(iter);
}

CMesh* CResourceManager::FindMesh(const string& strName)
{
	auto iter = m_mapMesh.find(strName);

	if (iter == m_mapMesh.end())
		return nullptr;

	CMesh* pTargetMesh = iter->second;

	pTargetMesh->AddRef();

	return pTargetMesh;

	return nullptr;
}

CMaterial* CResourceManager::CreateMaterial(const string& strName)
{
	CMaterial* pMaterial = FindMaterial(strName);

	if (pMaterial)
		return pMaterial;

	pMaterial = new CMaterial;

	pMaterial->AddRef();
	pMaterial->SetName(strName);

	m_mapMaterial.insert(make_pair(strName, pMaterial));

	return pMaterial;
}

void CResourceManager::ReleaseMaterial(const string& strName)
{
	auto iter = m_mapMaterial.find(strName);
	
	if (iter == m_mapMaterial.end())
		return;

	if (iter->second->Release() == 0)
		m_mapMaterial.erase(iter);
}

CMaterial* CResourceManager::FindMaterial(const string& strName)
{
	auto iter = m_mapMaterial.find(strName);

	if (iter == m_mapMaterial.end())
		return nullptr;

	CMaterial* pTargetMaterial = iter->second;

	pTargetMaterial->AddRef();

	return pTargetMaterial;
}

bool CResourceManager::LoadTexture(const string& strName, const TCHAR* pFileName, const string& strPathName)
{
	//해당 이름의 Texture가 없는 경우 생성후 Load
	//----------------------------------------------
	CTexture* pTexture = FindTexture(strName);

	if (pTexture)
	{
		SAFE_RELEASE(pTexture);
		return true;
	}

	pTexture = new CTexture;

	//Load
	if (!pTexture->LoadTexture(strName, pFileName, strPathName))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	m_mapTexture.insert(make_pair(strName, pTexture));

	//----------------------------------------------

	return true;
}

bool CResourceManager::LoadTextureFullPath(const string& strName, const TCHAR* pFullPath)
{
	//해당 이름의 Texture가 없는 경우 생성후 Load
	//----------------------------------------------
	CTexture* pTexture = FindTexture(strName);

	if (pTexture)
	{
		SAFE_RELEASE(pTexture);
		return true;
	}

	pTexture = new CTexture;

	//Load
	if (!pTexture->LoadTextureFullPath(strName, pFullPath))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	m_mapTexture.insert(make_pair(strName, pTexture));
	//----------------------------------------------

	return true;
}

bool CResourceManager::LoadTexture(const string& strName, const vector<const TCHAR*>& vecFileName, const string& strPathName)
{
	CTexture* pTexture = FindTexture(strName);

	if (pTexture)
	{
		SAFE_RELEASE(pTexture);
		return true;
	}

	pTexture = new CTexture;
	if (!pTexture->LoadTexture(strName, vecFileName, strPathName))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	m_mapTexture.insert(make_pair(strName, pTexture));

	return true;
}

bool CResourceManager::LoadTextureFullPath(const string& strName, const vector<const TCHAR*>& vecFullPath)
{
	CTexture* pTexture = FindTexture(strName);

	if (pTexture)
	{
		SAFE_RELEASE(pTexture);
		return true;
	}

	pTexture = new CTexture;

	if (!pTexture->LoadTextureFullPath(strName, vecFullPath))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	m_mapTexture.insert(make_pair(strName, pTexture));

	return true;
}

void CResourceManager::ReleaseTexture(const string& strName)
{
	auto	iter = m_mapTexture.find(strName);

	if (iter == m_mapTexture.end())
		return;

	if (iter->second->Release() == 0)
		m_mapTexture.erase(iter);
}

CTexture* CResourceManager::FindTexture(const string& strName)
{
	auto iter = m_mapTexture.find(strName);

	if (iter == m_mapTexture.end())
		return nullptr;

	CTexture* pTargetTexture = iter->second;
	pTargetTexture->AddRef();

	return pTargetTexture;
}

bool CResourceManager::CreateAnimation2DSequence(const string& strName, CTexture* pTexture, float fPlayTime, float fPlayRate)
{
	//해당 이름의 Sequence가 없는 경우에만 생성
	//------------------------------------------------
	CAnimation2DSequence* pSequence = FindAnim2DSequence(strName);

	if (pSequence)
	{
		SAFE_RELEASE(pSequence);
		return true;
	}

	pSequence = new CAnimation2DSequence;
	//------------------------------------------------

	//초기 Setting
	//------------------------------------------------
	pSequence->SetName(strName);

	if (!pSequence->Init(pTexture, fPlayTime, fPlayRate))
	{
		SAFE_RELEASE(pSequence);
		return false;
	}
	//------------------------------------------------

	m_mapAnim2D.insert(make_pair(strName, pSequence));

	return true;
}

bool CResourceManager::LoadAnimation2DSequence(const string& strName, const char* pFileName, const string& strPathName)
{
	CAnimation2DSequence* pSequence = FindAnim2DSequence(strName);

	if (pSequence)
	{
		SAFE_RELEASE(pSequence);
		return true;
	}

	pSequence = new CAnimation2DSequence;

	pSequence->SetName(strName);

	if (!pSequence->Init(pFileName, strPathName))
	{
		SAFE_RELEASE(pSequence);
		return false;
	}

	m_mapAnim2D.insert(make_pair(strName, pSequence));

	return true;
}

bool CResourceManager::LoadAnimation2DSequence(const char* pFileName, const string& strPathName)
{
	CAnimation2DSequence * pSequence = new CAnimation2DSequence;

	if (!pSequence->Init(pFileName, strPathName))
	{
		SAFE_RELEASE(pSequence);
		return false;
	}

	m_mapAnim2D.insert(make_pair(pSequence->GetName(), pSequence));

	return true;
}

void CResourceManager::AddAnimation2DFrame(const string& strName, const Vector2& vStart, const Vector2& vEnd)
{
	CAnimation2DSequence* pSequence = FindAnim2DSequence(strName);

	if (!pSequence)
		return;

	pSequence->AddFrame(vStart, vEnd);

	SAFE_RELEASE(pSequence);
}

void CResourceManager::SetAnimation2DFrame(const string& strName, int iCount)
{
	CAnimation2DSequence* pSequence = FindAnim2DSequence(strName);

	if (!pSequence)
		return;

	pSequence->SetFrame(iCount);

	SAFE_RELEASE(pSequence);
}

void CResourceManager::ReleaseAnimation2DSequence(const string& strName)
{
	auto	iter = m_mapAnim2D.find(strName);

	if (iter == m_mapAnim2D.end())
		return;

	if (iter->second->Release() == 0)
	{
		m_mapAnim2D.erase(iter);
	}
}

CAnimation2DSequence* CResourceManager::FindAnim2DSequence(const string& strName)
{
	auto	iter = m_mapAnim2D.find(strName);

	if (iter == m_mapAnim2D.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CResourceManager::AddAnimation2DNotify(const string& strSequence, const string& strName, int iFrame)
{
	CAnimation2DSequence* pSequence = FindAnim2DSequence(strSequence);

	if (!pSequence)
		return;

	pSequence->AddNotify(strName, iFrame);

	SAFE_RELEASE(pSequence);
}

bool CResourceManager::LoadSound(const string& strGroup, bool bLoop, const string& strName, const char* pFileName, const string& strPathName)
{
	CSound* pSound = FindSound(strName);

	if (pSound)
	{
		SAFE_RELEASE(pSound);
		return false;
	}

	ChannelGroup* pGroup = FindChannelGroup(strGroup);

	if (!pGroup)
		return false;

	pSound = new CSound;

	if (!pSound->LoadSound(m_pSystem, pGroup, bLoop, strName, pFileName,
		strPathName))
	{
		SAFE_RELEASE(pSound);
		return false;
	}

	m_mapSound.insert(make_pair(strName, pSound));

	return true;
}

CSound* CResourceManager::FindSound(const string& strName)
{
	auto	iter = m_mapSound.find(strName);

	if (iter == m_mapSound.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

bool CResourceManager::CreateChannelGroup(const string& strName)
{
	ChannelGroup* pGroup = FindChannelGroup(strName);

	if (pGroup)
		return false;

	m_pSystem->createChannelGroup(strName.c_str(), &pGroup);

	m_pMasterGroup->addGroup(pGroup, false, 0);

	m_mapChannelGroup.insert(make_pair(strName, pGroup));

	return true;
}

ChannelGroup* CResourceManager::FindChannelGroup(const string& strName)
{
	auto	iter = m_mapChannelGroup.find(strName);

	if (iter == m_mapChannelGroup.end())
		return nullptr;

	return iter->second;
}

void CResourceManager::ReleaseSound(const string& strName)
{
	auto		iter = m_mapSound.find(strName);

	if (iter == m_mapSound.end())
		return;

	// Release함수가 true를 반환하면 지워졌다는 것이다.
	if (iter->second->Release() == 0)
		m_mapSound.erase(iter);
}

void CResourceManager::SetVolume(int iVolume)
{
	m_pMasterGroup->setVolume(iVolume / 100.f);
}

void CResourceManager::SetGroupVolume(const string& strGroup, int iVolume)
{
	ChannelGroup* pGroup = FindChannelGroup(strGroup);

	if (!pGroup)
		return;

	pGroup->setVolume(iVolume / 100.f);
}

bool CResourceManager::SoundPlay(const string& strName)
{
	CSound* pSound = FindSound(strName);

	if (!pSound)
		return false;

	pSound->Play();

	SAFE_RELEASE(pSound);

	return true;
}

bool CResourceManager::SoundStop(const string& strName)
{
	CSound* pSound = FindSound(strName);

	if (!pSound)
		return false;

	pSound->Stop();

	SAFE_RELEASE(pSound);

	return true;
}

bool CResourceManager::SoundPause(const string& strName)
{
	CSound* pSound = FindSound(strName);

	if (!pSound)
		return false;

	pSound->Pause();

	SAFE_RELEASE(pSound);

	return true;
}

bool CResourceManager::SoundResume(const string& strName)
{
	CSound* pSound = FindSound(strName);

	if (!pSound)
		return false;

	pSound->Resume();

	SAFE_RELEASE(pSound);

	return true;
}

void CResourceManager::InitFMOD()
{
	//FMOD System을 생성
	System_Create(&m_pSystem);

	//FMOD를 초기화(FMOD는 채널 사용)
	m_pSystem->init(256, FMOD_INIT_NORMAL, nullptr);

	//채널 그룹 미리 생성
	m_pSystem->getMasterChannelGroup(&m_pMasterGroup);

	CreateChannelGroup("BGM");
	CreateChannelGroup("Effect");
	CreateChannelGroup("UI");
}

ID3D11SamplerState* CResourceManager::CreateSampler(D3D11_FILTER eFilter)
{
	//생성할 Sampler정보 Setting
	//--------------------------------------------------------
	D3D11_SAMPLER_DESC	tDesc = {};

	//https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_sampler_desc
	tDesc.Filter = eFilter;								//텍스처를 샘플링 할 때 사용할 필터링 방법
	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		//범위 밖에있는 U좌표에 대한 처리 방법
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		//범위 밖에있는 V좌표에 대한 처리 방법
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;		
	tDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;		//샘플링된 데이터를 기존 샘플링된 데이터와 비교하는 함수
	tDesc.MinLOD = -FLT_MAX;							
	tDesc.MaxLOD = FLT_MAX;
	//--------------------------------------------------------

	//Sampler생성
	//--------------------------------------------------------
	ID3D11SamplerState* pSampler = nullptr;

	DEVICE->CreateSamplerState(&tDesc, &pSampler);
	//--------------------------------------------------------

	return pSampler;
}
