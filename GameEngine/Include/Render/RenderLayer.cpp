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
	//�ش� Component�� Instancing�� ����ϴ� ���
	//-------------------------------------------
	if (pComponent->IsInstancing())
	{
		//�ش� Component�� ����ϴ� CRenderInstancing Ž��
		//-------------------------------------------------
		CMesh* pMesh = pComponent->GetMesh();
		CMaterial* pMaterial = pComponent->GetMaterial();

		auto	iter = m_RenderInstancingList.begin();
		auto	iterEnd = m_RenderInstancingList.end();
		CRenderInstancing* pInstancing = nullptr;

		for (; iter != iterEnd; ++iter)
		{
			//�ش� Component�� ������� Mesh�� Material�� ���� RenderInstancing�� �ִ� ���
			//-------------------------------------------------------------------
			if ((*iter)->CheckMesh(pMesh) && (*iter)->CheckMaterial(pMaterial))
			{
				pInstancing = *iter;
				break;
			}
			//-------------------------------------------------------------------
		}
		//-------------------------------------------------

		//������ ���� ����
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

		//�ش� RenderInstancing�� InstancingData���� �߰�
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

	//Instancing�� �ʿ���� ��� m_vecRender���� ó��
	else
		m_vecRender.push_back(pComponent);
}

void CRenderLayer::AddCollider(CCollider* pCollider)
{
	//�ش� Collider�� Instancing�� ����ϴ� ���
	//-------------------------------------------
	if (pCollider->IsInstancing())
	{
		//�ش� Collider�� ����ϴ� CRenderInstancing Ž��
		//-------------------------------------------------
		CMesh* pMesh = pCollider->GetMesh();

		auto	iter = m_RenderInstancingList.begin();
		auto	iterEnd = m_RenderInstancingList.end();
		CRenderInstancing* pInstancing = nullptr;

		for (; iter != iterEnd; ++iter)
		{
			//�ش� Collider�� ������� Mesh�� ���� RenderInstancing�� �ִ� ���
			//-------------------------------------------------------------------
			if ((*iter)->CheckMesh(pMesh))
			{
				pInstancing = *iter;
				break;
			}
			//-------------------------------------------------------------------
		}
		//-------------------------------------------------

		//������ ���� ����
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

		//�ش� RenderInstancing�� InstancingData���� �߰�
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

	//Instancing�� �ʿ���� ��� m_vecRender���� ó��
	else
		m_vecColliderRender.push_back(pCollider);
}

void CRenderLayer::Render(float fTime)
{
	//2D�� ��� order�� �̿��ؼ� sort
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

	////��� �켱������ ���� ��� ���� ���
	//if (eDestPriority < eSrcPriority)
	//	return false;
	//else if (eDestPriority == eSrcPriority)
	//{
	//	//�տ��ִ°� y���� ���� ��� �ڿ� ���
	//	return pSrc->GetWorldPos().y < pDest->GetWorldPos().y;
	//}
	//else
	//	return true;

}
