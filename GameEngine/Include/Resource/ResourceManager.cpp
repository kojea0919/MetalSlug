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
	m_pDefaultUITexture(nullptr),m_pWriteFactory(nullptr)
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

	SAFE_RELEASE_MAP(m_mapTextFormat);
	SAFE_RELEASE_MAP(m_mapBrush);
	SAFE_RELEASE(m_pWriteFactory);

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

	// DirectWrite ���丮 ����
	//-------------------------------------------------------------------------
	// HRESULT DWriteCreateFactory(DWRITE_FACTORY_TYPE factoryType, REFIID iid, IUnknown **factory);

	// 1. DWRITE_FACTORY_TYPE factoryType - ���丮�� �������� �ݸ������� ���Ѵ�.  �ݸ��� �� DWRITE_FACTORY_TYPE_ISOLATED 
	// 2. REFIID iid - DirectWrite ���丮 �������̽��� �ĺ��ϴ� GUID ��
	// 3. ���� �� DirectWrite ���丮�� ��ȯ ���� ������
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_pWriteFactory), (IUnknown**)&m_pWriteFactory)))
		return false;

	CreateTextFormat("NormalText", TEXT("�ü�ü"), DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_ULTRA_EXPANDED, 20.f, TEXT("ko"));
	CreateTextFormat("NormalText1", TEXT("Noto Sans CJK KR Black"), DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_ULTRA_EXPANDED, 20.f, TEXT("ko"));
	//-------------------------------------------------------------------------


	//ShaderManager �ʱ�ȭ
	//--------------------------------------
	if (!GET_SINGLE(CShaderManager)->Init())
		return false;
	//--------------------------------------

	
	//2D �⺻ �簢�� ����
	//--------------------------------------
	m_pDefault2DMesh = new CMesh2D;

	//����,�ε��� ���� Setting
	if (!m_pDefault2DMesh->CreateMesh())
		return false;

	//Material���� Setting
	//---------------------------------------------------
	CMaterial* pDefaultcolorMtrl = CreateMaterial("Default2D");
	pDefaultcolorMtrl->SetShader("Standard2DShader");

	m_pDefault2DMesh->SetMaterial(pDefaultcolorMtrl);
	SAFE_RELEASE(pDefaultcolorMtrl);

	pDefaultcolorMtrl = CreateMaterial("Sprite");
	pDefaultcolorMtrl->SetShader("SpriteShader");
	SAFE_RELEASE(pDefaultcolorMtrl);
	//---------------------------------------------------

	//--------------------------------------


	//UI �⺻ �簢�� ����
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

	//Ȯ��, ���, Mipmap��� �� ������ ����ϴ� Sampler
	m_pPointSmp = CreateSampler(D3D11_FILTER_MIN_MAG_MIP_POINT);

	//Ȯ��, ���, Mipmap��� ���� ������ ����ϴ� Sampler
	m_pLinearSmp = CreateSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR);

	//Ȯ��, ���, Mipmap��� ���� ���͸��� ����ϴ� Sampler
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

	//�� Mesh Setting
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

	//�⺻ UI Texture Load
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
	//���� ��츸 ����
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
	//�ش� �̸��� Texture�� ���� ��� ������ Load
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
	//�ش� �̸��� Texture�� ���� ��� ������ Load
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
	//�ش� �̸��� Sequence�� ���� ��쿡�� ����
	//------------------------------------------------
	CAnimation2DSequence* pSequence = FindAnim2DSequence(strName);

	if (pSequence)
	{
		SAFE_RELEASE(pSequence);
		return true;
	}

	pSequence = new CAnimation2DSequence;
	//------------------------------------------------

	//�ʱ� Setting
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

	// Release�Լ��� true�� ��ȯ�ϸ� �������ٴ� ���̴�.
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

