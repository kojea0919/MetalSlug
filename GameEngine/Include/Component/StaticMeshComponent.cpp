#include "StaticMeshComponent.h"

CStaticMeshComponent::CStaticMeshComponent()
{
	m_eSceneComponentType = SCENECOMPONENT_TYPE::ST_3D;
	m_eSceneClassType = SCENECOMPONENT_CLASS_TYPE::STATICMESH;
}

CStaticMeshComponent::CStaticMeshComponent(const CStaticMeshComponent& com) :
	CPrimitiveComponent(com)
{
}

CStaticMeshComponent::~CStaticMeshComponent()
{
}

bool CStaticMeshComponent::Init()
{
	if (!CPrimitiveComponent::Init())
		return false;

	return true;
}

void CStaticMeshComponent::Start()
{
	CPrimitiveComponent::Start();
}

void CStaticMeshComponent::Update(float fTime)
{
	CPrimitiveComponent::Update(fTime);
}

void CStaticMeshComponent::PostUpdate(float fTime)
{
	CPrimitiveComponent::PostUpdate(fTime);
}

void CStaticMeshComponent::Collision(float fTime)
{
	CPrimitiveComponent::Collision(fTime);
}

void CStaticMeshComponent::PrevRender(float fTime)
{
	CPrimitiveComponent::PrevRender(fTime);
}

void CStaticMeshComponent::Render(float fTime)
{
	CPrimitiveComponent::Render(fTime);
}

void CStaticMeshComponent::PostRender(float fTime)
{
	CPrimitiveComponent::PostRender(fTime);
}

CStaticMeshComponent* CStaticMeshComponent::Clone()
{
	return new CStaticMeshComponent(*this);
}

void CStaticMeshComponent::Save(FILE* pFile)
{
	CPrimitiveComponent::Save(pFile);
}

void CStaticMeshComponent::Load(FILE* pFile)
{
	CPrimitiveComponent::Load(pFile);
}

