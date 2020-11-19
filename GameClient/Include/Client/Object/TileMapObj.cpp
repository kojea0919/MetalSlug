#include "TileMapObj.h"
#include "Component/TileMap.h"
#include "Component/Tile.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Material.h"

CTileMapObj::CTileMapObj()
    : m_pTileMap(nullptr)
{
}

CTileMapObj::CTileMapObj(const CTileMapObj& obj)
    : CGameObject(obj)
{
}

CTileMapObj::~CTileMapObj()
{
    SAFE_RELEASE(m_pTileMap);
}

bool CTileMapObj::Init()
{
    m_pTileMap = CreateComponent<CTileMap>("TileMap");

    SetRootComponent(m_pTileMap);

    m_pTileMap->CreateTile<CTile>(TILE_SHAPE::Rect, 100, 100, 100.f, 100.f);

    m_pTileMap->SetTileMaterial("TileMtrl");
    m_pTileMap->SetTileImageSize(64.f, 320.f);


    return true;
}

void CTileMapObj::Start()
{
    CGameObject::Start();
}

void CTileMapObj::Update(float fTime)
{
    CGameObject::Update(fTime);
}

void CTileMapObj::PostUpdate(float fTime)
{
    CGameObject::PostUpdate(fTime);
}

void CTileMapObj::Collision(float fTime)
{
    CGameObject::Collision(fTime);
}

void CTileMapObj::PrevRender(float fTime)
{
    CGameObject::PrevRender(fTime);
}

void CTileMapObj::Render(float fTime)
{
    CGameObject::Render(fTime);
}

void CTileMapObj::PostRender(float fTime)
{
    CGameObject::PostRender(fTime);
}
