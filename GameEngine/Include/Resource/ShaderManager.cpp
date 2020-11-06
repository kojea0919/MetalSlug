#include "ShaderManager.h"
#include "../Device.h"
#include "Shader.h"
#include "GraphicShader.h"

DEFINITION_SINGLE(CShaderManager)

CShaderManager::CShaderManager()
{

}

CShaderManager::~CShaderManager()
{
	auto iter = m_mapCBuffer.begin();
	auto iterEnd = m_mapCBuffer.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second->pBuffer);
	}

	SAFE_DELETE_MAP(m_mapCBuffer);
	SAFE_RELEASE_MAP(m_mapShader);
}

bool CShaderManager::Init()
{
	//�⺻ Shader Setting
	//-------------------------------------

	//1. Shader����
	CGraphicShader* pShader = CreateShader<CGraphicShader>("Standard2DShader");

	//2. Vertex,Pixel Shader ������
	//-----------------------------
	if (!LoadVertexShader("Standard2DShader", "Standard2DColorVS",
		TEXT("Standard2D.fx")))
		return false;

	if (!LoadPixelShader("Standard2DShader", "Standard2DColorPS",
		TEXT("Standard2D.fx")))
		return false;
	//-----------------------------

	//3. ������ �Է� Layout ����
	//-----------------------------
	pShader->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	pShader->AddInputLayoutDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	pShader->AddInputLayoutDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	//-----------------------------

	//4. �Է� Layout ����
	if (!pShader->CreateInputLayout())
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);
	//-------------------------------------

	//Sprite�� Shader Setting
	//-------------------------------------
	pShader = CreateShader<CGraphicShader>("SpriteShader");

	if (!LoadVertexShader("SpriteShader", "Standard2DSpriteVS",
		TEXT("Standard2D.fx")))
		return false;

	if (!LoadPixelShader("SpriteShader", "Standard2DSpritePS",
		TEXT("Standard2D.fx")))
		return false;

	pShader->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputLayoutDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputLayoutDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShader->CreateInputLayout())
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);
	//-------------------------------------

	//�⺻ Texture Shader
	//-------------------------------------
	pShader = CreateShader<CGraphicShader>("NormalShader");

	if (!LoadVertexShader("NormalShader", "Standard2DTexVS",
		TEXT("Standard2D.fx")))
		return false;

	if (!LoadPixelShader("NormalShader", "Standard2DTexPS",
		TEXT("Standard2D.fx")))
		return false;

	pShader->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputLayoutDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputLayoutDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShader->CreateInputLayout())
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);
	//-------------------------------------

	//Collider�� Shader Setting
	//-------------------------------------
	pShader = CreateShader<CGraphicShader>("ColliderShader");

	if (!LoadVertexShader("ColliderShader", "ColliderVS",
		TEXT("Collider.fx")))
		return false;

	if (!LoadPixelShader("ColliderShader", "ColliderPS",
		TEXT("Collider.fx")))
		return false;

	pShader->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShader->CreateInputLayout())
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);
	//-------------------------------------

	//UI�� Shader Setting
	//-------------------------------------
	pShader = CreateShader<CGraphicShader>("UIShader");

	if (!LoadVertexShader("UIShader", "UITexVS",
		TEXT("UI.fx")))
		return false;

	if (!LoadPixelShader("UIShader", "UITexPS",
		TEXT("UI.fx")))
		return false;

	pShader->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputLayoutDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputLayoutDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShader->CreateInputLayout())
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);
	//-------------------------------------

	//ProgressBar Shader
	//-------------------------------------
	pShader = CreateShader<CGraphicShader>("BarShader");

	if (!LoadVertexShader("BarShader", "UIBarVS",
		TEXT("UI.fx")))
		return false;

	if (!LoadPixelShader("BarShader", "UIBarPS",
		TEXT("UI.fx")))
		return false;

	pShader->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputLayoutDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputLayoutDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShader->CreateInputLayout())
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);
	//-------------------------------------

	//test
	//-------------------------------------
	pShader = CreateShader<CGraphicShader>("RandomColorShader");

	if (!LoadVertexShader("RandomColorShader", "Standard2DSpriteVS",
		TEXT("Standard2D.fx")))
		return false;

	if (!LoadPixelShader("RandomColorShader", "Standard2DRandomColorPS",
		TEXT("Standard2D.fx")))
		return false;

	pShader->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputLayoutDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputLayoutDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShader->CreateInputLayout())
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);
	//-------------------------------------

	//Transform, Material���� ������� ����
	//-------------------------------------
	CreateCBuffer("Transform", sizeof(TransformCBuffer), 0);
	CreateCBuffer("Material", sizeof(MaterialCBuffer), 1,
		(int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL);
	//-------------------------------------

	//Sprite���� ������� ����
	//-------------------------------------
	CreateCBuffer("Sprite", sizeof(SpriteCBuffer), 2,
		(int)CBUFFER_SHADER_TYPE::CBUFFER_VERTEX);
	//-------------------------------------

	//DeltaTime���� ������� ����
	//-------------------------------------
	CreateCBuffer("DeltaTime", sizeof(DeltaTimeCBuffer), 3,
		(int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL);
	//-------------------------------------

	//ProgressBar���� ������� ����
	//-------------------------------------
	CreateCBuffer("Bar", sizeof(BarCBuffer), 10);
	//-------------------------------------

	return true;
}

bool CShaderManager::LoadVertexShader(const string& strName, const char* pEntryName, const TCHAR* pFileName, const string& strPathName)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	//Graphic Shader�� �ƴ� ���� pass
	//--------------------------------------------------------
	else if (pShader->GetShaderType() == SHADER_TYPE::COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}
	//--------------------------------------------------------

	bool bResult = ((CGraphicShader*)pShader)->LoadVertexShader(strName,
		pEntryName, pFileName);

	SAFE_RELEASE(pShader);

	return bResult;
}

