#include "ObjectComponent.h"

CObjectComponent::CObjectComponent()
{
	m_eComponentType = COMPONENT_TYPE::OBJECT;
}

CObjectComponent::CObjectComponent(const CObjectComponent& com)	:
	CComponent(com)
{
}

CObjectComponent::~CObjectComponent()
{
}

bool CObjectComponent::Init()
{
	return true;
}

void CObjectComponent::Start()
{
	CComponent::Start();
}

void CObjectComponent::Update(float fTime)
{
	CComponent::Update(fTime);
}

void CObjectComponent::PostUpdate(float fTime)
{
	CComponent::PostUpdate(fTime);
}

void CObjectComponent::Collision(float fTime)
{
	CComponent::Collision(fTime);
}

void CObjectComponent::PrevRender(float fTime)
{
	CComponent::PrevRender(fTime);
}

void CObjectComponent::Render(float fTime)
{
	CComponent::Render(fTime);
}

void CObjectComponent::PostRender(float fTime)
{
	CComponent::PostRender(fTime);
}

CObjectComponent* CObjectComponent::Clone()
{
	return new CObjectComponent(*this);
}

void CObjectComponent::Save(FILE* pFile)
{
	CComponent::Save(pFile);
}

void CObjectComponent::Load(FILE* pFile)
{
	CComponent::Load(pFile);
}
