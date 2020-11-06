#include "Texture.h"
#include "../PathManager.h"
#include "../Device.h"

CTexture::CTexture()
	: m_eImageType(IMAGE_TYPE::ATLAS)
{
}

CTexture::~CTexture()
{
	for (size_t i = 0; i < m_vecResourceInfo.size(); ++i)
	{
		SAFE_RELEASE(m_vecResourceInfo[i]->pSRV);
		SAFE_DELETE_ARRAY(m_vecResourceInfo[i]->pFileName);
		SAFE_DELETE_ARRAY(m_vecResourceInfo[i]->pPathName);
		SAFE_DELETE_ARRAY(m_vecResourceInfo[i]->pFullPath);
		SAFE_DELETE(m_vecResourceInfo[i]->pImage);
		SAFE_DELETE(m_vecResourceInfo[i]);
	}
}

bool CTexture::LoadTexture(const string& strName, const TCHAR* pFileName, const string& strPathName)
{
	//TextureResourceInfo 생성
	PTextureResourceInfo pInfo = new TextureResourceInfo;

	m_vecResourceInfo.push_back(pInfo);

	//Texture 이름 Setting
	m_strName = strName;

	//ImageType Setting
	m_eImageType = IMAGE_TYPE::ATLAS;

	//경로 setting
	//--------------------------------------------
	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

	TCHAR* pFullPath = new TCHAR[MAX_PATH];
	memset(pFullPath, 0, sizeof(TCHAR) * MAX_PATH);

	if (pPath)
		lstrcpy(pFullPath, pPath);

	lstrcat(pFullPath, pFileName);
	pInfo->pFullPath = pFullPath;
	//--------------------------------------------

	//파일이름,경로 저장
	//--------------------------------------------
	TCHAR* pSaveName = new TCHAR[MAX_PATH];
	memset(pSaveName, 0, sizeof(TCHAR) * MAX_PATH);

	lstrcpy(pSaveName, pFileName);

	pInfo->pFileName = pSaveName;

	char* pPathName = new char[MAX_PATH];
	memset(pPathName, 0, MAX_PATH);

	strcpy_s(pPathName, MAX_PATH,
		strPathName.c_str());
	pInfo->pPathName = pPathName;
	//--------------------------------------------

	//이미지 포맷 Setting
	//--------------------------------------------
	TCHAR strExt[_MAX_EXT] = {};

	_wsplitpath_s(pFullPath, nullptr, 0, nullptr, 0, nullptr, 0,
		strExt, _MAX_EXT);

	//Multibyte 변환
	char _strExt[_MAX_EXT] = {};

	WideCharToMultiByte(CP_ACP, 0, strExt, -1, _strExt,
		lstrlen(strExt), nullptr, nullptr);

	_strupr_s(_strExt);
	//--------------------------------------------


	//이미지 포맷에 따른 Load 함수 호출
	//--------------------------------------------
	ScratchImage* pImage = new ScratchImage;

	if (strcmp(_strExt, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(pFullPath, DDS_FLAGS_NONE, nullptr, *pImage)))
		{
			SAFE_DELETE(pImage);
			return false;
		}
	}

	else if (strcmp(_strExt, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(pFullPath, nullptr, *pImage)))
		{
			SAFE_DELETE(pImage);
			return false;
		}
	}

	else
	{
		if (FAILED(LoadFromWICFile(pFullPath, DDS_FLAGS_NONE, nullptr, *pImage)))
		{
			SAFE_DELETE(pImage);
			return false;
		}
	}
	//--------------------------------------------

	pInfo->pImage = pImage;

	//해당 Texture에 대한 ShaderResourceView생성
	return CreateResource((int)m_vecResourceInfo.size()-1);
}