bool CShaderManager::LoadPixelShader(const string& strName, const char* pEntryName, const TCHAR* pFileName, const string& strPathName)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	//Graphic Shader�� �ƴ� ���� pass
	//--------------------------------------------------------
	else if (pShader->GetShaderType() == SHADER_TYPE::COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}
	//--------------------------------------------------------

	bool bResult = ((CGraphicShader*)pShader)->LoadPixelShader(strName,
		pEntryName, pFileName);

	SAFE_RELEASE(pShader);

	return bResult;
}

bool CShaderManager::LoadHullShader(const string& strName, const char* pEntryName, const TCHAR* pFileName, const string& strPathName)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	//Graphic Shader�� �ƴ� ���� pass
	//--------------------------------------------------------
	else if (pShader->GetShaderType() == SHADER_TYPE::COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}
	//--------------------------------------------------------

	bool bResult = ((CGraphicShader*)pShader)->LoadHullShader(strName,
		pEntryName, pFileName);

	SAFE_RELEASE(pShader);

	return bResult;
}

bool CShaderManager::LoadDomainShader(const string& strName, const char* pEntryName, const TCHAR* pFileName, const string& strPathName)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	//Graphic Shader�� �ƴ� ���� pass
	//--------------------------------------------------------
	else if (pShader->GetShaderType() == SHADER_TYPE::COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}
	//--------------------------------------------------------

	bool bResult = ((CGraphicShader*)pShader)->LoadDomainShader(strName,
		pEntryName, pFileName);

	SAFE_RELEASE(pShader);

	return bResult;
}

bool CShaderManager::LoadGeometryShader(const string& strName, const char* pEntryName, const TCHAR* pFileName, const string& strPathName)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	//Graphic Shader�� �ƴ� ���� pass
	//--------------------------------------------------------
	else if (pShader->GetShaderType() == SHADER_TYPE::COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}
	//--------------------------------------------------------

	bool bResult = ((CGraphicShader*)pShader)->LoadGeometryShader(strName,
		pEntryName, pFileName);

	SAFE_RELEASE(pShader);

	return bResult;
}

void CShaderManager::ReleaseShader(const string& strName)
{
	auto iter = m_mapShader.find(strName);
	
	if (iter == m_mapShader.end())
		return;

	if (iter->second->Release() == 0)
		m_mapShader.erase(iter);
}

