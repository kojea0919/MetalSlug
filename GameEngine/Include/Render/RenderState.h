#pragma once

#include "../GameEngine.h"

class CRenderState
{
	friend class CRenderManager;

protected:
	CRenderState();
	virtual ~CRenderState();

protected:
	//현재 적용중인 RenderState와 이전 RenderState
	//--------------------------------------------
	ID3D11DeviceChild* m_pState;
	ID3D11DeviceChild* m_pPrevState;
	//--------------------------------------------
public:
	//RenderState 적용
	virtual void SetState() = 0;
	//원래 RenderState 적용
	virtual void ResetState() = 0;
};

