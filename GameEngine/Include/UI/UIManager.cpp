#include "UIManager.h"
#include "../Device.h"

DEFINITION_SINGLE(CUIManager)

CUIManager::CUIManager()
{

}

CUIManager::~CUIManager()
{

}

bool CUIManager::Init()
{
	Resolution tRS = RESOLUTION;

	m_matProj = XMMatrixOrthographicOffCenterLH(0.f,
		(float)tRS.iWidth, 0.f, (float)tRS.iHeight,
		0.f, 3000.f);

	return true;
}