bool CResourceManager::CreateTextFormat(const string& strName, const TCHAR* pFontName, int iWeight, int iStyle, int iStretch, float fSize, const TCHAR* pLocalName)
{
	IDWriteTextFormat* pFormat = FindFont(strName);

	if (pFormat)
		return false;

	/*
	�ؽ�Ʈ ���̾ƿ��� ���Ǵ� �ؽ�Ʈ �������

	HRESULT CreateTextFormat(1, 2, 3, 4, 5, 6, 7, 8)
	1. WCHAR const* fontFamilyName	-	��Ʈ�� �̸�.
	2. IDWriteFontCollection* fontCollection	- �� ���� nullptr, ex) Arial ��Ʈ�� Arial Baclk ���� �÷����� ������ �ִ�.
	3. DWRITE_FONT_WEIGHT fontWeight	- ��Ʈ�� ����, DWRITE_FONT_WEIGHT ����ü�� �� �� ������ ũ��� ������ ���� �ȴ�.
	4. DWRITE_FONT_STYLE fontStyle	-	��Ʈ ��Ÿ��, DWRITE_FONT_STYLE ����ü / ����, ���, ����� ��Ÿ�ϵ��� �ִ�.
	5. DWRITE_FONT_STRETCH fontStretch	-	���� ����ũ�⿡�� �þ����� ����
	6. FLOAT fontSize	-	���� ũ��
	7. WCHAR const* localeName	- ����� ���� �̸�. �̱��� en-us �̰� �ѱ��� ko-KR �Ϻ��� ja-JP �̸� ������ en-uk �̴�.
	8. IDWriteTextFormat** textFormat	-	����� ����� IDWriteTextFormat ������
	*/

	if (FAILED(m_pWriteFactory->CreateTextFormat(pFontName, nullptr, (DWRITE_FONT_WEIGHT)iWeight,
		(DWRITE_FONT_STYLE)iStyle, (DWRITE_FONT_STRETCH)iStretch, fSize, pLocalName, &pFormat)))
		return false;

	m_mapTextFormat.insert(make_pair(strName, pFormat));

	return true;
}

ID2D1SolidColorBrush* CResourceManager::CreateColor(float r, float g, float b, float a)
{
	ID2D1SolidColorBrush* pBrush = FindBrush(r, g, b, a);

	if (pBrush)
		return pBrush;

	// ������ ���� �⺻ �������� 1.f�� �� �귯���� �����.
	// HRESULT ID2D1RenderTarget::CreateSolidColorBrush(1, 2)
	// 1. const D2D1_COLOR_F &color		- D2D1::ColorF ����ü, �ܼ� r,g,b,a �� ä���ִ� ����ü�̴�.
	// 2. ID2D1SolidColorBrush **solidColorBrush	-	��ȯ ���� �귯���� ������
	if (FAILED(RENDERTARGET2D->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &pBrush)))
		return nullptr;

	m_mapBrush.insert(make_pair(CreateColorKey(r, g, b, a), pBrush));

	return pBrush;
}

ID2D1SolidColorBrush* CResourceManager::CreateColor(BYTE r, BYTE g, BYTE b, BYTE a)
{
	ID2D1SolidColorBrush* pBrush = FindBrush(r, g, b, a);

	if (pBrush)
		return pBrush;

	// ������ ���� �⺻ �������� 1.f�� �� �귯���� �����.
	// HRESULT ID2D1RenderTarget::CreateSolidColorBrush(1, 2)
	// 1. const D2D1_COLOR_F &color		- D2D1::ColorF ����ü, �ܼ� r,g,b,a �� ä���ִ� ����ü�̴�.
	// 2. ID2D1SolidColorBrush **solidColorBrush	-	��ȯ ���� �귯���� ������
	if (FAILED(RENDERTARGET2D->CreateSolidColorBrush(D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f, a / 255.f), &pBrush)))
		return nullptr;

	m_mapBrush.insert(make_pair(CreateColorKey(r, g, b, a), pBrush));

	return pBrush;
}

ID2D1SolidColorBrush* CResourceManager::CreateColor(const Vector4& vColor)
{
	ID2D1SolidColorBrush* pBrush = FindBrush(vColor);

	if (pBrush)
		return pBrush;

	// ������ ���� �⺻ �������� 1.f�� �� �귯���� �����.
	// HRESULT ID2D1RenderTarget::CreateSolidColorBrush(1, 2)
	// 1. const D2D1_COLOR_F &color		- D2D1::ColorF ����ü, �ܼ� r,g,b,a �� ä���ִ� ����ü�̴�.
	// 2. ID2D1SolidColorBrush **solidColorBrush	-	��ȯ ���� �귯���� ������
	if (FAILED(RENDERTARGET2D->CreateSolidColorBrush(D2D1::ColorF(vColor.x, vColor.y, vColor.z, vColor.w), &pBrush)))
		return nullptr;

	m_mapBrush.insert(make_pair(CreateColorKey(vColor), pBrush));

	return pBrush;
}

