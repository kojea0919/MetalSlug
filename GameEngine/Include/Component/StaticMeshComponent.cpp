#include "StaticMeshComponent.h"

CStaticMeshComponent::CStaticMeshComponent()
{
	m_eSceneComponentType = SCENECOMPONENT_TYPE::ST_3D;
	m_eSceneClassType = SCENECOMPONENT_CLASS_TYPE::STATICMESH;
}

CStaticMeshComponent::CStaticMeshComponent(const CStaticMeshComponent& com) :
	CMeshComponent(com)
{
}

CStaticMeshComponent::~CStaticMeshComponent()
{
}

bool CStaticMeshComponent::Init()
{
	if (!CMeshComponent::Init())
		return false;

	return true;
}

void CStaticMeshComponent::Start()
{
	CMeshComponent::Start();
}

void CStaticMeshComponent::Update(float fTime)
{
	CMeshComponent::Update(fTime);
}

void CStaticMeshComponent::PostUpdate(float fTime)
{
	CMeshComponent::PostUpdate(fTime);
}

void CStaticMeshComponent::Collision(float fTime)
{
	CMeshComponent::Collision(fTime);
}

void CStaticMeshComponent::PrevRender(float fTime)
{
	CMeshComponent::PrevRender(fTime);
}

void CStaticMeshComponent::Render(float fTime)
{
	CMeshComponent::Render(fTime);
}

void CStaticMeshComponent::PostRender(float fTime)
{
	CMeshComponent::PostRender(fTime);
}

CStaticMeshComponent* CStaticMeshComponent::Clone()
{
	return new CStaticMeshComponent(*this);
}

void CStaticMeshComponent::Save(FILE* pFile)
{
	CMeshComponent::Save(pFile);
}

void CStaticMeshComponent::Load(FILE* pFile)
{
	CMeshComponent::Load(pFile);
}

