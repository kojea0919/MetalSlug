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
	//ī�޶� Ÿ��(3D,2D,UI)
	CAMERA_TYPE m_eCameraType;

	//View���(���� ��ǥ�� -> ī�޶� ��ǥ��)
	Matrix m_matView;

	//���� ���
	Matrix m_matProj;

	//���� ����
	//--------------------------
	float m_fViewAngle;		//�þ߰�
	float m_fDistance;		//�Ÿ�
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

		//ī�޶� Ÿ���� 3D�� ��� Update
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

