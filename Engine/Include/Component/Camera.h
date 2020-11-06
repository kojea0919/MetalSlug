#pragma once

#include "SceneComponent.h"

class CCamera : public CSceneComponent
{
	friend class CGameObject;
	friend class CCameraManager;

protected:
	CCamera();
	CCamera(const CCamera & com);
	virtual ~CCamera();

protected:
	//카메라 타입(3D,2D,UI)
	CAMERA_TYPE m_eCameraType;

	//View행렬(월드 좌표계 -> 카메라 좌표계)
	Matrix m_matView;

	//투영 행렬
	Matrix m_matProj;

	//절단 공간
	//--------------------------
	float m_fViewAngle;		//시야각
	float m_fDistance;		//거리
	//--------------------------

public:
	CAMERA_TYPE GetCameraType() const
	{
		return m_eCameraType;
	}

	Matrix GetViewMatrix() const
	{
		return m_matView;
	}

	Matrix GetProjMatrix() const
	{
		return m_matProj;
	}

	void SetViewAngle(float fAngle)
	{
		m_fViewAngle = fAngle;

		//카메라 타입이 3D인 경우 Update
		if (m_eCameraType == CAMERA_TYPE::CAM3D)
			SetCameraType(m_eCameraType);
	}
	
	void SetDistance(float fDistance)
	{
		m_fDistance = fDistance;

		SetCameraType(m_eCameraType);
	}

	void SetCameraType(CAMERA_TYPE eType);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CCamera* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

