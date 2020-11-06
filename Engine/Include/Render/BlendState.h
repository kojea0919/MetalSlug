#pragma once
#include "RenderState.h"
class CBlendState : public CRenderState
{
	friend class CRenderManager;

protected:
	CBlendState();
	~CBlendState();

private:
	//생성할 Blend상태에 대한 구조체
	vector<D3D11_RENDER_TARGET_BLEND_DESC>	m_vecDesc;

	UINT		m_iSampleMask;
	UINT		m_iPrevSampleMask;
	float		m_fBlendFactor[4];
	float		m_fPrevBlendFactor[4];

public:
	//D3D11_RENDER_TARGET_BLEND_DESC구조체 Setting해서 추가
	//Default인자는 Alpha Blending
	//--------------------------------------------------------------------
	//C_src * F_src ­ (연산) C_dst * F_dst
	//eSrcBlend : F_src
	//eDestBlend : F_dst
	//eBlendOP : 연산자
	//eSrcBlendAlpha : 알파 성분의 F_src
	//eDestBlendAlpha : 알파 성분의 F_dsg
	void AddBlendInfo(bool bEnable = true,
		D3D11_BLEND eSrcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND eDestBlend = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP eBlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND eSrcBlendAlpha = D3D11_BLEND_ONE,
		D3D11_BLEND eDestBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP eBlendAlphaOp = D3D11_BLEND_OP_ADD,
		UINT8 iWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	//--------------------------------------------------------------------


	//BlendState 생성함수
	bool CreateState(bool bAlphaCover = false, bool bIndependent = false);

public:
	virtual void SetState();
	virtual void ResetState();
};

