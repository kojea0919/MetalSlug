#include "DepthStencilState.h"
#include "../Device.h"

CDepthStencilState::CDepthStencilState()
{
    m_iStencilRef = 0xffffffff;
    m_iPrevStencilRef = 0xffffffff;
}

CDepthStencilState::~CDepthStencilState()
{
}

bool CDepthStencilState::CreateDepthStencil(bool bDepthEnable, D3D11_DEPTH_WRITE_MASK eDepthWrite, D3D11_COMPARISON_FUNC eDepthFunc, bool bStencilEnable, UINT8 iStencilReadMask, UINT8 iStencilWriteMask, const D3D11_DEPTH_STENCILOP_DESC& tFrontFace, const D3D11_DEPTH_STENCILOP_DESC& tBackFace)
{
	D3D11_DEPTH_STENCIL_DESC	tDesc = {};
	tDesc.DepthEnable = bDepthEnable;
	tDesc.DepthWriteMask = eDepthWrite;
	tDesc.DepthFunc = eDepthFunc;
	tDesc.StencilEnable = bStencilEnable;
	tDesc.StencilReadMask = iStencilReadMask;
	tDesc.StencilWriteMask = iStencilWriteMask;
	tDesc.FrontFace = tFrontFace;
	tDesc.BackFace = tBackFace;

	if (FAILED(DEVICE->CreateDepthStencilState(&tDesc, (ID3D11DepthStencilState**)&m_pState)))
		return false;

	return true;
}

void CDepthStencilState::SetState()
{
	CONTEXT->OMGetDepthStencilState((ID3D11DepthStencilState**)&m_pPrevState,
		&m_iPrevStencilRef);
	CONTEXT->OMSetDepthStencilState((ID3D11DepthStencilState*)m_pState,
		m_iStencilRef);
}

void CDepthStencilState::ResetState()
{
	CONTEXT->OMSetDepthStencilState((ID3D11DepthStencilState*)m_pPrevState,
		m_iPrevStencilRef);
	SAFE_RELEASE(m_pPrevState);
}
