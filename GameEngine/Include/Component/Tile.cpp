#include "Tile.h"
#include "../Resource/Material.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Mesh2D.h"

CTile::CTile()
    : m_pMaterial(nullptr),
    m_pMesh(nullptr)
{
}

CTile::~CTile()
{
    SAFE_RELEASE(m_pMesh);
    SAFE_RELEASE(m_pMaterial);
}

bool CTile::Init()
{
    //Tile은 이동하지 않으므로 Init에서 Setting후 끝
    Matrix	matScale, matTranslate;

    matScale.Scaling(m_vTileSize.x, m_vTileSize.y, 1.f);
    matTranslate.Translation(m_vTilePos.x, m_vTilePos.y, 0.f);

    m_matWorld = matScale * matTranslate;

    return true;
}

void CTile::Start()
{
}

void CTile::Update(float fTime)
{
}

void CTile::PostUpdate(float fTime)
{
}

void CTile::Render(float fTime)
{
}
