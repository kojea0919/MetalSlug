#include "UISlot.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Texture.h"
#include "../Resource/Material.h"

CUISlot::CUISlot()
{
}

CUISlot::CUISlot(const CUISlot& control)
	: CUIControl(control)
{
}

CUISlot::~CUISlot()
{
}

bool CUISlot::Init()
{
	if (!CUIControl::Init())
		return false;

	m_vConnectPos = Vector3(7.f, 5.f, 0.f);

	return true;
}

void CUISlot::Start()
{
	CUIControl::Start();
}

void CUISlot::Update(float fTime)
{
	CUIControl::Update(fTime);
}

void CUISlot::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CUISlot::PrevRender()
{
	CUIControl::PrevRender();
}

void CUISlot::Render()
{
	CUIControl::Render();

	m_pUITexture->SetShader(0, (int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL);
	m_pMaterial->SetMaterial();

	m_pBodyMesh->Render(0.f);
}

void CUISlot::PostRender()
{
	CUIControl::PostRender();
}

CUISlot* CUISlot::Clone()
{
	return new CUISlot(*this);
}
