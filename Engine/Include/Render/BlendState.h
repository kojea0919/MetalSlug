#pragma once
#include "RenderState.h"
class CBlendState : public CRenderState
{
	friend class CRenderManager;

protected:
	CBlendState();
	~CBlendState();

private:
	//������ Blend���¿� ���� ����ü
	vector<D3D11_RENDER_TARGET_BLEND_DESC>	m_vecDesc;

	UINT		m_iSampleMask;
	UINT		m_iPrevSampleMask;
	float		m_fBlendFactor[4];
	float		m_fPrevBlendFactor[4];

public:
	//D3D11_RENDER_TARGET_BLEND_DESC����ü Setting�ؼ� �߰�
	//Default���ڴ� Alpha Blending
	//--------------------------------------------------------------------
	//C_src * F_src �� (����) C_dst * F_dst
	//eSrcBlend : F_src
	//eDestBlend : F_dst
	//eBlendOP : ������
	//eSrcBlendAlpha : ���� ������ F_src
	//eDestBlendAlpha : ���� ������ F_dsg
	void AddBlendInfo(bool bEnable = true,
		D3D11_BLEND eSrcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND eDestBlend = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP eBlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND eSrcBlendAlpha = D3D11_BLEND_ONE,
		D3D11_BLEND eDestBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP eBlendAlphaOp = D3D11_BLEND_OP_ADD,
		UINT8 iWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	//--------------------------------------------------------------------


	//BlendState �����Լ�
	bool CreateState(bool bAlphaCover = false, bool bIndependent = false);

public:
	virtual void SetState();
	virtual void ResetState();
};

