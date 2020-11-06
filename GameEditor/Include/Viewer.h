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
	//입력 Layout 생성 함수
	bool CreateInputLayout();

	//D3D11_INPUT_DELEMENT_DESC 추가 함수
	void AddInputLayoutDesc(const char* pSemanticName,
		UINT iSemanticIndex, DXGI_FORMAT eFmt,
		UINT iInputSlot, UINT iSize,
		D3D11_INPUT_CLASSIFICATION eSlotClass,
		UINT iInstanceDataStepRate);

protected:
	//정점, 인덱스 버퍼 초기화 함수
	bool InitBuffer();

	virtual void SetIndexBuffer() = 0;

protected:
	//Shader 초기화 함수
	//---------------------------
	virtual bool InitShader();
	virtual void SetShaderName() = 0;
	//---------------------------

	//Shader Setting함수
	void SetShader();

protected:
	//상수버퍼 생성
	bool CreateCBuffer();

	//상수버퍼 Setting
	virtual void SetCBuffer() = 0;

protected:
	CEditDevice* m_pDevice;

	//사각형 정점, 인덱스 버퍼
	//-----------------------------
	Vertex2D		m_tRect[4];
	VertexBuffer	m_tVB;
	IndexBuffer		m_tIB;
	//-----------------------------

	//입력 Layout
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

	//사각형 크기 상수 버퍼
	//-----------------------------
	PConstantBuffer m_pCBuffer;
	ViewerCBuffer	m_ViewerCBuffer;
	//-----------------------------

	//화면 크기
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

	//위상 구조
	D3D_PRIMITIVE_TOPOLOGY m_ePrimitiveTopology;

	//인덱스 버퍼
	//---------------------------
	unsigned short	m_sIdx[20];
	int				m_iUseCnt;
	//---------------------------
};

