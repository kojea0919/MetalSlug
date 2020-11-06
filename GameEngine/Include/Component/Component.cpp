#include "Component.h"
#include "../GameObject.h"

CComponent::CComponent()
	: m_bStart(false),m_pScene(nullptr),
	m_pObject(nullptr)
{
}

CComponent::CComponent(const CComponent& com)
	: CRef(com), m_eComponentType(com.m_eComponentType),
	m_bStart(false)
{
}

CComponent::~CComponent()
{
}

CGameObject* CComponent::GetGameObject() const
{
	m_pObject->AddRef();
	return m_pObject;
}

bool CComponent::Init()
{
	return true;
}

void CComponent::Start()
{
	m_bStart = true;
}

void CComponent::Update(float fTime)
{
}

void CComponent::PostUpdate(float fTime)
{
}

void CComponent::Collision(float fTime)
{
}

void CComponent::PrevRender(float fTime)
{
}

void CComponent::Render(float fTime)
{
}

void CComponent::PostRender(float fTime)
{
}

void CComponent::Save(FILE* pFile)
{
	CRef::Save(pFile);

	//Component타입 저장
	fwrite(&m_eComponentType, sizeof(m_eComponentType), 1, pFile);
}

void CComponent::Load(FILE* pFile)
{
	CRef::Load(pFile);

	//Component타입 읽기
	fread(&m_eComponentType, sizeof(m_eComponentType), 1, pFile);
	m_bStart = false;
}
