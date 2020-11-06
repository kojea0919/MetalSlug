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
	//Shader Ÿ��(Graphic, Compute)
	SHADER_TYPE m_eType;

	//�Է� Layout
	ID3D11InputLayout* m_pInputLayout;

	//���� ���ۿ��� ������ �� �Ӽ��� Setting
	vector<D3D11_INPUT_ELEMENT_DESC> m_vecInputLayoutDesc;

	//�Է� Layoutũ�� Setting
	UINT m_iInputLayoutSize;

public:
	//ShaderType Get�Լ�
	SHADER_TYPE GetShaderType() const;

	//D3D11_INPUT_ELEMENT_DESC �߰��Լ�
	bool AddInputLayoutDesc(const char* pSemanticName,
		UINT iSemanticIndex, DXGI_FORMAT eFmt,
		UINT iInputSlot, UINT iSize,
		D3D11_INPUT_CLASSIFICATION eSlotClass,
		UINT iInstanceDataStepRate);

	virtual bool CreateInputLayout();

public:
	virtual void SetShader();
};