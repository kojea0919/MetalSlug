#pragma once

#include "PrimitiveComponent.h"

class CMeshComponent : public CPrimitiveComponent
{
	friend class CGameObject;

protected:
	CMeshComponent();
	CMeshComponent(const CMeshComponent & com);
	virtual ~CMeshComponent();

protected:
	//모든 MeshComponent는 각각의 Material이 존재
	class CMaterial* m_pMaterial;

public:
	//Material Set함수
	void SetMaterial(class CMaterial* pMaterial);

	//Material Get함수
	class CMaterial* GetMaterial() const;

public:
	//Texture Set함수
	//---------------------------------------------------
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
	//---------------------------------------------------

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CMeshComponent* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};