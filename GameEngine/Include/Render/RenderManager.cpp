
#include "RenderManager.h"
#include "../Component/SceneComponent.h"
#include "../Component/PrimitiveComponent.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "RenderLayer.h"
#include "../Component/Collider.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()
{
	m_vecRenderCollider.reserve(300);
}

CRenderManager::~CRenderManager()
{
	SAFE_DELETE_VECLIST(m_vecRenderLayer);
	SAFE_DELETE_VECLIST(m_vecRenderLayer2D);
	SAFE_DELETE_MAP(m_mapRenderState);
}

bool CRenderManager::CreateLayer(const string& strName, int iSortOrder)
{
	size_t iSize = m_vecRenderLayer.size();

	//해당 이름의 Layer가 있는 경우는 pass
	//-------------------------------------------------
	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		if (m_vecRenderLayer[iCnt]->GetName() == strName)
			return false;
	}
	//-------------------------------------------------

	//Layer생성
	//-------------------------------------------------
	CRenderLayer* pLayer = new CRenderLayer;

	pLayer->SetName(strName);
	pLayer->SetSortOrder(iSortOrder);

	m_vecRenderLayer.push_back(pLayer);
	//-------------------------------------------------

	sort(m_vecRenderLayer.begin(), m_vecRenderLayer.end(), CRenderManager::SortLayer);

	return true;
}

bool CRenderManager::CreateLayer2D(const string& strName, int iSortOrder)
{
	size_t	iSize = m_vecRenderLayer2D.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (m_vecRenderLayer2D[i]->GetName() == strName)
			return false;
	}

	CRenderLayer* pLayer = new CRenderLayer;

	pLayer->SetName(strName);
	pLayer->SetSortOrder(iSortOrder);
	pLayer->Set2D();

	m_vecRenderLayer2D.push_back(pLayer);

	sort(m_vecRenderLayer2D.begin(), m_vecRenderLayer2D.end(), CRenderManager::SortLayer);

	return true;
}

bool CRenderManager::Init()
{
	//Alpha Blending
	//-----------------------------
	AddBlendInfo("AlphaBlend");
	CreateBlendState("AlphaBlend");
	//-----------------------------

	// 깊어버퍼를 사용안하는 2D 용 상태를 만든다.
	CreateDepthStencil("DepthDisable", false);

	// Default Layer 생성
	CreateLayer("Collider", 2);
	CreateLayer("Default", 1);
	CreateLayer("Back");

	CreateLayer2D("Collider", 2);
	CreateLayer2D("Default", 1);
	CreateLayer2D("Back");

	return true;
}

void CRenderManager::AddSceneComponent(CPrimitiveComponent* pComponent)
{
	size_t iSize = m_vecRenderLayer.size();

	//Component가 속한 layer를 찾아서 Add
	//--------------------------------------------------
	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		switch (pComponent->GetSceneComponentType())
		{
		case SCENECOMPONENT_TYPE::ST_3D:
			if (m_vecRenderLayer[iCnt]->GetName() == pComponent->GetLayerName())
			{
				m_vecRenderLayer[iCnt]->AddPrimitiveComponent(pComponent);
				break;
			}
			break;
		case SCENECOMPONENT_TYPE::ST_2D:
			if (m_vecRenderLayer2D[iCnt]->GetName() == pComponent->GetLayerName())
			{
				m_vecRenderLayer2D[iCnt]->AddPrimitiveComponent(pComponent);
				break;
			}
			break;
		}
	}
	//--------------------------------------------------
}

void CRenderManager::AddCollider(CCollider* pCollider)
{
	m_vecRenderCollider.push_back(pCollider);
}

void CRenderManager::Render(float fTime)
{
	Render3D(fTime);

	//SetState("DepthDisable");

	Render2D(fTime);

	//ResetState("DepthDisable");
}

void CRenderManager::Render3D(float fTime)
{
	//Primitive Component Redner
	//-----------------------------------------
	{
		auto	iter = m_vecRenderLayer.begin();
		auto	iterEnd = m_vecRenderLayer.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Render(fTime);
		}
	}
	//-----------------------------------------

	//Collider Render
	//-----------------------------------------
	/*{
		auto	iter = m_vecRenderCollider.begin();
		auto	iterEnd = m_vecRenderCollider.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Render(fTime);
		}
	}*/
	//-----------------------------------------
}

