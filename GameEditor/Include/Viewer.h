#pragma once

#include "Resource/Mesh.h"
#include "GameEngine.h"

typedef struct _tagViewerCBuffer
{
	Matrix	matWVP;
}ViewerCBuffer, * PViewerCBuffer;

class CViewer
{
protected:
	CViewer();
	virtual ~CViewer() = 0;

public:
	//Device Init
	bool Init(class CEditDevice* pDevice);

public:
	//Render
	void RenderImage();

public:
	void SetViewSize(int iWidth, int iHeight)
	{
		m_iViewWidth = iWidth;
		m_iViewHeight = iHeight;
	}

protected:
	//�Է� Layout ���� �Լ�
	bool CreateInputLayout();

	//D3D11_INPUT_DELEMENT_DESC �߰� �Լ�
	void AddInputLayoutDesc(const char* pSemanticName,
		UINT iSemanticIndex, DXGI_FORMAT eFmt,
		UINT iInputSlot, UINT iSize,
		D3D11_INPUT_CLASSIFICATION eSlotClass,
		UINT iInstanceDataStepRate);

protected:
	//����, �ε��� ���� �ʱ�ȭ �Լ�
	bool InitBuffer();

	virtual void SetIndexBuffer() = 0;

protected:
	//Shader �ʱ�ȭ �Լ�
	//---------------------------
	virtual bool InitShader();
	virtual void SetShaderName() = 0;
	//---------------------------

	//Shader Setting�Լ�
	void SetShader();

protected:
	//������� ����
	bool CreateCBuffer();

	//������� Setting
	virtual void SetCBuffer() = 0;

protected:
	CEditDevice* m_pDevice;

	//�簢�� ����, �ε��� ����
	//-----------------------------
	Vertex2D		m_tRect[4];
	VertexBuffer	m_tVB;
	IndexBuffer		m_tIB;
	//-----------------------------

	//�Է� Layout
	//-----------------------------
	ID3D11InputLayout* m_pInputLayout;
	vector<D3D11_INPUT_ELEMENT_DESC>	m_vecInputLayoutDesc;
	UINT				m_iInputLayoutSize;
	//-----------------------------

	//Shader
	//-----------------------------
	ID3D11VertexShader* m_pVS;
	ID3DBlob* m_pVSBlob;
	ID3D11PixelShader* m_pPS;
	ID3DBlob* m_pPSBlob;
	//-----------------------------

	//�簢�� ũ�� ��� ����
	//-----------------------------
	PConstantBuffer m_pCBuffer;
	ViewerCBuffer	m_ViewerCBuffer;
	//-----------------------------

	//ȭ�� ũ��
	//-----------------------------
	int				m_iViewWidth;
	int				m_iViewHeight;
	//-----------------------------

	//ShaderName
	//-----------------------------
	TCHAR			m_strFileName[64];
	char			m_strVSFuncName[64];
	char			m_strPSFuncName[64];
	//-----------------------------

	//���� ����
	D3D_PRIMITIVE_TOPOLOGY m_ePrimitiveTopology;

	//�ε��� ����
	//---------------------------
	unsigned short	m_sIdx[20];
	int				m_iUseCnt;
	//---------------------------
};

