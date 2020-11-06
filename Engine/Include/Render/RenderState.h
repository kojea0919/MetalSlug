#pragma once

#include "../GameEngine.h"

class CRenderState
{
	friend class CRenderManager;

protected:
	CRenderState();
	virtual ~CRenderState();

protected:
	//���� �������� RenderState�� ���� RenderState
	//--------------------------------------------
	ID3D11DeviceChild* m_pState;
	ID3D11DeviceChild* m_pPrevState;
	//--------------------------------------------
public:
	//RenderState ����
	virtual void SetState() = 0;
	//���� RenderState ����
	virtual void ResetState() = 0;
};

