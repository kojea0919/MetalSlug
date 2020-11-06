#pragma once

#include "Shader.h"

class CGraphicShader : public CShader
{
	friend class CShaderManager;
	friend class CSceneResource;

protected:
	CGraphicShader();
	virtual ~CGraphicShader();

private:
	//Shader
	//-----------------------
	ID3D11VertexShader* m_pVS;
	ID3DBlob* m_pVSBlob;
	ID3D11PixelShader* m_pPS;
	ID3DBlob* m_pPSBlob;
	ID3D11HullShader* m_pHS;
	ID3DBlob* m_pHSBlob;
	ID3D11DomainShader* m_pDS;
	ID3DBlob* m_pDSBlob;
	ID3D11GeometryShader* m_pGS;
	ID3DBlob* m_pGSBlob;
	//-----------------------

public:
	//Shader Load함수
	//셰이더 파일을 컴파일하고 컴파일된 코드를 저장
	//----------------------------------------------
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
	//----------------------------------------------

public:
	virtual void SetShader();

	//입력 Layout 생성함수
	virtual bool CreateInputLayout();
};