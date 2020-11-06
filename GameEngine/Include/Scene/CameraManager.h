#pragma once

#include "../GameEngine.h"

class CCameraManager
{
	friend class CScene;

private:
	CCameraManager();
	~CCameraManager();

private:
	class CCamera* m_pMainCamera;
	class CCamera* m_pUICamera;

public:
	class CCamera* GetMainCamera()	const
	{
		return m_pMainCamera;
	}

	class CCamera* GetUICamera()	const
	{
		return m_pUICamera;
	}

public:
	void SetMainCamera(class CCamera* pCamera);
	void SetUICamera(class CCamera* pCamera);

public:
	bool Init();
	void Update(float fTime);
	void PostUpdate(float fTime);
};

