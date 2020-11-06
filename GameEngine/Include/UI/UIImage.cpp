#include "UIImage.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Texture.h"
#include "../Resource/Material.h"

CUIImage::CUIImage()
{
}

CUIImage::CUIImage(const CUIImage& control)
	: CUIControl(control)
{
}

CUIImage::~CUIImage()
{
}

bool CUIImage::Init()
{
	if (!CUIControl::Init())
		return false;

	return true;
}

void CUIImage::Start()
{
	CUIControl::Start();
}

void CUIImage::Update(float fTime)
{
	CUIControl::Update(fTime);
}

void CUIImage::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CUIImage::PrevRender()
{
	CUIControl::PrevRender();
}

void CUIImage::Render()
{
	CUIControl::Render();

	m_pUITexture->SetShader(0, (int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL);
	m_pMaterial->SetMaterial();

	m_pBodyMesh->Render(0.f);
}

void CUIImage::PostRender()
{
	CUIControl::PostRender();
}

CUIImage* CUIImage::Clone()
{
	return new CUIImage(*this);
}
