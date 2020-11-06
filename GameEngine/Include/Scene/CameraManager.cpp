#include "CameraManager.h"
#include "../Component/Camera.h"

CCameraManager::CCameraManager()
{
	// 기본카메라 생성
	m_pMainCamera = new CCamera;
	m_pUICamera = new CCamera;

	m_pUICamera->SetCameraType(CAMERA_TYPE::CAMUI);
}

CCameraManager::~CCameraManager()
{
	SAFE_RELEASE(m_pMainCamera);
	SAFE_RELEASE(m_pUICamera);
}

void CCameraManager::SetMainCamera(CCamera* pCamera)
{
	if (m_pMainCamera == pCamera)
		return;

	SAFE_RELEASE(m_pMainCamera);
	m_pMainCamera = pCamera;

	if (m_pMainCamera)
		m_pMainCamera->AddRef();
}

void CCameraManager::SetUICamera(CCamera* pCamera)
{
	if (m_pUICamera == pCamera)
		return;

	SAFE_RELEASE(m_pUICamera);
	m_pUICamera = pCamera;

	if (m_pUICamera)
		m_pUICamera->AddRef();
}

bool CCameraManager::Init()
{
	return true;
}

void CCameraManager::Update(float fTime)
{
	if (!m_pMainCamera->GetGameObject())
		m_pMainCamera->Update(fTime);

	if (!m_pUICamera->GetGameObject())
		m_pUICamera->Update(fTime);
}

void CCameraManager::PostUpdate(float fTime)
{
	if (!m_pMainCamera->GetGameObject())
		m_pMainCamera->PostUpdate(fTime);

	if (!m_pUICamera->GetGameObject())
		m_pUICamera->PostUpdate(fTime);
}
