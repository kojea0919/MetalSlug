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
