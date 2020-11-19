#include "Material.h"
#include "GraphicShader.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "MaterialInstance.h"

CMaterial::CMaterial()
	: m_pShader(nullptr), m_bStart(false),
	m_eMaterialType(Material_Type::Origin)
{
	memset(&m_tCBuffer, 0, sizeof(m_tCBuffer));
	m_tCBuffer.vDif = Vector4::White;
	m_tCBuffer.vAmb = Vector4::White;
	m_tCBuffer.vSpc = Vector4::White;
}

CMaterial::CMaterial(const CMaterial& mtrl)
	: CRef(mtrl), m_bStart(false)
{
	*this = mtrl;
	m_iRefCount = 1;

	if (m_pShader)
		m_pShader->AddRef();

	//Texture 복사
	//------------------------------------
	m_vecTexture.clear();
	size_t iSize = mtrl.m_vecTexture.size();

	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		PMaterialTextureInfo pInfo = new MaterialTextureInfo;

		*pInfo = *mtrl.m_vecTexture[iCnt];

		if (pInfo->pTexture)
			pInfo->pTexture->AddRef();

		m_vecTexture.push_back(pInfo);
	}
	//------------------------------------
}

CMaterial::~CMaterial()
{
	size_t iSize = m_vecTexture.size();

	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		GET_SINGLE(CResourceManager)->ReleaseTexture(m_vecTexture[iCnt]->pTexture->GetName());
		//SAFE_RELEASE(m_vecTexture[iCnt]->pTexture);
		SAFE_DELETE(m_vecTexture[iCnt]);
	}

	SAFE_RELEASE(m_pShader);
}

Vector2 CMaterial::GetDiffuseTextureSize(int iFrame) const
{
	CTexture* pTexture = m_vecTexture[iFrame]->pTexture;
	return Vector2((float)pTexture->GetWidth(),(float)pTexture->GetHeight());
}

void CMaterial::SetTexture(TEXTURE_LINK eLink, CTexture* pTexture, int iShaderType, int iRegister)
{
	if (m_bStart)
		return;

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

void CMaterial::SetTexture(TEXTURE_LINK eLink, const string& strName, int iShaderType, int iRegister)
{
	if (m_bStart)
		return;

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

void CMaterial::SetTexture(TEXTURE_LINK eLink, const string& strName, const TCHAR* pFileName, const string& strPathName, int iShaderType, int iRegister)
{
	if (m_bStart)
		return;

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

void CMaterial::SetTexture(TEXTURE_LINK eLink, const string& strName, const TCHAR* pFullPath, int iShaderType, int iRegister)
{
	if (m_bStart)
		return;

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

void CMaterial::SetDiffuseColor(const Vector4& vColor)
{
	if (m_bStart)
		return;

	m_tCBuffer.vDif = vColor;
}

void CMaterial::SetDiffuseColor(float r, float g, float b, float a)
{
	if (m_bStart)
		return;

	m_tCBuffer.vDif = Vector4(r, g, b, a);
}

void CMaterial::SetAmbientColor(const Vector4& vColor)
{
	if (m_bStart)
		return;

	m_tCBuffer.vAmb = vColor;
}

void CMaterial::SetAmbientColor(float r, float g, float b, float a)
{
	if (m_bStart)
		return;

	m_tCBuffer.vAmb = Vector4(r, g, b, a);
}

void CMaterial::SetSpecularColor(const Vector4& vColor)
{
	if (m_bStart)
		return;

	m_tCBuffer.vSpc = vColor;
}

void CMaterial::SetSpecularColor(float r, float g, float b, float a)
{
	if (m_bStart)
		return;

	m_tCBuffer.vSpc = Vector4(r, g, b, a);
}

void CMaterial::SetShader(const string& strName)
{
	SAFE_RELEASE(m_pShader);
	m_pShader = (CGraphicShader*)GET_SINGLE(CShaderManager)->FindShader(strName);
}

void CMaterial::SetMaterial()
{
	m_pShader->SetShader();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Material", &m_tCBuffer);

	for (size_t i = 0; i < m_vecTexture.size(); ++i)
	{
		m_vecTexture[i]->pTexture->SetShader(m_vecTexture[i]->iRegister, m_vecTexture[i]->iShaderType);
	}
}

CMaterial* CMaterial::Clone()
{
	return new CMaterial(*this);
}

CMaterialInstance* CMaterial::CreateMaterialInstance()
{
	CMaterialInstance* pInst = new CMaterialInstance;

	pInst->m_pOriginMaterial = this;
	AddRef();

	pInst->CopyOriginData();

	return pInst;
}

void CMaterial::Save(FILE* pFile)
{
	CRef::Save(pFile);

	//Shader 이름 저장
	//------------------------------------------------
	size_t iLen = m_pShader->GetName().length();
	fwrite(&iLen, sizeof(iLen), 1, pFile);
	fwrite(m_pShader->GetName().c_str(), 1, iLen, pFile);
	//------------------------------------------------

	//Diffuse, Ambient, Specu
	//------------------------------------------------
	fwrite(&m_tCBuffer.vDif, sizeof(Vector4), 1, pFile);
	fwrite(&m_tCBuffer.vAmb, sizeof(Vector4), 1, pFile);
	fwrite(&m_tCBuffer.vSpc, sizeof(Vector4), 1, pFile);
	//------------------------------------------------


}

void CMaterial::Load(FILE* pFile)
{
	CRef::Load(pFile);

	//Shader Load
	//------------------------------------------------
	size_t iLen = 0;
	char strName[256] = {};
	fread(&iLen, sizeof(iLen), 1, pFile);
	fread(strName, 1, iLen, pFile);

	SetShader(strName);
	//------------------------------------------------

	//상수 버퍼 Load
	//------------------------------------------------
	fread(&m_tCBuffer.vDif, sizeof(Vector4), 1, pFile);
	fread(&m_tCBuffer.vAmb, sizeof(Vector4), 1, pFile);
	fread(&m_tCBuffer.vSpc, sizeof(Vector4), 1, pFile);
	//------------------------------------------------
}
