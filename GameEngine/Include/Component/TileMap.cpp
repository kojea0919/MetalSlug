#include "TileMap.h"
#include "../Resource/Material.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Mesh2D.h"
#include "Transform.h"

CTileMap::CTileMap()
    : m_pMaterial(nullptr),m_pMesh(nullptr)
{
    m_eSceneComponentType = SCENECOMPONENT_TYPE::ST_2D;
    m_eSceneClassType = SCENECOMPONENT_CLASS_TYPE::TILEMAP;
    m_vTileSize = Vector3(30.f, 30.f, 1.f);
}

CTileMap::CTileMap(const CTileMap& com)
    :CPrimitiveComponent(com)
{
    m_pMaterial = com.m_pMaterial;

    if (m_pMaterial)
        m_pMaterial->AddRef();

    m_pMesh = com.m_pMesh;

    if (m_pMesh)
        m_pMesh->AddRef();

    m_vTileSize = com.m_vTileSize;
}

CTileMap::~CTileMap()
{
    SAFE_RELEASE(m_pMesh);
    SAFE_RELEASE(m_pMaterial);
}

void CTileMap::SetMaterial(CMaterial* pMaterial)
{
    SAFE_RELEASE(m_pMaterial);
    m_pMaterial = pMaterial;
    if (m_pMaterial)
        m_pMaterial->AddRef();
}

CMaterial* CTileMap::GetMaterial() const
{
    if (m_pMaterial)
        m_pMaterial->AddRef();

    return m_pMaterial;
}

void CTileMap::SetMesh(CMesh2D* pMesh)
{
    SAFE_RELEASE(m_pMesh);
    m_pMesh = pMesh;

    if (m_pMesh)
    {
        CMaterial* pMaterial = m_pMesh->GetMaterial();
        CMaterial* pClone = pMaterial->Clone();

        SetMaterial(pClone);
        m_pMesh->AddRef();
        SAFE_RELEASE(pMaterial);
        SAFE_RELEASE(pClone);

        m_pTransform->SetMeshSize(m_pMesh->GetMax() - m_pMesh->GetMin());
    }
}

void CTileMap::SetMesh(const string& strMeshName)
{
    SAFE_RELEASE(m_pMesh);
    m_pMesh = (CMesh2D*)m_pScene->GetResourceManager()->FindMesh(strMeshName);

    if (m_pMesh)
    {
        CMaterial* pMaterial = m_pMesh->GetMaterial();
        CMaterial* pClone = pMaterial->Clone();

        SetMaterial(pClone);
        SAFE_RELEASE(pClone);
        SAFE_RELEASE(pMaterial);

        m_pTransform->SetMeshSize(m_pMesh->GetMax() - m_pMesh->GetMin());
    }
}

bool CTileMap::Init()
{
    if (!CPrimitiveComponent::Init())
        return false;

    return true;
}

void CTileMap::Start()
{
    CPrimitiveComponent::Start();
}

void CTileMap::Update(float fTime)
{
    CPrimitiveComponent::Update(fTime);
}

void CTileMap::PostUpdate(float fTime)
{
    CPrimitiveComponent::PostUpdate(fTime);
}

void CTileMap::Collision(float fTime)
{
    CPrimitiveComponent::Collision(fTime);
}

void CTileMap::PrevRender(float fTime)
{
    CPrimitiveComponent::PrevRender(fTime);
}

void CTileMap::Render(float fTime)
{
    CPrimitiveComponent::Render(fTime);

    if (m_pMaterial)
        m_pMaterial->SetMaterial();
}

void CTileMap::PostRender(float fTime)
{
    CPrimitiveComponent::PostRender(fTime);
}

CTileMap* CTileMap::Clone()
{
    return new CTileMap(*this);
}

void CTileMap::Save(FILE* pFile)
{
    CPrimitiveComponent::Save(pFile);

    // 재질 정보를 저장한다.
    bool	bMaterial = false;

    if (m_pMaterial)
    {
        bMaterial = true;
        fwrite(&bMaterial, sizeof(bool), 1, pFile);

        int	iLength = (int)m_pMaterial->GetName().length();
        fwrite(&iLength, sizeof(int), 1, pFile);
        fwrite(m_pMaterial->GetName().c_str(), 1, iLength, pFile);
    }

    else
        fwrite(&bMaterial, sizeof(bool), 1, pFile);
}

void CTileMap::Load(FILE* pFile)
{
    CPrimitiveComponent::Load(pFile);

    // 재질 정보를 저장한다.
    bool	bMaterial = false;

    fread(&bMaterial, sizeof(bool), 1, pFile);

    if (bMaterial)
    {
        int	iLength = 0;
        char	strName[256] = {};
        fread(&iLength, sizeof(int), 1, pFile);
        fread(strName, 1, iLength, pFile);

        CMaterial* pMaterial = m_pScene->GetResourceManager()->FindMaterial(strName);

        SetMaterial(pMaterial);

        SAFE_RELEASE(pMaterial);
    }
}