void CRenderManager::Render2D(float fTime)
{
	//SetState("AlphaBlend");
	{
		auto	iter = m_vecRenderLayer2D.begin();
		auto	iterEnd = m_vecRenderLayer2D.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Render(fTime);
		}
	}

	//SetState("DepthDisable");
	// 충돌체를 그려낸다.
	{
		auto	iter = m_vecRenderCollider.begin();
		auto	iterEnd = m_vecRenderCollider.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Render(fTime);
		}
	}

	//ResetState("DepthDisable");

	//ResetState("AlphaBlend");
}

void CRenderManager::Clear()
{
	{
		auto	iter = m_vecRenderLayer2D.begin();
		auto	iterEnd = m_vecRenderLayer2D.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Clear();
		}
	}


	{
		auto	iter = m_vecRenderLayer.begin();
		auto	iterEnd = m_vecRenderLayer.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Clear();
		}
	}


	m_vecRenderCollider.clear();
}

bool CRenderManager::AddBlendInfo(const string& strName, bool bEnable, D3D11_BLEND eSrcBlend, D3D11_BLEND eDestBlend, D3D11_BLEND_OP eBlendOp, D3D11_BLEND eSrcBlendAlpha, D3D11_BLEND eDestBlendAlpha, D3D11_BLEND_OP eBlendAlphaOp, UINT8 iWriteMask)
{
	CBlendState* pState = (CBlendState*)FindRenderState(strName);

	if (!pState)
	{
		pState = new CBlendState;
		m_mapRenderState.insert(make_pair(strName, pState));
	}

	pState->AddBlendInfo(bEnable, eSrcBlend, eDestBlend,
		eBlendOp, eSrcBlendAlpha, eDestBlendAlpha,
		eBlendAlphaOp, iWriteMask);

	return true;
}

bool CRenderManager::CreateBlendState(const string& strName, bool bAlphaCover, bool bIndependent)
{
	CBlendState* pState = (CBlendState*)FindRenderState(strName);

	if (!pState)
		return false;

	if (!pState->CreateState(bAlphaCover, bIndependent))
	{
		auto	iter = m_mapRenderState.find(strName);
		m_mapRenderState.erase(iter);
		SAFE_DELETE(pState);
		return false;
	}

	return true;
}

bool CRenderManager::CreateDepthStencil(const string& strName, bool bDepthEnable, D3D11_DEPTH_WRITE_MASK eDepthWrite, D3D11_COMPARISON_FUNC eDepthFunc, bool bStencilEnable, UINT8 iStencilReadMask, UINT8 iStencilWriteMask, const D3D11_DEPTH_STENCILOP_DESC& tFrontFace, const D3D11_DEPTH_STENCILOP_DESC& tBackFace)
{
	CDepthStencilState* pState = (CDepthStencilState*)FindRenderState(strName);

	if (pState)
		return true;

	pState = new CDepthStencilState;

	if (!pState->CreateDepthStencil(bDepthEnable, eDepthWrite, eDepthFunc,
		bStencilEnable, iStencilReadMask, iStencilWriteMask, tFrontFace,
		tBackFace))
	{
		SAFE_DELETE(pState);
		return false;
	}

	m_mapRenderState.insert(make_pair(strName, pState));

	return true;
}

bool CRenderManager::SetState(const string& strName)
{
	CRenderState* pState = FindRenderState(strName);

	if (!pState)
		return false;

	pState->SetState();

	return true;
}

bool CRenderManager::ResetState(const string& strName)
{
	CRenderState* pState = FindRenderState(strName);

	if (!pState)
		return false;

	pState->ResetState();

	return true;
}

CRenderState* CRenderManager::FindRenderState(const string& strName)
{
	auto	iter = m_mapRenderState.find(strName);

	if (iter == m_mapRenderState.end())
		return nullptr;

	return iter->second;
}

bool CRenderManager::SortLayer(CRenderLayer* pSrc, CRenderLayer* pDest)
{	
	return pSrc->GetSortOrder() < pDest->GetSortOrder();
}