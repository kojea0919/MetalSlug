#include "UIText.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Texture.h"
#include "../Resource/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Device.h"

CUIText::CUIText()
{
	m_p2DTarget = GET_SINGLE(CDevice)->Get2DRenderTarget();
	m_bShadow = false;

	m_pFontName = new TCHAR[64];
	memset(m_pFontName, 0, sizeof(TCHAR) * 64);
	lstrcpy(m_pFontName, TEXT("Noto Sans CJK KR Black"));
	m_fSize = 20.f;
	m_eAlignH = TAH_LEFT;
	m_eAlignV = TAV_CENTER;

	m_bAlpha = false;
	m_bShadowAlpha = false;
	m_fOpacity = 1.f;
	m_fShadowOpacity = 1.f;
	m_vColor = Vector4::White;
	m_vShadowColor = Vector4::Black;

	m_iMaxCount = 256;
	m_pText = new TCHAR[m_iMaxCount];
	memset(m_pText, 0, sizeof(TCHAR) * m_iMaxCount);

	lstrcpy(m_pText, TEXT("Text"));

	m_pTextFormat = GET_SINGLE(CResourceManager)->FindFont("NormalText1");
	m_pTextLayout = GET_SINGLE(CResourceManager)->CreateTextLayout(m_pText, m_pTextFormat, 100.f, 100.f);
	m_pColor = GET_SINGLE(CResourceManager)->CreateColor(m_vColor);
	m_pShadowColor = GET_SINGLE(CResourceManager)->CreateColor(m_vShadowColor);

	m_tRenderArea.left = 0.f;
	m_tRenderArea.right = 100.f;
	m_tRenderArea.top = 0.f;
	m_tRenderArea.bottom = 100.f;
}

CUIText::CUIText(const CUIText& control)
{
	*this = control;

	m_pText = new TCHAR[m_iMaxCount];
	lstrcpy(m_pText, control.m_pText);

	m_pFontName = new TCHAR[64];
	lstrcpy(m_pFontName, control.m_pFontName);

	CreateTextLayout();
}

CUIText::~CUIText()
{
	SAFE_RELEASE(m_pTextLayout);
	SAFE_DELETE_ARRAY(m_pText);
	SAFE_DELETE_ARRAY(m_pFontName);
}

void CUIText::SetTextCount(int iMaxCount)
{
	if (m_iMaxCount >= iMaxCount)
		return;

	TCHAR* pText = new TCHAR[iMaxCount];
	memset(pText, 0, sizeof(TCHAR) * iMaxCount);

	memcpy(pText, m_pText, sizeof(TCHAR) * m_iMaxCount);

	m_iMaxCount = iMaxCount;

	SAFE_DELETE_ARRAY(m_pText);
	m_pText = pText;
}

void CUIText::SetText(const TCHAR* pText)
{
	int	iCount = lstrlen(pText);

	if (m_iMaxCount < iCount)
	{
		m_iMaxCount = iCount + 1;
		SAFE_DELETE_ARRAY(m_pText);
		m_pText = new TCHAR[m_iMaxCount];
	}

	memset(m_pText, 0, sizeof(TCHAR) * m_iMaxCount);
	lstrcpy(m_pText, pText);

	CreateTextLayout();
}

void CUIText::AddText(const TCHAR* pText)
{
	int	iCount = lstrlen(pText);
	int	iCurCount = lstrlen(m_pText);

	if (m_iMaxCount < iCount + iCurCount)
	{
		m_iMaxCount = (iCount + iCurCount) * 2;

		TCHAR* pNewText = new TCHAR[m_iMaxCount];
		memset(pNewText, 0, sizeof(TCHAR) * m_iMaxCount);

		lstrcpy(pNewText, m_pText);

		SAFE_DELETE_ARRAY(m_pText);

		m_pText = pNewText;
	}

	lstrcat(m_pText, pText);

	CreateTextLayout();
}

void CUIText::SetFont(const string& strName, const TCHAR* pFontName, int iWeight, int iStyle, int iStretch, float fSize, const TCHAR* pLocalName)
{
	lstrcpy(m_pFontName, pFontName);
	m_fSize = fSize;

	GET_SINGLE(CResourceManager)->CreateTextFormat(strName, pFontName, iWeight, iStyle, iStretch, fSize, pLocalName);

	m_pTextFormat = GET_SINGLE(CResourceManager)->FindFont(strName);

	CreateTextLayout();
}

void CUIText::SetFont(const string& strName)
{
	m_pTextFormat = GET_SINGLE(CResourceManager)->FindFont(strName);

	CreateTextLayout();
}

void CUIText::SetSize(float fSize)
{
	m_fSize = fSize;

	CreateTextLayout();
}

void CUIText::SetAlignH(TEXT_ALIGH_H eAlign)
{
	m_eAlignH = eAlign;

	if (!m_pTextLayout)
		CreateTextLayout();

	switch (eAlign)
	{
	case TAH_LEFT:
		m_pTextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;
	case TAH_RIGHT:
		m_pTextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;
	case TAH_CENTER:
		m_pTextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;
	}
}

void CUIText::SetAlignV(TEXT_ALIGH_V eAlign)
{
	m_eAlignV = eAlign;

	if (!m_pTextLayout)
		CreateTextLayout();

	switch (eAlign)
	{
	case TAV_TOP:
		m_pTextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	case TAV_BOTTOM:
		m_pTextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;
	case TAV_CENTER:
		m_pTextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	}
}

void CUIText::SetShadow(bool bShadow)
{
	m_bShadow = bShadow;
}

