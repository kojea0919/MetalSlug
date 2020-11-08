#include "GraphicShader.h"
#include "../PathManager.h"
#include "../Device.h"

CGraphicShader::CGraphicShader()
	: m_pVS(nullptr),m_pVSBlob(nullptr),
	m_pPS(nullptr),m_pPSBlob(nullptr),
	m_pHS(nullptr),m_pHSBlob(nullptr),
	m_pDS(nullptr),m_pDSBlob(nullptr),
	m_pGS(nullptr),m_pGSBlob(nullptr)
{
	m_eType = SHADER_TYPE::GRAPHIC;
}

CGraphicShader::~CGraphicShader()
{
	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pVSBlob);
	SAFE_RELEASE(m_pPS);
	SAFE_RELEASE(m_pPSBlob);
	SAFE_RELEASE(m_pHS);
	SAFE_RELEASE(m_pHSBlob);
	SAFE_RELEASE(m_pDS);
	SAFE_RELEASE(m_pDSBlob);
	SAFE_RELEASE(m_pGS);
	SAFE_RELEASE(m_pGSBlob);
}

bool CGraphicShader::LoadVertexShader(const string& strName, const char* pEntryName, const TCHAR* pFileName, const string& strPathName)
{
	//전에 Load한 Shader Release
	//-------------------------
	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pVSBlob);
	//-------------------------


	UINT iFlag = 0;
#ifdef _DEBUG
	//Debug모드인 경우 셰이더의 문법상 오류를 찾아낼 수 있도록 Flag지정
	iFlag = D3DCOMPILE_DEBUG;
#endif

	//경로 setting
	//-----------------------------------------
	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);
	//-----------------------------------------

	//셰이더 파일을 컴파일
	//-----------------------------------------
	ID3DBlob* pError = nullptr;

	// msdn
	// https://docs.microsoft.com/ko-kr/windows/win32/direct3dhlsl/d3dcompile-constants
	if (FAILED(D3DCompileFromFile(strFullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName, "vs_5_0",
		iFlag, 0, &m_pVSBlob, &pError)))
	{
		OutputDebugStringA((char*)pError->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	// 성공시 m_pVSBlob에는 컴파일된 Shader코드가 들어간다.

	//-----------------------------------------

	if (FAILED(DEVICE->CreateVertexShader(m_pVSBlob->GetBufferPointer(),
		m_pVSBlob->GetBufferSize(), nullptr, &m_pVS)))
		return false;

	return true;
}

bool CGraphicShader::LoadPixelShader(const string& strName, const char* pEntryName, const TCHAR* pFileName, const string& strPathName)
{
	//전에 Load한 Shader Release
		//-------------------------
	SAFE_RELEASE(m_pPS);
	SAFE_RELEASE(m_pPSBlob);
	//-------------------------


	UINT iFlag = 0;
#ifdef _DEBUG
	//Debug모드인 경우 셰이더의 문법상 오류를 찾아낼 수 있도록 Flag지정
	iFlag = D3DCOMPILE_DEBUG;
#endif

	//경로 setting
	//-----------------------------------------
	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);
	//-----------------------------------------

	//셰이더 파일을 컴파일
	//-----------------------------------------
	ID3DBlob* pError = nullptr;

	// msdn
	// https://docs.microsoft.com/ko-kr/windows/win32/direct3dhlsl/d3dcompile-constants
	if (FAILED(D3DCompileFromFile(strFullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName, "ps_5_0",
		iFlag, 0, &m_pPSBlob, &pError)))
	{
		OutputDebugStringA((char*)pError->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	// 성공시 m_pVSBlob에는 컴파일된 Shader코드가 들어간다.

	//-----------------------------------------

	if (FAILED(DEVICE->CreatePixelShader(m_pPSBlob->GetBufferPointer(),
		m_pPSBlob->GetBufferSize(), nullptr, &m_pPS)))
		return false;

	return true;
}

bool CGraphicShader::LoadHullShader(const string& strName, const char* pEntryName, const TCHAR* pFileName, const string& strPathName)
{
	SAFE_RELEASE(m_pHS);
	SAFE_RELEASE(m_pHSBlob);

	UINT	iFlag = 0;
#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	TCHAR	strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	ID3DBlob* pError = nullptr;

	if (FAILED(D3DCompileFromFile(strFullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName, "hs_5_0",
		iFlag, 0, &m_pHSBlob, &pError)))
	{
		OutputDebugStringA((char*)pError->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	if (FAILED(DEVICE->CreateHullShader(m_pHSBlob->GetBufferPointer(),
		m_pHSBlob->GetBufferSize(), nullptr, &m_pHS)))
		return false;

	return true;
}

bool CGraphicShader::LoadDomainShader(const string& strName, const char* pEntryName, const TCHAR* pFileName, const string& strPathName)
{
	SAFE_RELEASE(m_pDS);
	SAFE_RELEASE(m_pDSBlob);

	UINT	iFlag = 0;
#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	TCHAR	strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	ID3DBlob* pError = nullptr;

	if (FAILED(D3DCompileFromFile(strFullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName, "ds_5_0",
		iFlag, 0, &m_pDSBlob, &pError)))
	{
		OutputDebugStringA((char*)pError->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	if (FAILED(DEVICE->CreateDomainShader(m_pDSBlob->GetBufferPointer(),
		m_pDSBlob->GetBufferSize(), nullptr, &m_pDS)))
		return false;

	return true;
}

bool CGraphicShader::LoadGeometryShader(const string& strName, const char* pEntryName, const TCHAR* pFileName, const string& strPathName)
{
	SAFE_RELEASE(m_pGS);
	SAFE_RELEASE(m_pGSBlob);

	UINT	iFlag = 0;
#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	TCHAR	strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	ID3DBlob* pError = nullptr;

	if (FAILED(D3DCompileFromFile(strFullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName, "gs_5_0",
		iFlag, 0, &m_pGSBlob, &pError)))
	{
		OutputDebugStringA((char*)pError->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	if (FAILED(DEVICE->CreateGeometryShader(m_pGSBlob->GetBufferPointer(),
		m_pGSBlob->GetBufferSize(), nullptr, &m_pGS)))
		return false;

	return true;
}

void CGraphicShader::SetShader()
{
	CONTEXT->VSSetShader(m_pVS, nullptr, 0);
	CONTEXT->PSSetShader(m_pPS, nullptr, 0);
	CONTEXT->HSSetShader(m_pHS, nullptr, 0);
	CONTEXT->DSSetShader(m_pDS, nullptr, 0);
	CONTEXT->GSSetShader(m_pGS, nullptr, 0);

	CONTEXT->IASetInputLayout(m_pInputLayout);
}

bool CGraphicShader::CreateInputLayout()
{
	//Input Layout 생성
	if (FAILED(DEVICE->CreateInputLayout(&m_vecInputLayoutDesc[0],
		(UINT)m_vecInputLayoutDesc.size(), m_pVSBlob->GetBufferPointer(),
		m_pVSBlob->GetBufferSize(), &m_pInputLayout)))
		return false;

	return true;
}
