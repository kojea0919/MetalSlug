#include "TileMap.h"
#include "../Resource/Material.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Mesh2D.h"
#include "Transform.h"
#include "../Scene/CameraManager.h"
#include "Camera.h"
#include "../Device.h"
#include "../Render/RenderInstancing.h"
#include "../Render/RenderManager.h"
#include "../Render/RenderState.h"
#include "Tile.h"

CTileMap::CTileMap()
    : m_bTileMapRender(false),
    m_pTileMaterial(nullptr),
    m_pInstancing(nullptr),
    m_pDepthDisable(nullptr)
{
    m_eSceneComponentType = SCENECOMPONENT_TYPE::ST_2D;
    m_eSceneClassType = SCENECOMPONENT_CLASS_TYPE::TILEMAP;
    m_vTileSize = Vector3(30.f, 30.f, 1.f);
}

CTileMap::CTileMap(const CTileMap& com)
    :CPrimitiveComponent(com)
{
    m_bTileMapRender = com.m_bTileMapRender;

    m_vTileSize = com.m_vTileSize;

    if (m_pTileMaterial)
        m_pTileMaterial->AddRef();
}

CTileMap::~CTileMap()
{
    SAFE_DELETE(m_pInstancing);
    SAFE_RELEASE(m_pTileMaterial);
    SAFE_DELETE_VECLIST(m_vecTile);
}

void CTileMap::SetTexture(const string& strName)
{
    m_pMaterial->SetTexture(TEXTURE_LINK::DIFFUSE, strName);
    m_bTileMapRender = true;
}

void CTileMap::SetTexture(CTexture* pTexture)
{
    m_pMaterial->SetTexture(TEXTURE_LINK::DIFFUSE, pTexture);
    m_bTileMapRender = true;
}

void CTileMap::SetTileMaterial(const string& strName)
{
    //해당 이름의 Material을 Setting
    //---------------------------------------------------------------------
    SAFE_RELEASE(m_pTileMaterial);
    m_pTileMaterial = m_pScene->GetResourceManager()->FindMaterial(strName);
    //---------------------------------------------------------------------

    //모든 타일에 FrameStart,FrameEnd Setting
    //---------------------------------------------------------------------
    int iWidth = m_iCountX;
    int iHeight = m_iCountY;
    for (int iHeightCnt = 0; iHeightCnt < iHeight; ++iHeightCnt)
    {
        for (int iWidthCnt = 0; iWidthCnt < iWidth; ++iWidthCnt)
        {
            int iIndex = iHeightCnt * iWidth + iWidthCnt;

            m_vecTile[iIndex]->SetFrameStart(0.f, 0.f);
            m_vecTile[iIndex]->SetFrameEnd(m_pTileMaterial->GetDiffuseTextureSize());
        }
    }
    //---------------------------------------------------------------------

    if (!m_pInstancing)
    {
        m_pInstancing = new CRenderInstancing;
        m_pInstancing->Init(m_pMesh, m_pTileMaterial, sizeof(InstancingData2D), "InstancingShader2D");
    }

    else
        m_pInstancing->SetMaterial(m_pTileMaterial);
}

void CTileMap::SetTileMaterial(CMaterial* pMaterial)
{
    //Material Setting
    //---------------------------------------------------------------------
    SAFE_RELEASE(m_pTileMaterial);
    m_pTileMaterial = pMaterial;
    //---------------------------------------------------------------------

    //모든 타일에 FrameStart,FrameEnd Setting
    //---------------------------------------------------------------------
    int iWidth = m_iCountX;
    int iHeight = m_iCountY;
    for (int iHeightCnt = 0; iHeightCnt < iHeight; ++iHeightCnt)
    {
        for (int iWidthCnt = 0; iWidthCnt < iWidth; ++iWidthCnt)
        {
            int iIndex = iHeightCnt * iWidth + iWidthCnt;

            m_vecTile[iIndex]->SetFrameStart(0.f, 0.f);
            m_vecTile[iIndex]->SetFrameEnd(m_pTileMaterial->GetDiffuseTextureSize());
        }
    }
    //---------------------------------------------------------------------

    if (!m_pInstancing)
    {
        m_pInstancing = new CRenderInstancing;
        m_pInstancing->Init(m_pMesh, m_pTileMaterial, sizeof(InstancingData2D), "InstancingShader2D");
    }

    else
        m_pInstancing->SetMaterial(m_pTileMaterial);
}

void CTileMap::SetTileImageSize(float x, float y)
{
    m_vTileImageSize = Vector2(x, y);
    
    int iWidth = m_iCountX;
    int iHeight = m_iCountY;
    for (int iHeightCnt = 0; iHeightCnt < iHeight; ++iHeightCnt)
    {
        for (int iWidthCnt = 0; iWidthCnt < iWidth; ++iWidthCnt)
        {
            int iIndex = iHeightCnt * iWidth + iWidthCnt;

            m_vecTile[iIndex]->SetFrameStart(0.f, 0.f);
            m_vecTile[iIndex]->SetFrameEnd(m_vTileImageSize);
        }
    }
}

