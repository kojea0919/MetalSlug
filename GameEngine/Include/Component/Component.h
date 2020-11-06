#pragma once

#include "../Ref.h"

class CComponent : public CRef
{
	friend class CGameObject;

protected:
	CComponent();
	CComponent(const CComponent& com);
	virtual ~CComponent() = 0;

protected:
	//�����ִ� Scene
	class CScene* m_pScene;
	//�����ϰ��ִ� Object
	class CGameObject* m_pObject;

public:
	//GameObject Get�Լ�
	class CGameObject* GetGameObject() const;

protected:
	//Component�� Ÿ��(Scene,Object)
	COMPONENT_TYPE	m_eComponentType;

	//Component���� ����
	bool			m_bStart;

public:
	bool IsStart() const
	{
		return m_bStart;
	}

	COMPONENT_TYPE GetComponentType() const
	{
		return m_eComponentType;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CComponent* Clone() = 0;

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};