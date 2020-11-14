#include "MaterialInstance.h"
#include "Material.h"
#include "GraphicShader.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "Texture.h"

CMaterialInstance::CMaterialInstance()
	: m_pOriginMaterial(nullptr)
{
	m_eMaterialType = Material_Type::Instance;
}

CMaterialInstance::CMaterialInstance(const CMaterialInstance& mtrl)
	: CMaterial(mtrl)
{
	m_pOriginMaterial = mtrl.m_pOriginMaterial;

	if (m_pOriginMaterial)
		m_pOriginMaterial->AddRef();
}

CMaterialInstance::~CMaterialInstance()
{
	SAFE_RELEASE(m_pOriginMaterial);
}

void CMaterialInstance::SetTexture(TEXTURE_LINK eLink, CTexture* pTexture, int iShaderType, int iRegister)
{
	PMaterialTextureInfo	pInfo = new MaterialTextureInfo;

	pInfo->eTextureLink = eLink;
	pTexture->AddRef();
	pInfo->pTexture = pTexture;
	pInfo->iRegister = iRegister;
	pInfo->iShaderType = iShaderType;

	switch (eLink)
	{
	case TEXTURE_LINK::DIFFUSE:
		m_tCBuffer.iDifTex = 1;
		pInfo->iRegister = 0;
		break;
	case TEXTURE_LINK::NORMAL:
		m_tCBuffer.iNrmTex = 1;
		pInfo->iRegister = 1;
		break;
	case TEXTURE_LINK::SPECULAR:
		m_tCBuffer.iSpcTex = 1;
		pInfo->iRegister = 2;
		break;
	}

	m_vecTexture.push_back(pInfo);
}

void CMaterialInstance::SetTexture(TEXTURE_LINK eLink, const string& strName, int iShaderType, int iRegister)
{
	CTexture* pTexture = GET_SINGLE(CResourceManager)->FindTexture(strName);

	if (!pTexture)
		return;

	PMaterialTextureInfo	pInfo = new MaterialTextureInfo;

	pInfo->eTextureLink = eLink;
	pInfo->pTexture = pTexture;
	pInfo->iRegister = iRegister;
	pInfo->iShaderType = iShaderType;

	switch (eLink)
	{
	case TEXTURE_LINK::DIFFUSE:
		m_tCBuffer.iDifTex = 1;
		pInfo->iRegister = 0;
		break;
	case TEXTURE_LINK::NORMAL:
		m_tCBuffer.iNrmTex = 1;
		pInfo->iRegister = 1;
		break;
	case TEXTURE_LINK::SPECULAR:
		m_tCBuffer.iSpcTex = 1;
		pInfo->iRegister = 2;
		break;
	}

	m_vecTexture.push_back(pInfo);
}

void CMaterialInstance::SetTexture(TEXTURE_LINK eLink, const string& strName, const TCHAR* pFileName, const string& strPathName, int iShaderType, int iRegister)
{
	GET_SINGLE(CResourceManager)->LoadTexture(strName, pFileName, strPathName);
	CTexture* pTexture = GET_SINGLE(CResourceManager)->FindTexture(strName);

	if (!pTexture)
		return;

	PMaterialTextureInfo	pInfo = new MaterialTextureInfo;

	pInfo->eTextureLink = eLink;
	pInfo->pTexture = pTexture;
	pInfo->iRegister = iRegister;
	pInfo->iShaderType = iShaderType;

	switch (eLink)
	{
	case TEXTURE_LINK::DIFFUSE:
		m_tCBuffer.iDifTex = 1;
		pInfo->iRegister = 0;
		break;
	case TEXTURE_LINK::NORMAL:
		m_tCBuffer.iNrmTex = 1;
		pInfo->iRegister = 1;
		break;
	case TEXTURE_LINK::SPECULAR:
		m_tCBuffer.iSpcTex = 1;
		pInfo->iRegister = 2;
		break;
	}

	m_vecTexture.push_back(pInfo);
}

void CMaterialInstance::SetTexture(TEXTURE_LINK eLink, const string& strName, const TCHAR* pFullPath, int iShaderType, int iRegister)
{
	GET_SINGLE(CResourceManager)->LoadTextureFullPath(strName, pFullPath);
	CTexture* pTexture = GET_SINGLE(CResourceManager)->FindTexture(strName);

	if (!pTexture)
		return;

	PMaterialTextureInfo	pInfo = new MaterialTextureInfo;

	pInfo->eTextureLink = eLink;
	pInfo->pTexture = pTexture;
	pInfo->iRegister = iRegister;
	pInfo->iShaderType = iShaderType;

	switch (eLink)
	{
	case TEXTURE_LINK::DIFFUSE:
		m_tCBuffer.iDifTex = 1;
		pInfo->iRegister = 0;
		break;
	case TEXTURE_LINK::NORMAL:
		pInfo->iRegister = 1;
		m_tCBuffer.iNrmTex = 1;
		break;
	case TEXTURE_LINK::SPECULAR:
		pInfo->iRegister = 2;
		m_tCBuffer.iSpcTex = 1;
		break;
	}

	m_vecTexture.push_back(pInfo);
}

void CMaterialInstance::SetDiffuseColor(const Vector4& vColor)
{
	m_tCBuffer.vDif = vColor;
}

void CMaterialInstance::SetDiffuseColor(float r, float g, float b, float a)
{
	SetDiffuseColor(Vector4(r, g, b, a));
}

void CMaterialInstance::SetAmbientColor(const Vector4& vColor)
{
	m_tCBuffer.vAmb = vColor;
}

void CMaterialInstance::SetAmbientColor(float r, float g, float b, float a)
{
	SetAmbientColor(Vector4(r, g, b, a));
}

void CMaterialInstance::SetSpecularColor(const Vector4& vColor)
{
	m_tCBuffer.vSpc = vColor;
}

void CMaterialInstance::SetSpecularColor(float r, float g, float b, float a)
{
	SetSpecularColor(Vector4(r, g, b, a));
}

void CMaterialInstance::SetShader(const string& strName)
{
}

void CMaterialInstance::SetMaterial()
{
	CMaterial::SetMaterial();
}

CMaterialInstance* CMaterialInstance::Clone()
{
	return new CMaterialInstance(*this);
}

void CMaterialInstance::CopyOriginData()
{
	m_bStart = false;

	SAFE_RELEASE(m_pShader);
	m_pShader = m_pOriginMaterial->m_pShader;

	if (m_pShader)
		m_pShader->AddRef();

	m_vecTexture.clear();
	size_t iSize = m_pOriginMaterial->m_vecTexture.size();

	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		PMaterialTextureInfo pInfo = new MaterialTextureInfo;

		*pInfo = *m_pOriginMaterial->m_vecTexture[iCnt];

		if (pInfo->pTexture)
			pInfo->pTexture->AddRef();

		m_vecTexture.push_back(pInfo);
	}

	m_tCBuffer = m_pOriginMaterial->m_tCBuffer;
}

void CMaterialInstance::Save(FILE* pFile)
{
}

void CMaterialInstance::Load(FILE* pFile)
{
}
