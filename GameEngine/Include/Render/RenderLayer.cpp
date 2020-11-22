#include "RenderLayer.h"
#include "../Component/PrimitiveComponent.h"
#include "../Component/Collider.h"
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
	m_vecColliderRender.reserve(400);
}

CRenderLayer::~CRenderLayer()
{
	SAFE_DELETE_VECLIST(m_RenderInstancingList);
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
			//해당 Component가 사용중인 Mesh와 Material이 같은 RenderInstancing이 있는 경우
			//-------------------------------------------------------------------
			if ((*iter)->CheckMesh(pMesh) && (*iter)->CheckMaterial(pMaterial))
			{
				pInstancing = *iter;
				break;
			}
			//-------------------------------------------------------------------
		}
		//-------------------------------------------------

		//없으면 새로 생성
		//---------------------------------------
		if (!pInstancing)
		{
			pInstancing = new CRenderInstancing;

			if (pComponent->GetSceneComponentType() == SCENECOMPONENT_TYPE::ST_2D)
				pInstancing->Init(pMesh, pMaterial, sizeof(InstancingData2D), "InstancingShader2D");
			else
				pInstancing->Init(pMesh, pMaterial, sizeof(InstancingData), "InstancingShader");

			m_RenderInstancingList.push_back(pInstancing);
		}
		//---------------------------------------

		CCamera* pCamera = GET_SINGLE(CSceneManager)->GetScene()->GetCameraManager()->GetMainCamera();

		//해당 RenderInstancing에 InstancingData정보 추가
		//----------------------------------------------------
		if (pComponent->GetSceneComponentType() == SCENECOMPONENT_TYPE::ST_2D)
		{
			InstancingData2D	tData = {};
			tData.matWVP = pComponent->GetWorldMatrix() * pCamera->GetViewMatrix() * pCamera->GetProjMatrix();
			tData.vMeshPivot = pComponent->GetPivot();
			tData.vMeshSize = pComponent->GetMeshSize();
			tData.vFrameStart = pComponent->GetFrameStart();
			tData.vFrameEnd = pComponent->GetFrameEnd();
			tData.vImageSize = pComponent->GetTextureSize();

			tData.matWVP.Transpose();

			pInstancing->AddInstancingData(&tData);
		}
		else
		{
			InstancingData	tData = {};
			tData.matWVP = pComponent->GetWorldMatrix() * pCamera->GetViewMatrix() * pCamera->GetProjMatrix();
			tData.vMeshPivot = pComponent->GetPivot();
			tData.vMeshSize = pComponent->GetMeshSize();

			tData.matWVP.Transpose();

			pInstancing->AddInstancingData(&tData);
		}
		//----------------------------------------------------


		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pMesh);
	}
	//-------------------------------------------

	//Instancing이 필요없는 경우 m_vecRender에서 처리
	else
		m_vecRender.push_back(pComponent);
}

void CRenderLayer::AddCollider(CCollider* pCollider)
{
	//해당 Collider가 Instancing을 사용하는 경우
	//-------------------------------------------
	if (pCollider->IsInstancing())
	{
		//해당 Collider가 사용하는 CRenderInstancing 탐색
		//-------------------------------------------------
		CMesh* pMesh = pCollider->GetMesh();

		auto	iter = m_RenderInstancingList.begin();
		auto	iterEnd = m_RenderInstancingList.end();
		CRenderInstancing* pInstancing = nullptr;

		for (; iter != iterEnd; ++iter)
		{
			//해당 Collider가 사용중인 Mesh와 같은 RenderInstancing이 있는 경우
			//-------------------------------------------------------------------
			if ((*iter)->CheckMesh(pMesh))
			{
				pInstancing = *iter;
				break;
			}
			//-------------------------------------------------------------------
		}
		//-------------------------------------------------

		//없으면 새로 생성
		//---------------------------------------
		if (!pInstancing)
		{
			pInstancing = new CRenderInstancing;
			
			CMaterial * pMaterial = pCollider->GetMaterial();
			
			pInstancing->Init(pMesh, pMaterial, sizeof(InstancingDataCollider), "InstancingShaderCollider");
			
			SAFE_RELEASE(pMaterial);

			m_RenderInstancingList.push_back(pInstancing);
		}
		//---------------------------------------

		CCamera* pCamera = GET_SINGLE(CSceneManager)->GetScene()->GetCameraManager()->GetMainCamera();

		//해당 RenderInstancing에 InstancingData정보 추가
		//----------------------------------------------------
	
		InstancingDataCollider	tData = {};
		tData.matWVP = pCollider->GetWorldMatrix() * pCamera->GetViewMatrix() * pCamera->GetProjMatrix();
		tData.vMeshPivot = pCollider->GetPivot();
		tData.vMeshSize = pCollider->GetMeshSize();
		tData.bCollision = pCollider->IsCollision();

		tData.matWVP.Transpose();

		pInstancing->AddInstancingData(&tData);
	
		SAFE_RELEASE(pMesh);
		//----------------------------------------------------
	}
	//-------------------------------------------

	//Instancing이 필요없는 경우 m_vecRender에서 처리
	else
		m_vecColliderRender.push_back(pCollider);
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

	{
		auto	iter = m_vecColliderRender.begin();
		auto	iterEnd = m_vecColliderRender.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Render(fTime);
		}
	}
}

void CRenderLayer::Clear()
{
	m_vecRender.clear();
	m_vecColliderRender.clear();
}

bool CRenderLayer::SortY(CPrimitiveComponent* pSrc, CPrimitiveComponent* pDest)
{
	return pSrc->GetWorldPos().y > pDest->GetWorldPos().y;
	//Render_Priority eSrcPriority = pSrc->GetRender_Priority();
	//Render_Priority eDestPriority = pDest->GetRender_Priority();

	////출력 우선순위가 높은 경우 먼저 출력
	//if (eDestPriority < eSrcPriority)
	//	return false;
	//else if (eDestPriority == eSrcPriority)
	//{
	//	//앞에있는게 y값이 작은 경우 뒤에 출력
	//	return pSrc->GetWorldPos().y < pDest->GetWorldPos().y;
	//}
	//else
	//	return true;

}
