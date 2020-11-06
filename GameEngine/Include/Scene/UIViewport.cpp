#include "UIViewport.h"
#include "../UI/UIObject.h"
#include "../Render/RenderManager.h"
#include "Scene.h"
#include "SceneCollision.h"


CUIViewport::CUIViewport()
	: m_pScene(nullptr)
{
}

CUIViewport::~CUIViewport()
{
	SAFE_RELEASE_VECLIST(m_UIList);
}

void CUIViewport::AddUI(CUIObject* pUI)
{
	pUI->AddRef();
	pUI->m_pViewport = this;
	m_UIList.push_back(pUI);

}

bool CUIViewport::Init()
{
	return true;
}

void CUIViewport::Start()
{
	auto	iter = m_UIList.begin();
	auto	iterEnd = m_UIList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}
}

void CUIViewport::Update(float fTime)
{
	auto	iter = m_UIList.begin();
	auto	iterEnd = m_UIList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Update(fTime);
	}
}

void CUIViewport::PostUpdate(float fTime)
{
	auto	iter = m_UIList.begin();
	auto	iterEnd = m_UIList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->PostUpdate(fTime);
	}

	m_UIList.sort(CUIViewport::SortUI);
}

void CUIViewport::PrevRender(float fTime)
{
	auto	iter = m_UIList.begin();
	auto	iterEnd = m_UIList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->PrevRender();
	}
}

void CUIViewport::Render(float fTime)
{
	GET_SINGLE(CRenderManager)->SetState("DepthDisable");
	GET_SINGLE(CRenderManager)->SetState("AlphaBlend");

	m_UIList.sort(CUIViewport::SortUI);

	auto	iter = m_UIList.begin();
	auto	iterEnd = m_UIList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render();
	}

	GET_SINGLE(CRenderManager)->ResetState("AlphaBlend");
	GET_SINGLE(CRenderManager)->ResetState("DepthDisable");
}

void CUIViewport::PostRender(float fTime)
{
	auto	iter = m_UIList.begin();
	auto	iterEnd = m_UIList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->PostRender();
	}
}

void CUIViewport::Save(FILE* pFile)
{
}

void CUIViewport::Load(FILE* pFile)
{
}

bool CUIViewport::SortUI(CUIObject* pSrc, CUIObject* pDest)
{
	return pSrc->GetZOrder() < pDest->GetZOrder();
}
