#include "RenderState.h"

CRenderState::CRenderState()
	: m_pState(nullptr), m_pPrevState(nullptr)
{
}

CRenderState::~CRenderState()
{
	SAFE_RELEASE(m_pState);
	SAFE_RELEASE(m_pPrevState);
}