void CTileMap::SetTileImageSize(const Vector2& vSize)
{
    m_vTileImageSize = vSize; 

    int iWidth = m_iCountX;
    int iHeight = m_iCountY;
    for (int iHeightCnt = 0; iHeightCnt < iHeight; ++iHeightCnt)
    {
        for (int iWidthCnt = 0; iWidthCnt < iWidth; ++iWidthCnt)
        {
            int iIndex = iHeightCnt * iWidth + iWidthCnt;

            m_vecTile[iIndex]->SetFrameStart(0.f, 0.f);
            m_vecTile[iIndex]->SetFrameEnd(m_vTileImageSize);
        }
    }
}

void CTileMap::SetTileFrame(const Vector3& vPos, int iImageFrameX, int iImageFrameY)
{
    Vector3	vConvertPos = vPos - GetWorldPos();

    int idxX = (int)(vConvertPos.x / m_vTileSize.x);
    int idxY = (int)(vConvertPos.y / m_vTileSize.y);

    int	iIndex = idxY * m_iCountX + idxX;

    m_vecTile[iIndex]->SetFrameStart(iImageFrameX * m_vTileImageSize.x, iImageFrameY * m_vTileImageSize.y);
    m_vecTile[iIndex]->SetFrameEnd((iImageFrameX + 1) * m_vTileImageSize.x, (iImageFrameY + 1) * m_vTileImageSize.y);
}

void CTileMap::SetTileFrame(int idxX, int idxY, int iImageFrameX, int iImageFrameY)
{
    int	iIndex = idxY * m_iCountX + idxX;

    m_vecTile[iIndex]->SetFrameStart(iImageFrameX * m_vTileImageSize.x, iImageFrameY * m_vTileImageSize.y);
    m_vecTile[iIndex]->SetFrameEnd((iImageFrameX + 1) * m_vTileImageSize.x, (iImageFrameY + 1) * m_vTileImageSize.y);
}

bool CTileMap::Init()
{
    if (!CPrimitiveComponent::Init())
        return false;

    m_pDepthDisable = GET_SINGLE(CRenderManager)->FindRenderState("DepthDisable");

    CMesh* pMesh = m_pScene->GetResourceManager()->GetDefault2DMesh();
    SetMesh((CMesh2D*)pMesh);

    SAFE_RELEASE(pMesh);

    m_pTransform->SetTransformSpace(true);

    return true;
}

void CTileMap::Start()
{
    CPrimitiveComponent::Start();

    m_pScene->SetWorldStart(GetWorldPos().x, GetWorldPos().y, GetWorldPos().z);
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

    //Tile InstancingData 정보 Setting
    //------------------------------------------
    if (m_pInstancing)
    {
        CCamera* pCamera = m_pScene->GetCameraManager()->GetMainCamera();

        int iHeight = m_iEndY;
        int iWidth = m_iEndX;

        for (int iHeightCnt = m_iStartY; iHeightCnt <= iHeight + 1; ++iHeightCnt)
        {
            for (int iWidthCnt = m_iStartX; iWidthCnt <= iWidth + 1; ++iWidthCnt)
            {
                int iIndex = iHeightCnt * m_iCountX + iWidthCnt;

                InstancingData2D	tData = {};

                tData.matWVP = m_vecTile[iIndex]->GetWorldMatirx() * pCamera->GetViewMatrix() *
                    pCamera->GetProjMatrix();
                tData.vMeshPivot = m_pTransform->GetPivot();
                tData.vMeshSize = m_pTransform->GetMeshSize();
                tData.vFrameStart = m_vecTile[iIndex]->GetFrameStart();
                tData.vFrameEnd = m_vecTile[iIndex]->GetFrameEnd();
                tData.vImageSize = m_pTileMaterial->GetDiffuseTextureSize();

                tData.matWVP.Transpose();

                m_pInstancing->AddInstancingData(&tData);
            }
        }
    
    }
    //------------------------------------------
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

    m_pDepthDisable->SetState();

    if (m_bTileMapRender)
    {
        m_pMaterial->SetMaterial();
        m_pMesh->Render(fTime);
    }

    // Instancing으로 그려낸다.
    if (m_pInstancing)
    {
        m_pInstancing->Render(fTime);
    }

    m_pDepthDisable->ResetState();
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

void CTileMap::SetWorldInfo()
{
    SetWorldScale(m_vTileSize.x * m_iCountX, m_vTileSize.y * m_iCountY, 1.f);

    m_pScene->SetWorldSize(GetWorldScale().x, GetWorldScale().y, 0.f);
}
