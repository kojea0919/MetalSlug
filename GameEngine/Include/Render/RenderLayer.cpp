#include "RenderLayer.h"
#include "../Component/PrimitiveComponent.h"
#include "RenderInstancing.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh.h"
#include "../Component/Camera.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Scene/CameraManager.h"

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
	//해당 Component가 Instancing을 사용하는 경우
	//-------------------------------------------
	if (pComponent->IsInstancing())
	{
		//해당 Component가 사용하는 CRenderInstancing 탐색
		//-------------------------------------------------
		CMesh* pMesh = pComponent->GetMesh();
		CMaterial* pMaterial = pComponent->GetMaterial();

		auto	iter = m_RenderInstancingList.begin();
		auto	iterEnd = m_RenderInstancingList.end();
		CRenderInstancing* pInstancing = nullptr;

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->CheckMesh(pMesh) && (*iter)->CheckMaterial(pMaterial))
			{
				pInstancing = *iter;
				break;
			}
		}
		//-------------------------------------------------

		//없으면 새로 생성
		//---------------------------------------
		if (!pInstancing)
		{
			pInstancing = new CRenderInstancing;

			pInstancing->Init(pMesh, pMaterial);
		}
		//---------------------------------------

		CCamera* pCamera = GET_SINGLE(CSceneManager)->GetScene()->GetCameraManager()->GetMainCamera();

		InstancingData	tData = {};
		tData.matWVP = pComponent->GetWorldMatrix() * pCamera->GetViewMatrix() * pCamera->GetProjMatrix();
		tData.vMeshPivot = pComponent->GetPivot();
		tData.vMeshSize = pComponent->GetMeshSize();

		tData.matWVP.Transpose();

		pInstancing->AddInstancingData(&tData);

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pMesh);
	}
	//-------------------------------------------

	m_vecRender.push_back(pComponent);
}

void CRenderLayer::Render(float fTime)
{
	//2D일 경우 order를 이용해서 sort
	if (m_b2D)
	{
		sort(m_vecRender.begin(), m_vecRender.end(), CRenderLayer::SortY);
	}
	else
	{

	}


	{
		auto	iter = m_RenderInstancingList.begin();
		auto	iterEnd = m_RenderInstancingList.end();
		CRenderInstancing* pInstancing = nullptr;

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Render(fTime);
			(*iter)->Clear();
		}
	}

	{
		auto	iter = m_vecRender.begin();
		auto	iterEnd = m_vecRender.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Render(fTime);
		}
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
