#pragma once

#include "../Ref.h"

class CShader : public CRef
{
	friend class CShaderManager;
	friend class CSceneResource;

protected:
	CShader();
	virtual ~CShader() = 0;

public:
	//Shader 타입(Graphic, Compute)
	SHADER_TYPE m_eType;

	//입력 Layout
	ID3D11InputLayout* m_pInputLayout;

	//정점 버퍼에서 버퍼의 각 속성을 Setting
	vector<D3D11_INPUT_ELEMENT_DESC> m_vecInputLayoutDesc;

	//입력 Layout크기 Setting
	UINT m_iInputLayoutSize;

public:
	//ShaderType Get함수
	SHADER_TYPE GetShaderType() const;

	//D3D11_INPUT_ELEMENT_DESC 추가함수
	bool AddInputLayoutDesc(const char* pSemanticName,
		UINT iSemanticIndex, DXGI_FORMAT eFmt,
		UINT iInputSlot, UINT iSize,
		D3D11_INPUT_CLASSIFICATION eSlotClass,
		UINT iInstanceDataStepRate);

	virtual bool CreateInputLayout();

public:
	virtual void SetShader();
};