#pragma once

#include "SceneComponent.h"

class CPrimitiveComponent : public CSceneComponent
{
	friend class CGameObject;

protected:
	CPrimitiveComponent();
	CPrimitiveComponent(const CPrimitiveComponent& com);
	virtual ~CPrimitiveComponent();

protected:
	string		m_strLayer;

	//Instancing을 활용할지 나타내는 bool변수
	bool		m_bInstancing;

protected:
	class CMaterial* m_pMaterial;
	class CMesh* m_pMesh;

public:
	//Instancing이 되는 경우 InstancingData에 추가하기위해
	//필요한 Get함수
	//------------------------------------
	virtual Vector2 GetFrameStart() const;
	virtual Vector2 GetFrameEnd() const;
	virtual Vector2 GetTextureSize() const;
	//------------------------------------

public:
	void SetMaterial(class CMaterial* pMaterial);
	void SetMaterial(const string& strName);
	class CMaterial* GetMaterial()	const;

public:
	class CMesh* GetMesh()	const;
	void SetMesh(class CMesh* pMesh);
	void SetMesh(const string& strMeshName);

public:
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

public:
	string GetLayerName() const
	{
		return m_strLayer;
	}

	void SetLayer(const string& strName)
	{
		m_strLayer = strName;
	}

	bool IsInstancing()	const
	{
		return m_bInstancing;
	}

	void SetInstancing(bool bInstancing = true)
	{
		m_bInstancing = bInstancing;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CPrimitiveComponent* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

};

