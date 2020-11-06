#include "BlendState.h"
#include "../Device.h"

CBlendState::CBlendState()
    : m_iSampleMask(0xffffffff),m_iPrevSampleMask(0)
{
    memset(m_fBlendFactor, 0, sizeof(float) * 4);
    memset(m_fPrevBlendFactor, 0, sizeof(float) * 4);
}

CBlendState::~CBlendState()
{
}

void CBlendState::AddBlendInfo(bool bEnable, D3D11_BLEND eSrcBlend, D3D11_BLEND eDestBlend, D3D11_BLEND_OP eBlendOp, D3D11_BLEND eSrcBlendAlpha, D3D11_BLEND eDestBlendAlpha, D3D11_BLEND_OP eBlendAlphaOp, UINT8 iWriteMask)
{
	//https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_render_target_blend_desc
	D3D11_RENDER_TARGET_BLEND_DESC	tDesc = {};

	tDesc.BlendEnable = bEnable;
	tDesc.SrcBlend = eSrcBlend;
	tDesc.DestBlend = eDestBlend;
	tDesc.BlendOp = eBlendOp;
	tDesc.SrcBlendAlpha = eSrcBlendAlpha;
	tDesc.DestBlendAlpha = eDestBlendAlpha;
	tDesc.BlendOpAlpha = eBlendAlphaOp;
	tDesc.RenderTargetWriteMask = iWriteMask;

	m_vecDesc.push_back(tDesc);
}

bool CBlendState::CreateState(bool bAlphaCover, bool bIndependent)
{
	D3D11_BLEND_DESC	tDesc;

	tDesc.AlphaToCoverageEnable = bAlphaCover;
	tDesc.IndependentBlendEnable = bIndependent;
	memcpy(tDesc.RenderTarget, &m_vecDesc[0],
		sizeof(D3D11_RENDER_TARGET_BLEND_DESC) * m_vecDesc.size());

	if (FAILED(DEVICE->CreateBlendState(&tDesc, (ID3D11BlendState**)&m_pState)))
		return false;

	return true;
}

void CBlendState::SetState()
{
	//출력 병합기에 현재 State Setting
	//---------------------------------------------------------
	CONTEXT->OMGetBlendState((ID3D11BlendState**)&m_pPrevState,
		m_fPrevBlendFactor, &m_iPrevSampleMask);

	CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pState,
		m_fBlendFactor, m_iSampleMask);
	//---------------------------------------------------------
}

void CBlendState::ResetState()
{
	//State Reset
	//---------------------------------------------------------
	CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pPrevState,
		m_fPrevBlendFactor, m_iPrevSampleMask);
	SAFE_RELEASE(m_pPrevState);
	//---------------------------------------------------------
}	
