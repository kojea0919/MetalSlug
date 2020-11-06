#pragma once

#include "RenderState.h"

class CDepthStencilState : public CRenderState
{
	friend class CRenderManager;

protected:
	CDepthStencilState();
	virtual ~CDepthStencilState();

private:
	UINT	m_iStencilRef;
	UINT	m_iPrevStencilRef;

private:
	bool CreateDepthStencil(bool bDepthEnable = true,
		D3D11_DEPTH_WRITE_MASK eDepthWrite = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC eDepthFunc = D3D11_COMPARISON_LESS,
		bool bStencilEnable = false,
		UINT8 iStencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK,
		UINT8 iStencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
		const D3D11_DEPTH_STENCILOP_DESC& tFrontFace = {},
		const D3D11_DEPTH_STENCILOP_DESC& tBackFace = {});

public:
	virtual void SetState();
	virtual void ResetState();
};

