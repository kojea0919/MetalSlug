#pragma once

#include "../Ref.h"

//�ϳ��� Texture Resource�� ���� ����ü
//------------------------------------
typedef struct _tagTextureResourceInfo
{
	//Load�� Texture
	ScratchImage* pImage;

	//Load Texture�� ���� ShaderResourceView
	ID3D11ShaderResourceView* pSRV;

	//Texture ���,�̸�
	//------------------
	TCHAR* pFileName;
	char* pPathName;
	TCHAR* pFullPath;
	//------------------

	//�̹��� ũ�� ����
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

	//�̹��� Ÿ��
	IMAGE_TYPE					m_eImageType;

public:
	//Get�Լ�
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
	//Texture Set�Լ�
	void SetTexture(ID3D11ShaderResourceView* pSRV, int iIdx = 0)
	{
		//����ִ� ���� �����Ͽ� Setting
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
	//�ش� �̸��� Texture File Load
	//-------------------------------------------------------------
	bool LoadTexture(const string& strName, const TCHAR* pFileName,
		const string& strPathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const string& strName,
		const TCHAR* pFullPath);
	//-------------------------------------------------------------

	//vector�� ���� ��� Texture File Load
	//-------------------------------------------------------------
	bool LoadTexture(const string& strName,
		const vector<const TCHAR*>& vecFileName,
		const string& strPathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const string& strName,
		const vector<const TCHAR*>& vecFullPath);
	//-------------------------------------------------------------

private:
	//ResourceView �����Լ�
	bool CreateResource(int iIdx);

public:
	//iShaderType���̴��� iRegister�� �ؽ�ó ����
	void SetShader(int iRegister, int iShaderType,int iIdx = 0);
};



