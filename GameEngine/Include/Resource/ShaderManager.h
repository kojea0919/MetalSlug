#pragma once

#include "../GameEngine.h"

class CShaderManager
{
private:
	//Shader관리 Map
	unordered_map<string, class CShader*>	m_mapShader;

	//상수버퍼관리 Map
	unordered_map<string, PConstantBuffer>	m_mapCBuffer;

public:
	bool Init();

public:
	//Graphic Shader의 Load함수 호출
	//------------------------------------------------------------------
	bool LoadVertexShader(const string& strName, const char* pEntryName,
		const TCHAR* pFileName, const string& strPathName = SHADER_PATH);
	bool LoadPixelShader(const string& strName, const char* pEntryName,
		const TCHAR* pFileName, const string& strPathName = SHADER_PATH);
	bool LoadHullShader(const string& strName, const char* pEntryName,
		const TCHAR* pFileName, const string& strPathName = SHADER_PATH);
	bool LoadDomainShader(const string& strName, const char* pEntryName,
		const TCHAR* pFileName, const string& strPathName = SHADER_PATH);
	bool LoadGeometryShader(const string& strName, const char* pEntryName,
		const TCHAR* pFileName, const string& strPathName = SHADER_PATH);
	//------------------------------------------------------------------

public:
	//해당 이름의 셰이더 참조카운트 감소
	void ReleaseShader(const string& strName);

	//셰이더 탐색 함수
	class CShader* FindShader(const string& strName);

public:
	//입력 Layout
	//---------------------------------------
	bool AddInputLayoutDesc(const string& strShaderName,
		const char* pSemanticName,
		UINT iSemanticIndex, DXGI_FORMAT eFmt,
		UINT iInputSlot, UINT iSize,
		D3D11_INPUT_CLASSIFICATION eSlotClass,
		UINT iInstanceDataStepRate);

	bool CreateInputLayout(const string& strShaderName);
	//---------------------------------------

public:
	//상수버퍼 생성함수
	//-------------------------------------------------------
	bool CreateCBuffer(const string& strName, int iSize, int iRegister,
		int iShaderType = (int)CBUFFER_SHADER_TYPE::CBUFFER_VERTEX |
	(int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL);
	//-------------------------------------------------------

	//상수버퍼 적용함수
	bool UpdateCBuffer(const string& strName, void* pData);

private:
	//상수버퍼 탐색 함수
	PConstantBuffer FindCBuffer(const string& strName);

public:
	//셰이더 생성하여 map에 저장
	//------------------------------
	template<typename T>
	T* CreateShader(const string& strName)
	{
		T* pShader = (T*)FindShader(strName);

		if (pShader)
		{
			SAFE_RELEASE(pShader);
			return nullptr;
		}

		pShader = new T;

		pShader->SetName(strName);
		pShader->AddRef();
		m_mapShader.insert(make_pair(strName, pShader));

		return pShader;
	}
	//------------------------------

	DECLARE_SINGLE(CShaderManager)
};