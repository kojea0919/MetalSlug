#pragma once

#include "../Ref.h"

//하나의 Texture Resource에 대한 구조체
//------------------------------------
typedef struct _tagTextureResourceInfo
{
	//Load한 Texture
	ScratchImage* pImage;

	//Load Texture에 대한 ShaderResourceView
	ID3D11ShaderResourceView* pSRV;

	//Texture 경로,이름
	//------------------
	TCHAR* pFileName;
	char* pPathName;
	TCHAR* pFullPath;
	//------------------

	//이미지 크기 정보
	//------------------
	unsigned int	iWidth;
	unsigned int	iHeight;
	//------------------

	_tagTextureResourceInfo() :
		pImage(nullptr),
		pSRV(nullptr),
		pFileName(nullptr),
		pPathName(nullptr),
		pFullPath(nullptr),
		iWidth(0),
		iHeight(0)
	{
	}
}TextureResourceInfo, * PTextureResourceInfo;

//------------------------------------

class CTexture : public CRef
{
	friend class CResourceManager;
	friend class CSceneResource;

private:
	CTexture();
	~CTexture();

private:
	vector<PTextureResourceInfo>	m_vecResourceInfo;

	//이미지 타입
	IMAGE_TYPE					m_eImageType;

public:
	//Get함수
	//----------------------------
	unsigned int GetWidth(int iIdx = 0) const
	{
		return m_vecResourceInfo[iIdx]->iWidth;
	}

	unsigned int GetHeight(int iIdx = 0) const
	{
		return m_vecResourceInfo[iIdx]->iHeight;
	}

	IMAGE_TYPE GetImageType() const
	{
		return m_eImageType;
	}

	size_t GetImageCount() const
	{
		return m_vecResourceInfo.size();
	}
	//----------------------------

public:
	//Texture Set함수
	void SetTexture(ID3D11ShaderResourceView* pSRV, int iIdx = 0)
	{
		//비어있는 경우는 생성하여 Setting
		//---------------------------------
		if (m_vecResourceInfo.empty())
		{
			PTextureResourceInfo	pInfo = new TextureResourceInfo;
			pInfo->pSRV = pSRV;
			if (pSRV)
				pSRV->AddRef();

			m_vecResourceInfo.push_back(pInfo);
		}
		//---------------------------------

		else if(m_vecResourceInfo[iIdx])
		{
			SAFE_RELEASE(m_vecResourceInfo[iIdx]->pSRV);
			m_vecResourceInfo[iIdx]->pSRV = pSRV;
			m_vecResourceInfo[iIdx]->pSRV->AddRef();
		}
	}

public:
	//해당 이름의 Texture File Load
	//-------------------------------------------------------------
	bool LoadTexture(const string& strName, const TCHAR* pFileName,
		const string& strPathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const string& strName,
		const TCHAR* pFullPath);
	//-------------------------------------------------------------

	//vector에 들은 모든 Texture File Load
	//-------------------------------------------------------------
	bool LoadTexture(const string& strName,
		const vector<const TCHAR*>& vecFileName,
		const string& strPathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const string& strName,
		const vector<const TCHAR*>& vecFullPath);
	//-------------------------------------------------------------

private:
	//ResourceView 생성함수
	bool CreateResource(int iIdx);

public:
	//iShaderType셰이더에 iRegister로 텍스처 적용
	void SetShader(int iRegister, int iShaderType,int iIdx = 0);
};



