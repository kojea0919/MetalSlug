#pragma once
#include "Camera.h"

class CTestCamera : public CCamera
{
	friend class CGameObject;
	friend class CCameraManager;

protected:
	CTestCamera();
	CTestCamera(const CTestCamera & cam);
	virtual ~CTestCamera();

public:
	void SetNewComponent(CSceneComponent* pNewCom);

	void BackToPlayer();

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CTestCamera* Clone();
};

