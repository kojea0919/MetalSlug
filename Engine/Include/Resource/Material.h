#pragma once

#include "../Ref.h"

class CMaterial : public CRef
{
	friend class CResourceManager;
	friend class CMaterialInstance;

private:
	CMaterial();
	CMaterial(const CMaterial & mtrl);
	virtual ~CMaterial();

private:
	//Shader
	class CGraphicShader*	m_pShader;

	//Material 상수버퍼
	MaterialCBuffer			m_tCBuffer;

	//Texture
	vector<PMaterialTextureInfo>	m_vecTexture;

	bool					m_bStart;
	
	Material_Type			m_eMaterialType;

public:
	Material_Type GetMaterialType() const
	{
		return m_eMaterialType;
	}

	void Start()
	{
		m_bStart = true;
	}

	//Material에 저장된 Texture들중 iFrame번째 Texutre의 크기를 반환
	Vector2 GetDiffuseTextureSize(int iFrame = 0)	const;

public:
	//Texture Settting
	//----------------------------------------------------------------------
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
	//----------------------------------------------------------------------

public:
	//상수버퍼 Setting
	//-----------------------------------
	virtual void SetDiffuseColor(const Vector4& vColor);
	virtual void SetDiffuseColor(float r, float g, float b,float a);

	virtual void SetAmbientColor(const Vector4& vColor);
	virtual void SetAmbientColor(float r, float g, float b, float a);

	virtual void SetSpecularColor(const Vector4& vColor);
	virtual void SetSpecularColor(float r, float g, float b, float a);
	//-----------------------------------

	//Shader Setting
	virtual void SetShader(const string& strName);

	//Material 상수버퍼 적용
	virtual void SetMaterial();

	virtual CMaterial* Clone();

	class CMaterialInstance* CreateMaterialInstance();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};