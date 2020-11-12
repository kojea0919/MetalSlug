
#include "RenderManager.h"
#include "../Component/SceneComponent.h"
#include "BlendState.h"
#include "DepthStencilState.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()
{
	m_vecRenderList.reserve(400);
	m_vecRender2DList.reserve(400);
}

CRenderManager::~CRenderManager()
{
	SAFE_DELETE_MAP(m_mapRenderState);
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

	return true;
}

void CRenderManager::AddSceneComponent(CSceneComponent* pComponent)
{
	switch (pComponent->GetSceneComponentType())
	{
	case SCENECOMPONENT_TYPE::ST_3D:
		m_vecRenderList.push_back(pComponent);
		break;
	case SCENECOMPONENT_TYPE::ST_2D:
		m_vecRender2DList.push_back(pComponent);
		break;
	}
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
	auto	iter = m_vecRenderList.begin();
	auto	iterEnd = m_vecRenderList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render(fTime);
	}
}

void CRenderManager::Render2D(float fTime)
{
	sort(m_vecRender2DList.begin(), m_vecRender2DList.end(), CRenderManager::SortY);

	SetState("AlphaBlend");

	auto	iter = m_vecRender2DList.begin();
	auto	iterEnd = m_vecRender2DList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render(fTime);
	}

	ResetState("AlphaBlend");
}

void CRenderManager::Clear()
{
	m_vecRenderList.clear();
	m_vecRender2DList.clear();
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

bool CRenderManager::SortY(CSceneComponent* pSrc, CSceneComponent* pDest)
{
	Render_Priority eSrcPriority = pSrc->GetRender_Priority();
	Render_Priority eDestPriority = pDest->GetRender_Priority();

	if (eDestPriority > eSrcPriority)
		return false;
	
	return pSrc->GetWorldPos().y > pDest->GetWorldPos().y;
}