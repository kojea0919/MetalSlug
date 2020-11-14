#pragma once

#include "Material.h"

class CMaterialInstance : public CMaterial
{
	friend class CResourceManager;
	friend class CMaterial;

protected:
	CMaterialInstance();
	CMaterialInstance(const CMaterialInstance& mtrl);
	~CMaterialInstance();

protected:
	class CMaterial*	m_pOriginMaterial;

public:
	virtual void SetTexture(TEXTURE_LINK eLink, class CTexture* pTexture,
		int iShaderType = (int)CBUFFER_SHADER_TYPE::CBUFFER_VERTEX | (int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL,
		int iRegister = 0);
	virtual void SetTexture(TEXTURE_LINK eLink, const string& strName,
		int iShaderType = (int)CBUFFER_SHADER_TYPE::CBUFFER_VERTEX | (int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL,
		int iRegister = 0);
	virtual void SetTexture(TEXTURE_LINK eLink, const string& strName, const TCHAR* pFileName,
		const string& strPathName,
		int iShaderType = (int)CBUFFER_SHADER_TYPE::CBUFFER_VERTEX | (int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL,
		int iRegister = 0);
	virtual void SetTexture(TEXTURE_LINK eLink, const string& strName, const TCHAR* pFullPath,
		int iShaderType = (int)CBUFFER_SHADER_TYPE::CBUFFER_VERTEX | (int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL,
		int iRegister = 0);

public:
	virtual void SetDiffuseColor(const Vector4& vColor);
	virtual void SetDiffuseColor(float r, float g, float b, float a);

	virtual void SetAmbientColor(const Vector4& vColor);
	virtual void SetAmbientColor(float r, float g, float b, float a);

	virtual void SetSpecularColor(const Vector4& vColor);
	virtual void SetSpecularColor(float r, float g, float b, float a);

	void SetShader(const string& strName);
	void SetMaterial();
	virtual CMaterialInstance* Clone();

	//원래 Material의 값을 복사
	void CopyOriginData();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

