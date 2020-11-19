#include "PrimitiveComponent.h"
#include "../Render/RenderManager.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh.h"
#include "Transform.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CPrimitiveComponent::CPrimitiveComponent()
	: m_strLayer("Default"), m_bInstancing(false),
	m_pMesh(nullptr), m_pMaterial(nullptr)
{
}

CPrimitiveComponent::CPrimitiveComponent(const CPrimitiveComponent& com)
    : CSceneComponent(com)
{
	m_pMaterial = com.m_pMaterial;

	if (m_pMaterial)
		m_pMaterial->AddRef();

	m_pMesh = com.m_pMesh;

	if (m_pMesh)
		m_pMesh->AddRef();
}

CPrimitiveComponent::~CPrimitiveComponent()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pMaterial);
}

Vector2 CPrimitiveComponent::GetFrameStart() const
{
	return Vector2(0.f, 0.f);
}

Vector2 CPrimitiveComponent::GetFrameEnd() const
{
	return GetTextureSize();
}

Vector2 CPrimitiveComponent::GetTextureSize() const
{
	return m_pMaterial->GetDiffuseTextureSize(0);
}

void CPrimitiveComponent::SetMaterial(CMaterial* pMaterial)
{
	SAFE_RELEASE(m_pMaterial);
	m_pMaterial = pMaterial;
	if (m_pMaterial)
		m_pMaterial->AddRef();
}

CMaterial* CPrimitiveComponent::GetMaterial() const
{
	if (m_pMaterial)
		m_pMaterial->AddRef();

	return m_pMaterial;
}

CMesh* CPrimitiveComponent::GetMesh() const
{
	if (m_pMesh)
		m_pMesh->AddRef();

	return m_pMesh;
}

void CPrimitiveComponent::SetMesh(CMesh* pMesh)
{
	SAFE_RELEASE(m_pMesh);
	m_pMesh = pMesh;

	if (m_pMesh)
	{
		CMaterial* pMaterial = m_pMesh->GetMaterial();

		SetMaterial(pMaterial);
		m_pMesh->AddRef();
		SAFE_RELEASE(pMaterial);

		m_pTransform->SetMeshSize(m_pMesh->GetMax() - m_pMesh->GetMin());
	}
}

void CPrimitiveComponent::SetMesh(const string& strMeshName)
{
	SAFE_RELEASE(m_pMesh);
	m_pMesh = m_pScene->GetResourceManager()->FindMesh(strMeshName);

	if (m_pMesh)
	{
		CMaterial* pMaterial = m_pMesh->GetMaterial();

		SetMaterial(pMaterial);
		SAFE_RELEASE(pMaterial);

		m_pTransform->SetMeshSize(m_pMesh->GetMax() - m_pMesh->GetMin());
	}
}

void CPrimitiveComponent::SetTexture(TEXTURE_LINK eLink, CTexture* pTexture, int iShaderType, int iRegister)
{
	m_pMaterial->SetTexture(eLink, pTexture, iShaderType, iRegister);
}

void CPrimitiveComponent::SetTexture(TEXTURE_LINK eLink, const string& strName, int iShaderType, int iRegister)
{
	m_pMaterial->SetTexture(eLink, strName, iShaderType, iRegister);
}

void CPrimitiveComponent::SetTexture(TEXTURE_LINK eLink, const string& strName, const TCHAR* pFileName, const string& strPathName, int iShaderType, int iRegister)
{
	m_pMaterial->SetTexture(eLink, strName, pFileName, strPathName,
		iShaderType, iRegister);
}

void CPrimitiveComponent::SetTexture(TEXTURE_LINK eLink, const string& strName, const TCHAR* pFullPath, int iShaderType, int iRegister)
{
	m_pMaterial->SetTexture(eLink, strName, pFullPath,
		iShaderType, iRegister);
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
	CMaterial* pMaterial = m_pMaterial;
	pMaterial->AddRef();

	//MaterailInst를 사용하는 경우 Instancing false
	//---------------------------------------------------------
	if (m_pMaterial->GetMaterialType() == Material_Type::Instance)
		SetInstancing(false);
	//---------------------------------------------------------

	//RefCount가 7개 이상인 경우에만 Instancing 사용
	//---------------------------------------------------------
	else
	{
		if (pMaterial->GetRefCount() >= 7 && m_pMesh->GetRefCount() >= 7)
		{
			SetInstancing();
		}

		else
			SetInstancing(false);
	}
	//---------------------------------------------------------
	SAFE_RELEASE(pMaterial);

	GET_SINGLE(CRenderManager)->AddSceneComponent(this);
	CSceneComponent::PrevRender(fTime);
}

void CPrimitiveComponent::Render(float fTime)
{
	CSceneComponent::Render(fTime);

	if (m_pMaterial)
		m_pMaterial->SetMaterial();
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