bool CTexture::LoadTextureFullPath(const string& strName, const TCHAR* pFullPath)
{
	//TextureResourceInfo 생성
	PTextureResourceInfo pInfo = new TextureResourceInfo;

	m_vecResourceInfo.push_back(pInfo);

	//이름 Setting
	m_strName = strName;

	//ImageType Setting
	m_eImageType = IMAGE_TYPE::ATLAS;

	//경로 Setting
	//--------------------------------------------
	TCHAR* pPath = new TCHAR[MAX_PATH];
	memset(pPath, 0, sizeof(TCHAR) * MAX_PATH);

	lstrcpy(pPath, pFullPath);

	pInfo->pFullPath = pPath;
	//--------------------------------------------


	//이미지 포맷 Setting
	//--------------------------------------------
	TCHAR strExt[_MAX_EXT] = {};

	_wsplitpath_s(pFullPath, nullptr, 0, nullptr, 0, nullptr, 0,
		strExt, _MAX_EXT);

	//Multibyte 변환
	char _strExt[_MAX_EXT] = {};

	WideCharToMultiByte(CP_ACP, 0, strExt, -1, _strExt,
		lstrlen(strExt), nullptr, nullptr);

	_strupr_s(_strExt);
	//--------------------------------------------
	
	
	//이미지 포맷에 따른 Load 함수 호출
	//--------------------------------------------
	ScratchImage* pImage = new ScratchImage;

	if (strcmp(_strExt, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(pFullPath, DDS_FLAGS_NONE, nullptr, *pImage)))
		{
			SAFE_DELETE(pImage);
			return false;
		}
	}

	else if (strcmp(_strExt, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(pFullPath, nullptr, *pImage)))
		{
			SAFE_DELETE(pImage);
			return false;
		}
	}

	else
	{
		if (FAILED(LoadFromWICFile(pFullPath, DDS_FLAGS_NONE, nullptr, *pImage)))
		{
			SAFE_DELETE(pImage);
			return false;
		}
	}
	//--------------------------------------------

	pInfo->pImage = pImage;

	return CreateResource((int)m_vecResourceInfo.size() - 1);
}

