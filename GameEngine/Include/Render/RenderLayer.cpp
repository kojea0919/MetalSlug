#include "RenderLayer.h"
#include "../Component/PrimitiveComponent.h"

CRenderLayer::CRenderLayer()
	: m_iSortOrder(0), m_b2D(false)
{
	m_vecRender.reserve(400);
}

CRenderLayer::~CRenderLayer()
{
}

void CRenderLayer::AddPrimitiveComponent(CPrimitiveComponent* pComponent)
{
	m_vecRender.push_back(pComponent);
}

void CRenderLayer::Render(float fTime)
{
	//2D일 경우 order를 이용해서 sort
	/*if (m_b2D)
	{
		sort(m_vecRender.begin(), m_vecRender.end(), CRenderLayer::SortY);
	}
	else
	{

	}*/

	auto iter = m_vecRender.begin();
	auto iterEnd = m_vecRender.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render(fTime);
	}
}

void CRenderLayer::Clear()
{
	m_vecRender.clear();
}

bool CRenderLayer::SortY(CPrimitiveComponent* pSrc, CPrimitiveComponent* pDest)
{
	Render_Priority eSrcPriority = pSrc->GetRender_Priority();
	Render_Priority eDestPriority = pDest->GetRender_Priority();

	//출력 우선순위가 높은 경우 먼저 출력
	if (eDestPriority < eSrcPriority)
		return false;
	else if (eDestPriority == eSrcPriority)
	{
		//앞에있는게 y값이 작은 경우 뒤에 출력
		return pSrc->GetWorldPos().y < pDest->GetWorldPos().y;
	}
	else
		return true;

}
