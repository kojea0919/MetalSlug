#pragma once

#include "../GameEngine.h"

class CRenderManager
{
private:
	//Layer를 관리
	//--------------------------------------------
	vector<class CRenderLayer*>	m_vecRenderLayer2D;
	vector<class CRenderLayer*>	m_vecRenderLayer;
	//--------------------------------------------

	//RenderState 관리 Map
	unordered_map<string, class CRenderState*>	m_mapRenderState;

public:
	bool CreateLayer(const string& strName, int iSortOrder = 0);
	bool CreateLayer2D(const string& strName, int iSortOrder = 0);
	bool Init();
	void AddSceneComponent(class CPrimitiveComponent* pComponent);
	void AddCollider(class CCollider* pCollider);
	void Render(float fTime);

private:
	void Render3D(float fTime);
	void Render2D(float fTime);


public:
	void Clear();

public:
	//BlendState추가 함수
	//-----------------------------------------------------------------------
	bool AddBlendInfo(const string& strName, bool bEnable = true, 
		D3D11_BLEND eSrcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND eDestBlend = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP eBlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND eSrcBlendAlpha = D3D11_BLEND_ONE,
		D3D11_BLEND eDestBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP eBlendAlphaOp = D3D11_BLEND_OP_ADD,
		UINT8 iWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	//-----------------------------------------------------------------------

	//BlendState생성 함수
	//-----------------------------------------------------------------------
	bool CreateBlendState(const string& strName, bool bAlphaCover = false,
		bool bIndependent = false);
	//-----------------------------------------------------------------------

	bool CreateDepthStencil(const string& strName, bool bDepthEnable = true,
		D3D11_DEPTH_WRITE_MASK eDepthWrite = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC eDepthFunc = D3D11_COMPARISON_LESS,
		bool bStencilEnable = false,
		UINT8 iStencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK,
		UINT8 iStencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
		const D3D11_DEPTH_STENCILOP_DESC& tFrontFace = {},
		const D3D11_DEPTH_STENCILOP_DESC& tBackFace = {});

	bool SetState(const string& strName);
	bool ResetState(const string& strName);

	class CRenderState* FindRenderState(const string& strName);

private:
	static bool SortLayer(class CRenderLayer* pSrc,
		class CRenderLayer* pDest);

	DECLARE_SINGLE(CRenderManager)
};