void CUIText::SetShadowOffset(const Vector3& vShadowOffset)
{
	m_vShadowOffset = vShadowOffset;
}

void CUIText::SetShadowColor(float r, float g, float b, float a)
{
	m_vShadowColor = Vector4(r, g, b, a);

	m_pShadowColor = GET_SINGLE(CResourceManager)->CreateColor(r, g, b, a);
}

void CUIText::SetShadowColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_vShadowColor = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);

	m_pShadowColor = GET_SINGLE(CResourceManager)->CreateColor(r, g, b, a);
}

void CUIText::SetShadowColor(const Vector4& vColor)
{
	m_vShadowColor = vColor;

	m_pShadowColor = GET_SINGLE(CResourceManager)->CreateColor(vColor);
}

void CUIText::SetShadowColor(unsigned int iColor)
{
	m_vShadowColor.z = (iColor & 0x000000ff) / 255.f;
	m_vShadowColor.y = ((iColor >> 8) & 0x000000ff) / 255.f;
	m_vShadowColor.x = ((iColor >> 16) & 0x000000ff) / 255.f;
	m_vShadowColor.w = ((iColor >> 24) & 0x000000ff) / 255.f;

	m_pShadowColor = GET_SINGLE(CResourceManager)->CreateColor(iColor);
}

void CUIText::AlphaBlend(bool bAlphaBlend)
{
	m_bAlpha = bAlphaBlend;
}

void CUIText::ShadowAlphaBlend(bool bAlphaBlend)
{
	m_bShadowAlpha = bAlphaBlend;
}

void CUIText::SetOpacity(float fOpacity)
{
	m_fOpacity = fOpacity;
}

void CUIText::SetShadowOpacity(float fOpacity)
{
	m_fShadowOpacity = fOpacity;
}

void CUIText::SetColor(float r, float g, float b, float a)
{
	m_vColor = Vector4(r, g, b, a);

	m_pColor = GET_SINGLE(CResourceManager)->CreateColor(r, g, b, a);
}

void CUIText::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_vColor = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);

	m_pColor = GET_SINGLE(CResourceManager)->CreateColor(r, g, b, a);
}

void CUIText::SetColor(const Vector4& vColor)
{
	m_vColor = vColor;

	m_pColor = GET_SINGLE(CResourceManager)->CreateColor(vColor);
}

void CUIText::SetColor(unsigned int iColor)
{
	m_vColor.z = (iColor & 0x000000ff) / 255.f;
	m_vColor.y = ((iColor >> 8) & 0x000000ff) / 255.f;
	m_vColor.x = ((iColor >> 16) & 0x000000ff) / 255.f;
	m_vColor.w = ((iColor >> 24) & 0x000000ff) / 255.f;

	m_pColor = GET_SINGLE(CResourceManager)->CreateColor(iColor);
}

void CUIText::SetRenderArea(float l, float t, float r, float b)
{
	m_tRenderArea = D2D1::RectF(l, RESOLUTION.iHeight - t, r, RESOLUTION.iHeight - b);

	CreateTextLayout();
}

void CUIText::CreateTextLayout()
{
	if (!m_pTextFormat)
		return;

	SAFE_RELEASE(m_pTextLayout);

	m_pTextLayout = GET_SINGLE(CResourceManager)->CreateTextLayout(m_pText, m_pTextFormat,
		m_tRenderArea.right - m_tRenderArea.left, m_tRenderArea.bottom - m_tRenderArea.top);

	DWRITE_TEXT_RANGE	tRange;
	tRange.startPosition = 0;
	tRange.length = lstrlen(m_pText);

	m_pTextLayout->SetFontSize(m_fSize, tRange);
}

bool CUIText::Init()
{
	if (!CUIControl::Init())
		return false;

	return true;
}

void CUIText::Start()
{
	CUIControl::Start();
}

void CUIText::Update(float fTime)
{
	CUIControl::Update(fTime);
}

void CUIText::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CUIText::PrevRender()
{
	CUIControl::PrevRender();
}

void CUIText::Render()
{
	m_p2DTarget->BeginDraw();

	Vector3	vPos = GetWorldPos();
	float	fHeight = m_tRenderArea.bottom - m_tRenderArea.top;

	// 그림자를  출력해야  할  경우  그림자를  먼저  출력한다.
	if (m_bShadow)
	{
		Vector3	vShadowPos = vPos + m_vShadowOffset;

		if (m_bShadowAlpha)
			m_pShadowColor->SetOpacity(m_fShadowOpacity);

		else
			m_pShadowColor->SetOpacity(1.f);

		vShadowPos.y = RESOLUTION.iHeight - vShadowPos.y - fHeight;

		m_p2DTarget->DrawTextLayout(D2D1::Point2F(vShadowPos.x, vShadowPos.y), m_pTextLayout, m_pShadowColor, D2D1_DRAW_TEXT_OPTIONS_NONE);
	}

	if (m_bAlpha)
		m_pColor->SetOpacity(m_fOpacity);

	else
		m_pColor->SetOpacity(1.f);

	vPos.y = RESOLUTION.iHeight - vPos.y - fHeight;

	m_p2DTarget->DrawTextLayout(D2D1::Point2F(vPos.x, vPos.y), m_pTextLayout, m_pColor, D2D1_DRAW_TEXT_OPTIONS_NONE);

	m_p2DTarget->EndDraw();
}

void CUIText::PostRender()
{
	CUIControl::PostRender();
}

CUIText* CUIText::Clone()
{
	return new CUIText(*this);
}
