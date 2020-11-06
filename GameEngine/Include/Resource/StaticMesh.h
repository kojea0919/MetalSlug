#pragma once
#include "Mesh.h"
class CStaticMesh :
	public CMesh
{
	friend class CResourceManager;
	friend class CSceneResource;

protected:
	CStaticMesh();
	virtual ~CStaticMesh();

public:
	virtual void Render(float fTime);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