bool CTexture::LoadTexture(const string& strName, const vector<const TCHAR*>& vecFileName, const string& strPathName)
{
	//이름 Setting
	m_strName = strName;
	
	//ImageType Setting
	m_eImageType = IMAGE_TYPE::ARRAY;

	//vector에 존재하는 모든 Texture File Load
	//---------------------------------------
	size_t iLen = vecFileName.size();
	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
	{
		PTextureResourceInfo pInfo = new TextureResourceInfo;

		m_vecResourceInfo.push_back(pInfo);

		//경로 setting
		//--------------------------------------------
		const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

		TCHAR* pFullPath = new TCHAR[MAX_PATH];
		memset(pFullPath, 0, sizeof(TCHAR) * MAX_PATH);

		if (pPath)
			lstrcpy(pFullPath, pPath);

		lstrcat(pFullPath, vecFileName[iCnt]);
		pInfo->pFullPath = pFullPath;
		//--------------------------------------------

		//파일이름,경로 저장
		//--------------------------------------------
		TCHAR* pSaveName = new TCHAR[MAX_PATH];
		memset(pSaveName, 0, sizeof(TCHAR) * MAX_PATH);

		lstrcpy(pSaveName, vecFileName[iCnt]);

		pInfo->pFileName = pSaveName;

		char* pPathName = new char[MAX_PATH];
		memset(pPathName, 0, MAX_PATH);

		strcpy_s(pPathName, MAX_PATH,
			strPathName.c_str());
		pInfo->pPathName = pPathName;
		//--------------------------------------------

		//이미지 포맷 Setting
		//--------------------------------------------
		TCHAR strExt[_MAX_EXT] = {};

		_wsplitpath_s(pFullPath, nullptr, 0, nullptr, 0, nullptr, 0,
			strExt, _MAX_EXT);

		//Multibyte 변환
		char _strExt[_MAX_EXT] = {};

		WideCharToMultiByte(CP_ACP, 0, strExt, -1, _strExt,
			lstrlen(strExt), nullptr, nullptr);

		_strupr_s(_strExt);
		//--------------------------------------------


		//이미지 포맷에 따른 Load 함수 호출
		//--------------------------------------------
		ScratchImage* pImage = new ScratchImage;

		if (strcmp(_strExt, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(pFullPath, DDS_FLAGS_NONE, nullptr, *pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}

		else if (strcmp(_strExt, ".TGA") == 0)
		{
			if (FAILED(LoadFromTGAFile(pFullPath, nullptr, *pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}

		else
		{
			if (FAILED(LoadFromWICFile(pFullPath, DDS_FLAGS_NONE, nullptr, *pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}
		//--------------------------------------------

		pInfo->pImage = pImage;

		bool bResult = CreateResource((int)iCnt);

		if (!bResult)
			return false;
	}
	//---------------------------------------

	return true;
}

bool CTexture::LoadTextureFullPath(const string& strName, const vector<const TCHAR*>& vecFullPath)
{
	//이름 Setting
	m_strName = strName;

	//ImageType Setting
	m_eImageType = IMAGE_TYPE::ARRAY;

	size_t iLen = vecFullPath.size();
	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
	{
		//TextureResourceInfo 생성
		PTextureResourceInfo pInfo = new TextureResourceInfo;

		m_vecResourceInfo.push_back(pInfo);

		//경로 Setting
		//--------------------------------------------
		TCHAR* pFullPath = new TCHAR[MAX_PATH];
		memset(pFullPath, 0, sizeof(TCHAR) * MAX_PATH);

		lstrcpy(pFullPath, vecFullPath[iCnt]);

		pInfo->pFullPath = pFullPath;
		//--------------------------------------------


		//이미지 포맷 Setting
		//--------------------------------------------
		TCHAR strExt[_MAX_EXT] = {};

		_wsplitpath_s(pFullPath, nullptr, 0, nullptr, 0, nullptr, 0,
			strExt, _MAX_EXT);

		//Multibyte 변환
		char _strExt[_MAX_EXT] = {};

		WideCharToMultiByte(CP_ACP, 0, strExt, -1, _strExt,
			lstrlen(strExt), nullptr, nullptr);

		_strupr_s(_strExt);
		//--------------------------------------------


		//이미지 포맷에 따른 Load 함수 호출
		//--------------------------------------------
		ScratchImage* pImage = new ScratchImage;

		if (strcmp(_strExt, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(pFullPath, DDS_FLAGS_NONE, nullptr, *pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}

		else if (strcmp(_strExt, ".TGA") == 0)
		{
			if (FAILED(LoadFromTGAFile(pFullPath, nullptr, *pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}

		else
		{
			if (FAILED(LoadFromWICFile(pFullPath, DDS_FLAGS_NONE, nullptr, *pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}
		//--------------------------------------------

		pInfo->pImage = pImage;

		bool bResult = CreateResource((int)iCnt);

		if (!bResult)
			return false;
	}

	return true;
}

bool CTexture::CreateResource(int iIdx)
{
	//해당 인덱스의 이미지에 대한 ShaderResourceView생성
	//--------------------------------------------------
	if (FAILED(CreateShaderResourceView(DEVICE,
		m_vecResourceInfo[iIdx]->pImage->GetImages(),
		m_vecResourceInfo[iIdx]->pImage->GetImageCount(),
		m_vecResourceInfo[iIdx]->pImage->GetMetadata(),
		&m_vecResourceInfo[iIdx]->pSRV)))
		return false;
	//--------------------------------------------------

	//이미지 크기 Setting
	//--------------------------------------------------
	m_vecResourceInfo[iIdx]->iWidth = (unsigned int)m_vecResourceInfo[iIdx]->pImage->GetImages()[0].width;
	m_vecResourceInfo[iIdx]->iHeight = (unsigned int)m_vecResourceInfo[iIdx]->pImage->GetImages()[0].height;
	//--------------------------------------------------

	return true;
}

void CTexture::SetShader(int iRegister, int iShaderType,int iIdx)
{
	//각 셰이더에 Texture Bind
	//----------------------------------------

	if (iShaderType & (int)CBUFFER_SHADER_TYPE::CBUFFER_VERTEX)
		CONTEXT->VSSetShaderResources(iRegister, 1, &m_vecResourceInfo[iIdx]->pSRV);

	if (iShaderType & (int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL)
		CONTEXT->PSSetShaderResources(iRegister, 1, &m_vecResourceInfo[iIdx]->pSRV);

	if (iShaderType & (int)CBUFFER_SHADER_TYPE::CBUFFER_DOMAIN)
		CONTEXT->DSSetShaderResources(iRegister, 1, &m_vecResourceInfo[iIdx]->pSRV);

	if (iShaderType & (int)CBUFFER_SHADER_TYPE::CBUFFER_HULL)
		CONTEXT->HSSetShaderResources(iRegister, 1, &m_vecResourceInfo[iIdx]->pSRV);

	if (iShaderType & (int)CBUFFER_SHADER_TYPE::CBUFFER_GEOMETRY)
		CONTEXT->GSSetShaderResources(iRegister, 1, &m_vecResourceInfo[iIdx]->pSRV);

	if (iShaderType & (int)CBUFFER_SHADER_TYPE::CBUFFER_COMPUTE)
		CONTEXT->CSSetShaderResources(iRegister, 1, &m_vecResourceInfo[iIdx]->pSRV);

	//----------------------------------------
}
