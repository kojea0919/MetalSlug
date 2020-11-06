#pragma once

#include "../Ref.h"

//Texture정보 구조체
//------------------------------------------
typedef struct _tagMaterialTextureInfo
{
	TEXTURE_LINK	eTextureLink;	//텍스처 종류
	class CTexture* pTexture;		//사용할 텍스처
	int				iRegister;		//사용할 레지스터
	int				iShaderType;	//적용할 셰이더 타입
}MaterialTextureInfo, * PMaterialTextureInfo;
//------------------------------------------

class CMaterial : public CRef
{
	friend class CResourceManager;

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

public:
	//Texture Settting
	//----------------------------------------------------------------------
	void SetTexture(TEXTURE_LINK eLink, class CTexture* pTexture,
		int iShaderType = (int)CBUFFER_SHADER_TYPE::CBUFFER_VERTEX | (int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL,
		int iRegister = 0);
	void SetTexture(TEXTURE_LINK eLink, const string& strName,
		int iShaderType = (int)CBUFFER_SHADER_TYPE::CBUFFER_VERTEX | (int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL,
		int iRegister = 0);
	void SetTexture(TEXTURE_LINK eLink, const string& strName, const TCHAR* pFileName,
		const string& strPathName,
		int iShaderType = (int)CBUFFER_SHADER_TYPE::CBUFFER_VERTEX | (int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL,
		int iRegister = 0);
	void SetTexture(TEXTURE_LINK eLink, const string& strName, const TCHAR* pFullPath,
		int iShaderType = (int)CBUFFER_SHADER_TYPE::CBUFFER_VERTEX | (int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL,
		int iRegister = 0);
	//----------------------------------------------------------------------

public:
	//상수버퍼 Setting
	//-----------------------------------
	void SetDiffuseColor(const Vector4& vColor);
	void SetDiffuseColor(float r, float g, float b,float a);

	void SetAmbientColor(const Vector4& vColor);
	void SetAmbientColor(float r, float g, float b, float a);

	void SetSpecularColor(const Vector4& vColor);
	void SetSpecularColor(float r, float g, float b, float a);
	//-----------------------------------

	//Shader Setting
	void SetShader(const string& strName);

	//Material 상수버퍼 적용
	void SetMaterial();

	CMaterial* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};