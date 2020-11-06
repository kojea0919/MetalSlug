#include "Mesh2DComponent.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Material.h"
#include "Transform.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"


CMesh2DComponent::CMesh2DComponent()
	: m_pMesh(nullptr)
{
	m_eSceneComponentType = SCENECOMPONENT_TYPE::ST_2D;
	m_eSceneClassType = SCENECOMPONENT_CLASS_TYPE::MESH2D;
}

CMesh2DComponent::CMesh2DComponent(const CMesh2DComponent& com)
	: CMeshComponent(com)
{
	m_eSceneComponentType = SCENECOMPONENT_TYPE::ST_2D;
	m_eSceneClassType = SCENECOMPONENT_CLASS_TYPE::MESH2D;

	m_pMesh = com.m_pMesh;

	if (m_pMesh)
		m_pMesh->AddRef();
}

CMesh2DComponent::~CMesh2DComponent()
{
	SAFE_RELEASE(m_pMesh);
}

CMesh2D* CMesh2DComponent::GetMesh() const
{
	if (m_pMesh)
		m_pMesh->AddRef();

	return m_pMesh;
}

void CMesh2DComponent::SetMesh(CMesh2D* pMesh)
{
	SAFE_RELEASE(m_pMesh);

	m_pMesh = pMesh;

	if (pMesh)
	{
		pMesh->AddRef();

		//Material 복사
		//---------------------------------------------
		CMaterial* pMaterial = m_pMesh->GetMaterial();
		if (!pMaterial)
			return;

		CMaterial* pClone = pMaterial->Clone();

		SetMaterial(pClone);

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pClone);
		//---------------------------------------------

		m_pTransform->SetMeshSize(m_pMesh->GetMax() - m_pMesh->GetMin());
	}
}

void CMesh2DComponent::SetMesh(const string& strMeshName)
{
	//해당 이름을 가진 Mesh를 ResourceManager에서 찾아서 Setting
	//------------------------------------------------------
	SAFE_RELEASE(m_pMesh);
	m_pMesh = (CMesh2D*)m_pScene->GetResourceManager()->FindMesh(strMeshName);
	//------------------------------------------------------
	
	if (m_pMesh)
	{
		//Material 복사
		//---------------------------------------------
		CMaterial* pMaterial = m_pMesh->GetMaterial();
		if (!pMaterial)
			return;

		CMaterial* pClone = pMaterial->Clone();

		SetMaterial(pClone);

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pClone);
		//---------------------------------------------

		m_pTransform->SetMeshSize(m_pMesh->GetMax() - m_pMesh->GetMin());
	}
}

bool CMesh2DComponent::Init()
{
	if (!CMeshComponent::Init())
		return false;

	//2DComponent는 2D사각형을 기본으로 Setting
	//------------------------------------------
	CMesh* pMesh = m_pScene->GetResourceManager()->GetDefault2DMesh();
	SetMesh((CMesh2D*)pMesh);
	//------------------------------------------

	SAFE_RELEASE(pMesh);

	return true;
}

void CMesh2DComponent::Start()
{
	CMeshComponent::Start();
}

void CMesh2DComponent::Update(float fTime)
{
	CMeshComponent::Update(fTime);
}

void CMesh2DComponent::PostUpdate(float fTime)
{
	CMeshComponent::PostUpdate(fTime);
}

void CMesh2DComponent::Collision(float fTime)
{
	CMeshComponent::Collision(fTime);
}

void CMesh2DComponent::PrevRender(float fTime)
{
	CMeshComponent::PrevRender(fTime);
}

void CMesh2DComponent::Render(float fTime)
{
	CMeshComponent::Render(fTime);

	m_pMesh->Render(fTime);
}

void CMesh2DComponent::PostRender(float fTime)
{
	CMeshComponent::PostRender(fTime);
}

CMesh2DComponent* CMesh2DComponent::Clone()
{
	return new CMesh2DComponent(*this);
}

void CMesh2DComponent::Save(FILE* pFile)
{
	CMeshComponent::Save(pFile);
}

void CMesh2DComponent::Load(FILE* pFile)
{
	CMeshComponent::Load(pFile);

	CMesh* pMesh = m_pScene->GetResourceManager()->GetDefault2DMesh();
	SetMesh((CMesh2D*)pMesh);

	SAFE_RELEASE(pMesh);
}
