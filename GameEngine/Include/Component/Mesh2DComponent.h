#pragma once

#include "MeshComponent.h"

class CMesh2DComponent : public CMeshComponent
{
	friend class CGameObject;

protected:
	CMesh2DComponent();
	CMesh2DComponent(const CMesh2DComponent& com);
	virtual ~CMesh2DComponent();

protected:
	class CMesh2D* m_pMesh;

public:
	class CMesh2D* GetMesh() const;
	void SetMesh(class CMesh2D* pMesh);
	void SetMesh(const string& strMeshName);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CMesh2DComponent* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};