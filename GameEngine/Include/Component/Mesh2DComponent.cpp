#include "Mesh2DComponent.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Material.h"
#include "Transform.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"


CMesh2DComponent::CMesh2DComponent()
{
	m_eSceneComponentType = SCENECOMPONENT_TYPE::ST_2D;
	m_eSceneClassType = SCENECOMPONENT_CLASS_TYPE::MESH2D;
}

CMesh2DComponent::CMesh2DComponent(const CMesh2DComponent& com)
	: CPrimitiveComponent(com)
{
}

CMesh2DComponent::~CMesh2DComponent()
{
	SAFE_RELEASE(m_pMesh);
}

bool CMesh2DComponent::Init()
{
	if (!CPrimitiveComponent::Init())
		return false;

	//2DComponent는 2D사각형을 기본으로 Setting
	//------------------------------------------
	CMesh* pMesh = m_pScene->GetResourceManager()->GetDefault2DMesh();
	SetMesh((CMesh2D*)pMesh);
	//------------------------------------------

	SAFE_RELEASE(pMesh);

	m_pTransform->SetTransformSpace(true);

	return true;
}

void CMesh2DComponent::Start()
{
	CPrimitiveComponent::Start();
}

void CMesh2DComponent::Update(float fTime)
{
	CPrimitiveComponent::Update(fTime);
}

void CMesh2DComponent::PostUpdate(float fTime)
{
	CPrimitiveComponent::PostUpdate(fTime);
}

void CMesh2DComponent::Collision(float fTime)
{
	CPrimitiveComponent::Collision(fTime);
}

void CMesh2DComponent::PrevRender(float fTime)
{
	CPrimitiveComponent::PrevRender(fTime);
}

void CMesh2DComponent::Render(float fTime)
{
	CPrimitiveComponent::Render(fTime);

	m_pMesh->Render(fTime);
}

void CMesh2DComponent::PostRender(float fTime)
{
	CPrimitiveComponent::PostRender(fTime);
}

CMesh2DComponent* CMesh2DComponent::Clone()
{
	return new CMesh2DComponent(*this);
}

void CMesh2DComponent::Save(FILE* pFile)
{
	CPrimitiveComponent::Save(pFile);
}

void CMesh2DComponent::Load(FILE* pFile)
{
	CPrimitiveComponent::Load(pFile);

	CMesh* pMesh = m_pScene->GetResourceManager()->GetDefault2DMesh();
	SetMesh((CMesh2D*)pMesh);

	SAFE_RELEASE(pMesh);
}
