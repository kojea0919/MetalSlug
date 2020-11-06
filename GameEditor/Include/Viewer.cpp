#include "pch.h"
#include "Viewer.h"
#include "EditDevice.h"
#include "PathManager.h"

CViewer::CViewer()
	: m_pDevice(nullptr), m_pInputLayout(nullptr),
	m_pVS(nullptr), m_pVSBlob(nullptr),
	m_pPS(nullptr), m_pPSBlob(nullptr), m_iInputLayoutSize(0),
	m_pCBuffer(nullptr), m_iViewWidth(0), m_iViewHeight(0),
	m_ePrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	m_iUseCnt(6)
{
	memset(m_strFileName, 0, sizeof(TCHAR) * 64);
	memset(m_strVSFuncName, 0, sizeof(char) * 64);
	memset(m_strPSFuncName, 0, sizeof(char) * 64);
}

CViewer::~CViewer()
{
	SAFE_RELEASE(m_tVB.pBuffer);
	SAFE_RELEASE(m_tIB.pBuffer);

	if (m_pCBuffer)
		SAFE_RELEASE(m_pCBuffer->pBuffer);
	SAFE_DELETE(m_pCBuffer);
	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pVSBlob);
	SAFE_RELEASE(m_pPS);
	SAFE_RELEASE(m_pPSBlob);
	SAFE_RELEASE(m_pInputLayout);
}

bool CViewer::Init(CEditDevice* pDevice)
{
	m_pDevice = pDevice;

	//�簢�� Setting
	if (!InitBuffer())
		return false;

	//Shader Setting
	//-----------------------------------
	if (!InitShader())
		return false;

	//�Է� Layout Setting
	//-----------------------------------
	AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputLayoutDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputLayoutDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!CreateInputLayout())
		return false;
	//-----------------------------------

	SetShader();
	//-----------------------------------

	return true;
}

void CViewer::RenderImage()
{
	SetShader();
	CreateCBuffer();

	UINT iStride = m_tVB.iSize;
	UINT iOffset = 0;

	//���� ���� Setting
	m_pDevice->GetContext()->IASetPrimitiveTopology(m_ePrimitiveTopology);

	//https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetvertexbuffers
	m_pDevice->GetContext()->IASetVertexBuffers(0, 1, &m_tVB.pBuffer, &iStride, &iOffset);

	//�ε��� ���۰� �ִ� ���� �Է� �����⿡ Setting
	if (m_tIB.pBuffer)
	{
		m_pDevice->GetContext()->IASetIndexBuffer(m_tIB.pBuffer, m_tIB.eFmt, 0);
		m_pDevice->GetContext()->DrawIndexed(m_tIB.iCount, 0, 0);
	}
	else
	{
		m_pDevice->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		m_pDevice->GetContext()->Draw(m_tVB.iCount, 0);
	}
}

bool CViewer::CreateInputLayout()
{
	if (FAILED(m_pDevice->GetDevice()->CreateInputLayout(&m_vecInputLayoutDesc[0],
		(UINT)m_vecInputLayoutDesc.size(), m_pVSBlob->GetBufferPointer(),
		m_pVSBlob->GetBufferSize(), &m_pInputLayout)))
		return false;

	return true;
}

void CViewer::AddInputLayoutDesc(const char* pSemanticName, UINT iSemanticIndex, DXGI_FORMAT eFmt, UINT iInputSlot, UINT iSize, D3D11_INPUT_CLASSIFICATION eSlotClass, UINT iInstanceDataStepRate)
{
	D3D11_INPUT_ELEMENT_DESC tDesc = {};

	tDesc.SemanticName = pSemanticName;
	tDesc.SemanticIndex = iSemanticIndex;
	tDesc.Format = eFmt;
	tDesc.InputSlot = iInputSlot;

	tDesc.AlignedByteOffset = m_iInputLayoutSize;
	tDesc.InputSlotClass = eSlotClass;
	tDesc.InstanceDataStepRate = iInstanceDataStepRate;

	m_vecInputLayoutDesc.push_back(tDesc);

	m_iInputLayoutSize += iSize;
}