ID2D1SolidColorBrush* CResourceManager::CreateColor(UINT iColor)
{
	ID2D1SolidColorBrush* pBrush = FindBrush(iColor);

	if (pBrush)
		return pBrush;

	// floatŸ������  iColor��  �и��Ѵ�.
	float	r, g, b, a;

	b = (iColor & 0x000000ff) / 255.f;
	g = ((iColor >> 8) & 0x000000ff) / 255.f;
	r = ((iColor >> 16) & 0x000000ff) / 255.f;
	a = ((iColor >> 24) & 0x000000ff) / 255.f;

	// ������ ���� �⺻ �������� 1.f�� �� �귯���� �����.
	// HRESULT ID2D1RenderTarget::CreateSolidColorBrush(1, 2)
	// 1. const D2D1_COLOR_F &color		- D2D1::ColorF ����ü, �ܼ� r,g,b,a �� ä���ִ� ����ü�̴�.
	// 2. ID2D1SolidColorBrush **solidColorBrush	-	��ȯ ���� �귯���� ������
	if (FAILED(RENDERTARGET2D->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &pBrush)))
		return nullptr;

	m_mapBrush.insert(make_pair(iColor, pBrush));

	return pBrush;
}

ID2D1SolidColorBrush* CResourceManager::FindBrush(float r, float g, float b, float a)
{
	UINT	iKey = CreateColorKey(r, g, b, a);

	auto	iter = m_mapBrush.find(iKey);

	if (iter == m_mapBrush.end())
		return nullptr;

	return iter->second;
}

ID2D1SolidColorBrush* CResourceManager::FindBrush(BYTE r, BYTE g, BYTE b, BYTE a)
{
	UINT	iKey = CreateColorKey(r, g, b, a);

	auto	iter = m_mapBrush.find(iKey);

	if (iter == m_mapBrush.end())
		return nullptr;

	return iter->second;
}

ID2D1SolidColorBrush* CResourceManager::FindBrush(const Vector4& vColor)
{
	UINT	iKey = CreateColorKey(vColor);

	auto	iter = m_mapBrush.find(iKey);

	if (iter == m_mapBrush.end())
		return nullptr;

	return iter->second;
}

ID2D1SolidColorBrush* CResourceManager::FindBrush(UINT iColor)
{
	auto	iter = m_mapBrush.find(iColor);

	if (iter == m_mapBrush.end())
		return nullptr;

	return iter->second;
}

UINT CResourceManager::CreateColorKey(float r, float g, float b, float a)
{
	UINT	iColor = 0;		// 4Byte ������ �����. 1Byte �� A, R, G, B ���� �����Ŵ�.
	BYTE	R, G, B, A;		// ���� ǥ���� ���� 1Byte���� 4���� �����.

	R = (BYTE)(r * 255);	// R = { 0 < R < 1 } �� ���̴�. % ������ ���ϱ� ���� (���� ���� * 255) �� ���ش�.
	G = (BYTE)(g * 255);
	B = (BYTE)(b * 255);
	A = (BYTE)(a * 255);


	// ������ R,G,B,A ���� �ƴ϶� A,R,G,B ������ �� ���̴�.
	// ���� A(����) ���� ��Ʈ�� �־��ش�.
	// ���⼭ �߿��Ѱ� ��Ʈ�� �����ʿ������� ä������.
	// A = 0x3A   �̰�� int�� ����ȯ�Ǽ� 0x0000003A �� �ȴ�.
	// iColor = iColor | A; �� �ص� �Ȱ���
	iColor = A;

	iColor = (iColor << 8) | R;	// ��Ʈ�� 8ĭ(1Byte) �������� �о� �ְ�, �� ���� �� R���� ���� ��Ʈ�� OR ������ �Ͽ� ������ 8ĭ(1Byte)�� ä���ش�.
	iColor = (iColor << 8) | G; // ��Ʈ�� 8ĭ(1Byte) �� �������� �о� �ְ�, �� ���� �� G���� ���� ��Ʈ�� OR ������ �Ͽ� ������ 8ĭ(1Byte)�� ä���ش�.
	iColor = (iColor << 8) | B;

	return iColor;
}

UINT CResourceManager::CreateColorKey(BYTE r, BYTE g, BYTE b, BYTE a)
{
	UINT	iColor = 0;

	iColor = a;

	iColor = (iColor << 8) | r;
	iColor = (iColor << 8) | g;
	iColor = (iColor << 8) | b;

	return iColor;
}

UINT CResourceManager::CreateColorKey(const Vector4& vColor)
{
	UINT	iColor = 0;
	BYTE	r, g, b, a;

	r = (BYTE)(vColor.x * 255);
	g = (BYTE)(vColor.y * 255);
	b = (BYTE)(vColor.z * 255);
	a = (BYTE)(vColor.w * 255);

	iColor = a;

	iColor = (iColor << 8) | r;
	iColor = (iColor << 8) | g;
	iColor = (iColor << 8) | b;

	return iColor;
}

