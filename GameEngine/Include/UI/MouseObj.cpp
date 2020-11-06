#include "MouseObj.h"
#include "UITransform.h"
#include "../Resource/Material.h"
#include "UIImage.h"
#include "UISprite.h"

CMouseObj::CMouseObj()
	: m_pImage(nullptr)
{
}

CMouseObj::CMouseObj(const CMouseObj& mouse)
	: CUIObject(mouse)
{
}

CMouseObj::~CMouseObj()
{
	SAFE_RELEASE(m_pImage);
}

bool CMouseObj::Init()
{
	if (!CUIObject::Init())
		return false;

	m_pImage = CreateControl<CUISprite>("Image");

	SetRoot(m_pImage);

	m_pImage->SetRelativeScale(32.f, 31.f, 1.f);
	m_pImage->SetPivot(0.f, 1.f, 0.f);
	m_pImage->AddSpriteInfo("idle", "MouseIdle", true);

	return true;
}

void CMouseObj::Start()
{
	CUIObject::Start();
}

void CMouseObj::Update(float fTime)
{
	CUIObject::Update(fTime);
}

void CMouseObj::PostUpdate(float fTime)
{
	CUIObject::PostUpdate(fTime);
}

void CMouseObj::PrevRender(float fTime)
{
	CUIObject::PrevRender();
}

void CMouseObj::Render(float fTime)
{
	CUIObject::Render();
}

void CMouseObj::PostRender(float fTime)
{
	CUIObject::PostRender();
}

void CMouseObj::Save(FILE* pFile)
{
	CUIObject::Save(pFile);
}

void CMouseObj::Load(FILE* pFile)
{
	CUIObject::Load(pFile);
}

void CMouseObj::CollisionBegin(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CMouseObj::CollisionEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
}
