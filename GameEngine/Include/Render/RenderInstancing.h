#pragma once

#include "../GameEngine.h"

class CRenderInstancing
{
public:
	CRenderInstancing();
	~CRenderInstancing();

private:
	//Instacning할 Object들이 사용하는
	//공용 Mesh,Material
	//------------------------------------
	class CMesh*			m_pMesh;
	class CMaterial*		m_pMaterial;
	//------------------------------------

	//오브젝트들을 Instancing하기위한 Shader를 따로 관리
	class CShader*			m_pShader;

	//Instancing Data를 저장할 버퍼
	//------------------------------------------
	VertexBuffer			m_tInstancingBuffer;
	int						m_iCount;
	//------------------------------------------

	bool					m_b2D;


public:
	bool Is2D() const
	{
		return m_b2D;
	}

	bool CheckMesh(class CMesh* pMesh)
	{
		return m_pMesh == pMesh;
	}

	bool CheckMaterial(class CMaterial* pMaterial)
	{
		return m_pMaterial == pMaterial;
	}

	void SetMaterial(class CMaterial* pMaterial);

public:
	//pMesh, pMaterial : Object들이 공용으로 사용하는 Mesh와 Material
	//iSize : Instancing Data의 크기(2D,3D 판정)
	//strShaderName : 해당 RenderInstancing에서 사용할 인스턴싱용 Shader
	//-------------------------------------------------------
	bool Init(class CMesh* pMesh, class CMaterial* pMaterial,
		int iSize, const string & strShaderName);
	//-------------------------------------------------------

	void AddInstancingData(const void* pData);
	void Render(float fTime);
	void Clear();
};