IDWriteTextLayout* CResourceManager::CreateTextLayout(const TCHAR* pText, IDWriteTextFormat* pFormat, float fWidth, float fHeight)
{
	IDWriteTextLayout* pLayout = nullptr;

	// ���ڿ�, �ؽ�Ʈ ���� ����(Layout)�� �����.
	//HRESULT IDWriteFactory::CreateTextLayout(1, 2, 3, 4, 5, 6)

	// 1. WCHAR const *string	-	������ ���ڿ�
	// 2. UINT32 stringLength	-	���ڿ��� ����
	// 3. IDWriteTextFormat *textFormat	-	���ڿ��� ���� �� ������ ��Ÿ���� ��ü�� ���� ������
	// 4. FLOAT maxWidth		-	���ڿ��� ��� ������ �ʺ�
	// 5. FLOAT maxHeight		-	���ڿ��� ��� ������ ����
	// 6. IDWriteTextLayout **textLayout	-	��ȯ ���� ���̾ƿ��� ������

	if (FAILED(m_pWriteFactory->CreateTextLayout(pText, lstrlen(pText), pFormat, fWidth, fHeight, &pLayout)))
		return nullptr;

	return pLayout;
}

IDWriteTextLayout* CResourceManager::CreateTextLayout(const TCHAR* pText, const string& strFormat, float fWidth, float fHeight)
{
	IDWriteTextFormat* pFormat = FindFont(strFormat);

	if (!pFormat)
		return nullptr;

	IDWriteTextLayout* pLayout = nullptr;

	// ���ڿ�, �ؽ�Ʈ ���� ����(Layout)�� �����.
	//HRESULT IDWriteFactory::CreateTextLayout(1, 2, 3, 4, 5, 6)

	// 1. WCHAR const* string	-	������ ���ڿ�
	// 2. UINT32 stringLength	-	���ڿ��� ����
	// 3. IDWriteTextFormat* textFormat	-	���ڿ��� ���� �� ������ ��Ÿ���� ��ü�� ���� ������
	// 4. FLOAT maxWidth	-	���ڿ��� ��� ������ �ʺ�
	// 5. FLOAT maxHeight	-	���ڿ��� ��� ������ ����
	// 6. IDWriteTextLayout** textLayout	-	��ȯ ���� ���̾ƿ��� ������

	if (FAILED(m_pWriteFactory->CreateTextLayout(pText, lstrlen(pText), pFormat, fWidth, fHeight, &pLayout)))
		return nullptr;

	return pLayout;
}

IDWriteTextFormat* CResourceManager::FindFont(const string& strName)
{
	unordered_map<string, IDWriteTextFormat*>::iterator	iter = m_mapTextFormat.find(strName);

	if (iter == m_mapTextFormat.end())
		return nullptr;

	return iter->second;
}

void CResourceManager::InitFMOD()
{
	//FMOD System�� ����
	System_Create(&m_pSystem);

	//FMOD�� �ʱ�ȭ(FMOD�� ä�� ���)
	m_pSystem->init(256, FMOD_INIT_NORMAL, nullptr);

	//ä�� �׷� �̸� ����
	m_pSystem->getMasterChannelGroup(&m_pMasterGroup);

	CreateChannelGroup("BGM");
	CreateChannelGroup("Effect");
	CreateChannelGroup("UI");
}

ID3D11SamplerState* CResourceManager::CreateSampler(D3D11_FILTER eFilter)
{
	//������ Sampler���� Setting
	//--------------------------------------------------------
	D3D11_SAMPLER_DESC	tDesc = {};

	//https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_sampler_desc
	tDesc.Filter = eFilter;								//�ؽ�ó�� ���ø� �� �� ����� ���͸� ���
	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		//���� �ۿ��ִ� U��ǥ�� ���� ó�� ���
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		//���� �ۿ��ִ� V��ǥ�� ���� ó�� ���
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;		
	tDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;		//���ø��� �����͸� ���� ���ø��� �����Ϳ� ���ϴ� �Լ�
	tDesc.MinLOD = -FLT_MAX;							
	tDesc.MaxLOD = FLT_MAX;
	//--------------------------------------------------------

	//Sampler����
	//--------------------------------------------------------
	ID3D11SamplerState* pSampler = nullptr;

	DEVICE->CreateSamplerState(&tDesc, &pSampler);
	//--------------------------------------------------------

	return pSampler;
}
