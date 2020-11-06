#pragma once

#include "PrimitiveComponent.h"

//SpriteInfo 구조체 
//애니메이션 하나에 대한 정보를 가지고 있는 구조체
//------------------------------------------------
typedef struct _tagSpriteInfo
{
	class CAnimation2DSequence* pAnimation;
	float			fPlayRate;
	float			fPlayTime;
	int				iFrame;
	bool			bLoop;
	function<void()>	FinishCallback;

	_tagSpriteInfo() :
		fPlayRate(1.f),
		fPlayTime(0.f),
		iFrame(0),
		bLoop(true)
	{
	}
}SpriteInfo, * PSpriteInfo;
//------------------------------------------------

class CSpriteComponent : public CPrimitiveComponent
{
	friend class CGameObject;

protected:
	CSpriteComponent();
	CSpriteComponent(const CSpriteComponent& com);
	virtual ~CSpriteComponent();

protected:
	class CMesh2D* m_pMesh;
	class CMaterial* m_pMaterial;

	unordered_map<string, PSpriteInfo>	m_mapSpriteInfo;

	//현재 실행중인 애니메이션
	PSpriteInfo		m_pCurrent;

	//Sprite상수 버퍼
	SpriteCBuffer	m_tCBuffer;

	//scale
	float			m_fScale;

public:
	void SetScale(float fScale)
	{
		m_fScale = fScale;
	}

public:
	void SetMaterial(class CMaterial* pMaterial);
	class CMaterial* GetMaterial() const;

public:
	//Texture관리 함수
	//--------------------------------------------
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
	//--------------------------------------------

	
	//Sprite관리 함수
	//--------------------------------------------
	void AddSpriteInfo(const string& strName,
		const string& strAnimName,
		bool bLoop, float fPlayRate = 1.f);
	void AddSpriteInfo(const string& strName,
		class CAnimation2DSequence* pSequence,
		bool bLoop, float fPlayRate = 1.f);
	void AddSpriteInfo(const string& strName, const string& strSequenceName);

	void ChangeSprite(const string& strName);

	void CallNotify(const string& strName);

	template <typename T>
	void SetFinishCallback(const string& strName,
		T* pObj, void (T::* pFunc)())
	{
		PSpriteInfo	pInfo = FindSpriteInfo(strName);

		if (!pInfo)
			return;

		pInfo->FinishCallback = bind(pFunc, pObj);
	}

private:
	PSpriteInfo FindSpriteInfo(const string& strName);
	//--------------------------------------------


public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CSpriteComponent* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};