bool CViewer::InitBuffer()
{
	//���� ���� Setting
	//--------------------------------------------
	m_tRect[0].vPos = Vector3(0.f, 1, 0.f);
	m_tRect[0].vUV = Vector2(0.f, 0.f);
	m_tRect[0].vColor = Vector4::Red;

	m_tRect[1].vPos = Vector3(1, 1, 0.f);
	m_tRect[1].vUV = Vector2(1.f, 0.f);
	m_tRect[1].vColor = Vector4::Red;

	m_tRect[2].vPos = Vector3(0.f, 0.f, 0.f);
	m_tRect[2].vUV = Vector2(0.f, 1.f);
	m_tRect[2].vColor = Vector4::Red;

	m_tRect[3].vPos = Vector3(1, 0.f, 0.f);
	m_tRect[3].vUV = Vector2(1.f, 1.f);
	m_tRect[3].vColor = Vector4::Red;
	//--------------------------------------------

	//���� ���� Setting
	//--------------------------------------------
	m_tVB.iSize = sizeof(Vertex2D);
	m_tVB.iCount = 4;

	m_tVB.eUsage = D3D11_USAGE_IMMUTABLE;

	D3D11_BUFFER_DESC tDesc = {};
	tDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tDesc.ByteWidth = m_tVB.iSize * m_tVB.iCount;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tData = {};
	tData.pSysMem = &m_tRect;


	if (FAILED(m_pDevice->GetDevice()->CreateBuffer(&tDesc, &tData, &m_tVB.pBuffer)))
		return false;
	//--------------------------------------------

	//�ε��� ���� Setting
	SetIndexBuffer();

	//�ε��� ���� Setting
	//---------------------------------------
	m_tIB.iSize = 2;
	m_tIB.iCount = m_iUseCnt;
	m_tIB.eUsage = D3D11_USAGE_IMMUTABLE;
	m_tIB.eFmt = DXGI_FORMAT_R16_UINT;

	D3D11_BUFFER_DESC tIdxDesc = {};
	tIdxDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tIdxDesc.ByteWidth = m_tIB.iCount * m_tIB.iSize;
	tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//---------------------------------------
	
	//������ ����
	//---------------------------------------
	D3D11_SUBRESOURCE_DATA tIdxData = {};
	tIdxData.pSysMem = &m_sIdx;

	//---------------------------------------

	//�ε��� ���� ����
	//---------------------------------------
	if (FAILED(m_pDevice->GetDevice()->CreateBuffer(&tIdxDesc, &tIdxData, &m_tIB.pBuffer)))
		return false;

	return true;
}

bool CViewer::InitShader()
{
	SetShaderName();

	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pVSBlob);

	UINT iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(SHADER_PATH);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, m_strFileName);
	//-----------------------------------------

	//���̴� ������ ������
	//-----------------------------------------
	ID3DBlob* pError = nullptr;

	// msdn
	// https://docs.microsoft.com/ko-kr/windows/win32/direct3dhlsl/d3dcompile-constants
	if (FAILED(D3DCompileFromFile(strFullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, m_strVSFuncName, "vs_5_0",
		iFlag, 0, &m_pVSBlob, &pError)))
	{
		OutputDebugStringA((char*)pError->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	// ������ m_pVSBlob���� �����ϵ� Shader�ڵ尡 ����.

	//-----------------------------------------

	if (FAILED(m_pDevice->GetDevice()->CreateVertexShader(m_pVSBlob->GetBufferPointer(),
		m_pVSBlob->GetBufferSize(), nullptr, &m_pVS)))
		return false;

	//���� Load�� Shader Release
	//-------------------------
	SAFE_RELEASE(m_pPS);
	SAFE_RELEASE(m_pPSBlob);
	//-------------------------

	//���̴� ������ ������
	//-----------------------------------------
	// msdn
	// https://docs.microsoft.com/ko-kr/windows/win32/direct3dhlsl/d3dcompile-constants
	if (FAILED(D3DCompileFromFile(strFullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, m_strPSFuncName, "ps_5_0",
		iFlag, 0, &m_pPSBlob, &pError)))
	{
		OutputDebugStringA((char*)pError->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	// ������ m_pVSBlob���� �����ϵ� Shader�ڵ尡 ����.

	//-----------------------------------------

	if (FAILED(m_pDevice->GetDevice()->CreatePixelShader(m_pPSBlob->GetBufferPointer(),
		m_pPSBlob->GetBufferSize(), nullptr, &m_pPS)))
		return false;

	return true;
}

void CViewer::SetShader()
{
	m_pDevice->GetContext()->VSSetShader(m_pVS, nullptr, 0);
	m_pDevice->GetContext()->PSSetShader(m_pPS, nullptr, 0);

	m_pDevice->GetContext()->IASetInputLayout(m_pInputLayout);
}

bool CViewer::CreateCBuffer()
{
	if (m_pCBuffer)
	{
		SAFE_RELEASE(m_pCBuffer->pBuffer);
		SAFE_DELETE(m_pCBuffer);
	}

	m_pCBuffer = new ConstantBuffer;

	m_pCBuffer->iSize = sizeof(ViewerCBuffer);
	m_pCBuffer->iRegister = 0;

	D3D11_BUFFER_DESC tDesc = {};

	//D3D11_USAGE_DYNAMIC : GPU(�б� ����) �� CPU(���� ����)
	//��ο��� ������ �� ���ְ� CPU���� �� �����Ӵ� �ѹ� 
	//������Ʈ�Ǵ� ���ҽ��� �����ϴ�. ������Ʈ�� Map �Լ��� ����ؾ��Ѵ�.
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.ByteWidth = m_pCBuffer->iSize;
	tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//---------------------------------------

	//������� ����
	//---------------------------------------
	if (FAILED(m_pDevice->GetDevice()->CreateBuffer(&tDesc, nullptr, &m_pCBuffer->pBuffer)))
	{
		SAFE_DELETE(m_pCBuffer);
		return false;
	}
	//---------------------------------------

	//������� Setting
	SetCBuffer();

	D3D11_MAPPED_SUBRESOURCE tMap = {};

	m_pDevice->GetContext()->Map(m_pCBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

	//Update
	memcpy(tMap.pData, &m_ViewerCBuffer, m_pCBuffer->iSize);

	m_pDevice->GetContext()->Unmap(m_pCBuffer->pBuffer, 0);

	m_pDevice->GetContext()->VSSetConstantBuffers(m_pCBuffer->iRegister, 1, &m_pCBuffer->pBuffer);

	return true;
}
