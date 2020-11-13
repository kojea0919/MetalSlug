#include "PrimitiveComponent.h"
#include "../Render/RenderManager.h"

CPrimitiveComponent::CPrimitiveComponent()
	: m_strLayer("Default")
{
}

CPrimitiveComponent::CPrimitiveComponent(const CPrimitiveComponent& com)
    : CSceneComponent(com)
{
}

CPrimitiveComponent::~CPrimitiveComponent()
{
}

bool CPrimitiveComponent::Init()
{
    CSceneComponent::Init();

    return true;
}

void CPrimitiveComponent::Start()
{
	CSceneComponent::Start();

	auto	iter = m_vecChild.begin();
	auto	iterEnd = m_vecChild.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}
}

void CPrimitiveComponent::Update(float fTime)
{
	CSceneComponent::Update(fTime);
}

void CPrimitiveComponent::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CPrimitiveComponent::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CPrimitiveComponent::PrevRender(float fTime)
{
	GET_SINGLE(CRenderManager)->AddSceneComponent(this);
	CSceneComponent::PrevRender(fTime);
}

void CPrimitiveComponent::Render(float fTime)
{
	CSceneComponent::Render(fTime);
}

void CPrimitiveComponent::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

CPrimitiveComponent* CPrimitiveComponent::Clone()
{
	return new CPrimitiveComponent(*this);
}

void CPrimitiveComponent::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);

	fwrite(&m_eRenderPriority, sizeof(m_eRenderPriority), 1, pFile);
}

void CPrimitiveComponent::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);
	
	fread(&m_eRenderPriority, sizeof(m_eRenderPriority), 1, pFile);
}
