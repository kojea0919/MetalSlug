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
	//2D�� ��� order�� �̿��ؼ� sort
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

	//��� �켱������ ���� ��� ���� ���
	if (eDestPriority < eSrcPriority)
		return false;
	else if (eDestPriority == eSrcPriority)
	{
		//�տ��ִ°� y���� ���� ��� �ڿ� ���
		return pSrc->GetWorldPos().y < pDest->GetWorldPos().y;
	}
	else
		return true;

}
