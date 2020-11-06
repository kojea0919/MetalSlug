#pragma once

#include "../GameEngine.h"

class CShaderManager
{
private:
	//Shader���� Map
	unordered_map<string, class CShader*>	m_mapShader;

	//������۰��� Map
	unordered_map<string, PConstantBuffer>	m_mapCBuffer;

public:
	bool Init();

public:
	//Graphic Shader�� Load�Լ� ȣ��
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
	//�ش� �̸��� ���̴� ����ī��Ʈ ����
	void ReleaseShader(const string& strName);

	//���̴� Ž�� �Լ�
	class CShader* FindShader(const string& strName);

public:
	//�Է� Layout
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
	//������� �����Լ�
	//-------------------------------------------------------
	bool CreateCBuffer(const string& strName, int iSize, int iRegister,
		int iShaderType = (int)CBUFFER_SHADER_TYPE::CBUFFER_VERTEX |
	(int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL);
	//-------------------------------------------------------

	//������� �����Լ�
	bool UpdateCBuffer(const string& strName, void* pData);

private:
	//������� Ž�� �Լ�
	PConstantBuffer FindCBuffer(const string& strName);

public:
	//���̴� �����Ͽ� map�� ����
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