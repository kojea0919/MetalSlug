#pragma once

#include "SceneComponent.h"

class CPrimitiveComponent : public CSceneComponent
{
	friend class CGameObject;

protected:
	CPrimitiveComponent();
	CPrimitiveComponent(const CPrimitiveComponent& com);
	virtual ~CPrimitiveComponent();

protected:
	string		m_strLayer;

public:
	string GetLayerName() const
	{
		return m_strLayer;
	}

	void SetLayer(const string& strName)
	{
		m_strLayer = strName;
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
	virtual CPrimitiveComponent* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

};