CShader* CShaderManager::FindShader(const string& strName)
{
	auto iter = m_mapShader.find(strName);

	if (iter == m_mapShader.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

bool CShaderManager::AddInputLayoutDesc(const string& strShaderName, const char* pSemanticName, UINT iSemanticIndex, DXGI_FORMAT eFmt, UINT iInputSlot, UINT iSize, D3D11_INPUT_CLASSIFICATION eSlotClass, UINT iInstanceDataStepRate)
{
	CShader* pShader = FindShader(strShaderName);

	if (!pShader)
		return false;

	pShader->AddInputLayoutDesc(pSemanticName, iSemanticIndex, eFmt,
		iInputSlot, iSize, eSlotClass, iInstanceDataStepRate);

	SAFE_RELEASE(pShader);
	
	return true;
}

bool CShaderManager::CreateInputLayout(const string& strShaderName)
{
		CShader* pShader = FindShader(strShaderName);

	if (!pShader)
		return false;

	pShader->CreateInputLayout();

	SAFE_RELEASE(pShader);

	return true;
}

bool CShaderManager::CreateCBuffer(const string& strName, int iSize, int iRegister, int iShaderType)
{
	//�ش� �̸��� ������۰� �̹� �����ϴ� ��� pass
	//-------------------------------------
	PConstantBuffer pBuffer = FindCBuffer(strName);

	if (pBuffer)
		return false;
	//-------------------------------------

	pBuffer = new ConstantBuffer;

	pBuffer->iSize = iSize;
	pBuffer->iRegister = iRegister;
	pBuffer->iShaderType = iShaderType;

	//������ ���� ���� Setting
	//---------------------------------------
	D3D11_BUFFER_DESC tDesc = {};

	//D3D11_USAGE_DYNAMIC : GPU(�б� ����) �� CPU(���� ����)
	//��ο��� ������ �� ���ְ� CPU���� �� �����Ӵ� �ѹ� 
	//������Ʈ�Ǵ� ���ҽ��� �����ϴ�. ������Ʈ�� Map �Լ��� ����ؾ��Ѵ�.
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.ByteWidth = iSize;
	tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//---------------------------------------

	//������� ����
	//---------------------------------------
	if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &pBuffer->pBuffer)))
	{
		SAFE_DELETE(pBuffer);
		return false;
	}
	//---------------------------------------

	m_mapCBuffer.insert(make_pair(strName, pBuffer));

	return true;
}

bool CShaderManager::UpdateCBuffer(const string& strName, void* pData)
{
	PConstantBuffer pBuffer = FindCBuffer(strName);

	if (!pBuffer)
		return false;

	D3D11_MAPPED_SUBRESOURCE tMap = {};

	//Map�Լ�
	//SubResource�� ���Ե� �����Ϳ� ���� �����͸� ��������
	//�ش� ���� ���ҽ��� ���� GPU ������ �ź������μ�
	//������۸� Update�� �� �ִ�.
	//https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-map
	CONTEXT->Map(pBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

	//Update
	memcpy(tMap.pData, pData, pBuffer->iSize);

	CONTEXT->Unmap(pBuffer->pBuffer, 0);

	if (pBuffer->iShaderType & (int)CBUFFER_SHADER_TYPE::CBUFFER_VERTEX)
		CONTEXT->VSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	if (pBuffer->iShaderType & (int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL)
		CONTEXT->PSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	if (pBuffer->iShaderType & (int)CBUFFER_SHADER_TYPE::CBUFFER_DOMAIN)
		CONTEXT->DSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	if (pBuffer->iShaderType & (int)CBUFFER_SHADER_TYPE::CBUFFER_HULL)
		CONTEXT->HSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	if (pBuffer->iShaderType & (int)CBUFFER_SHADER_TYPE::CBUFFER_GEOMETRY)
		CONTEXT->GSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	if (pBuffer->iShaderType & (int)CBUFFER_SHADER_TYPE::CBUFFER_COMPUTE)
		CONTEXT->CSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	return true;
}

PConstantBuffer CShaderManager::FindCBuffer(const string& strName)
{
	auto iter = m_mapCBuffer.find(strName);

	if (iter == m_mapCBuffer.end())
		return nullptr;

	return iter->second;
}
