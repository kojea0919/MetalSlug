#include "MeshComponent.h"
#include "../Resource/Material.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CMeshComponent::CMeshComponent()
	: m_pMaterial(nullptr)
{
}

CMeshComponent::CMeshComponent(const CMeshComponent& com)
	: CPrimitiveComponent(com)
{
	m_pMaterial = com.m_pMaterial;

	if (m_pMaterial)
		m_pMaterial->AddRef();
}

CMeshComponent::~CMeshComponent()
{
	SAFE_RELEASE(m_pMaterial);
}

void CMeshComponent::SetMaterial(CMaterial* pMaterial)
{
	SAFE_RELEASE(m_pMaterial);
	m_pMaterial = pMaterial;

	if (pMaterial)
		pMaterial->AddRef();
}

CMaterial* CMeshComponent::GetMaterial() const
{
	if (m_pMaterial)
		m_pMaterial->AddRef();

	return m_pMaterial;
}

void CMeshComponent::SetTexture(TEXTURE_LINK eLink, CTexture* pTexture, int iShaderType, int iRegister)
{
	m_pMaterial->SetTexture(eLink, pTexture, iShaderType, iRegister);
}

void CMeshComponent::SetTexture(TEXTURE_LINK eLink, const string& strName, int iShaderType, int iRegister)
{
	m_pMaterial->SetTexture(eLink, strName, iShaderType, iRegister);
}

void CMeshComponent::SetTexture(TEXTURE_LINK eLink, const string& strName, const TCHAR* pFileName, const string& strPathName, int iShaderType, int iRegister)
{
	m_pMaterial->SetTexture(eLink,strName,pFileName,strPathName,
		iShaderType, iRegister);
}

void CMeshComponent::SetTexture(TEXTURE_LINK eLink, const string& strName, const TCHAR* pFullPath, int iShaderType, int iRegister)
{
	m_pMaterial->SetTexture(eLink, strName, pFullPath,
		iShaderType, iRegister);
}

bool CMeshComponent::Init()
{
	if (!CPrimitiveComponent::Init())
		return false;

	return true;
}

void CMeshComponent::Start()
{
	CPrimitiveComponent::Start();
}

void CMeshComponent::Update(float fTime)
{
	CPrimitiveComponent::Update(fTime);
}

void CMeshComponent::PostUpdate(float fTime)
{
	CPrimitiveComponent::PostUpdate(fTime);
}

void CMeshComponent::Collision(float fTime)
{
	CPrimitiveComponent::Collision(fTime);
}

void CMeshComponent::PrevRender(float fTime)
{
	CPrimitiveComponent::PrevRender(fTime);
}

void CMeshComponent::Render(float fTime)
{
	CPrimitiveComponent::Render(fTime);

	//Mesh를 그리기전 Material정보 Setting(Shader Setting, 상수버퍼 Setting)
	if (m_pMaterial)
		m_pMaterial->SetMaterial();
}

void CMeshComponent::PostRender(float fTime)
{
	CPrimitiveComponent::PostRender(fTime);
}

CMeshComponent* CMeshComponent::Clone()
{
	return new CMeshComponent(*this);
}

void CMeshComponent::Save(FILE* pFile)
{
	CPrimitiveComponent::Save(pFile);

	bool bMaterial = false;

	//Material이 있는 경우 Material 정보 저장
	//----------------------------------------
	if (m_pMaterial)
	{
		bMaterial = true;
		fwrite(&bMaterial, sizeof(bool), 1, pFile);

		int iLen = (int)m_pMaterial->GetName().length();
		fwrite(&iLen, sizeof(iLen), 1, pFile);
		fwrite(m_pMaterial->GetName().c_str(), 1, iLen, pFile);
	}
	//----------------------------------------

	else
		fwrite(&bMaterial, sizeof(bool), 1, pFile);
}

void CMeshComponent::Load(FILE* pFile)
{
	CPrimitiveComponent::Load(pFile);

	//Material이 있으면 Material 저장
	//----------------------------------------
	bool bMaterial = false;

	fread(&bMaterial, sizeof(bool), 1, pFile);

	if (bMaterial)
	{
		//이름 read
		//-----------------------------------
		int iLen = 0;
		char strName[256] = {};
		fread(&iLen, sizeof(iLen), 1, pFile);
		fread(strName, 1, iLen, pFile);
		//-----------------------------------

		CMaterial* pMaterial = m_pScene->GetResourceManager()->FindMaterial(strName);

		SetMaterial(pMaterial);

		SAFE_RELEASE(pMaterial);
	}
	//----------------------------------------
}
