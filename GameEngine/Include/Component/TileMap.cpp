#include "TileMap.h"
#include "../Resource/Material.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Mesh2D.h"
#include "Transform.h"
#include "../Scene/CameraManager.h"
#include "Camera.h"
#include "../Device.h"
#include "Tile.h"

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

    //화면 영역을 TileMap 좌하단을 원점으로 표현
    //--------------------------------------------------------------
    CCamera* pCamera = m_pScene->GetCameraManager()->GetMainCamera();

    //화면 영역 Setting
    Vector3	vLB = pCamera->GetWorldPos() - Vector3(RESOLUTION.iWidth / 2.f, RESOLUTION.iHeight / 2.f, 0.f);
    Vector3	vRT = vLB + Vector3((float)RESOLUTION.iWidth, (float)RESOLUTION.iHeight, 0.f);

    Vector3	vPos = GetWorldPos();

    vLB -= vPos;
    vRT -= vPos;
    //--------------------------------------------------------------

    vPos.z = 1000.f;

    SetWorldPos(vPos);

    //화면에 출력중인 Tile만 Update
    //--------------------------------------------------------------
    m_iStartX = (int)(vLB.x / m_vTileSize.x);
    m_iStartY = (int)(vLB.y / m_vTileSize.y);

    m_iEndX = (int)(vRT.x / m_vTileSize.x);
    m_iEndY = (int)(vRT.y / m_vTileSize.y);

    m_iStartX = m_iStartX < 0 ? 0 : m_iStartX;
    m_iStartY = m_iStartY < 0 ? 0 : m_iStartY;
    m_iEndX = m_iEndX >= m_iCountX ? m_iCountX - 1 : m_iEndX;
    m_iEndY = m_iEndY >= m_iCountY ? m_iCountY - 1 : m_iEndY;

    for (int iHeightCnt = m_iStartY; iHeightCnt <= m_iEndY; ++iHeightCnt)
    {
        for (int iWidthCnt = m_iStartX; iWidthCnt <= m_iEndX; ++iWidthCnt)
        {
            int	iIndex = iHeightCnt * m_iCountX + iWidthCnt;
            m_vecTile[iIndex]->Update(fTime);
        }
    }
    //--------------------------------------------------------------
}

void CTileMap::PostUpdate(float fTime)
{
    CPrimitiveComponent::PostUpdate(fTime);

    for (int iHeightCnt = m_iStartY; iHeightCnt <= m_iEndY; ++iHeightCnt)
    {
        for (int iWidthCnt = m_iStartX; iWidthCnt <= m_iEndX; ++iWidthCnt)
        {
            int	iIndex = iHeightCnt * m_iCountX + iWidthCnt;
            m_vecTile[iIndex]->PostUpdate(fTime);
        }
    }
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

    if (m_pMesh)
        m_pMesh->Render(fTime);

    for (int iHeightCnt = m_iStartY; iHeightCnt <= m_iEndY; ++iHeightCnt)
    {
        for (int iWidthCnt = m_iStartX; iWidthCnt <= m_iEndX; ++iWidthCnt)
        {
            int	iIndex = iHeightCnt * m_iCountX + iWidthCnt;
            m_vecTile[iIndex]->Render(fTime);
        }
    }
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
