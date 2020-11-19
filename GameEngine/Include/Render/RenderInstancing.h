#pragma once

#include "../GameEngine.h"

class CRenderInstancing
{
public:
	CRenderInstancing();
	~CRenderInstancing();

private:
	//Instacning�� Object���� ����ϴ�
	//���� Mesh,Material
	//------------------------------------
	class CMesh*			m_pMesh;
	class CMaterial*		m_pMaterial;
	//------------------------------------

	//������Ʈ���� Instancing�ϱ����� Shader�� ���� ����
	class CShader*			m_pShader;

	//Instancing Data�� ������ ����
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
	//pMesh, pMaterial : Object���� �������� ����ϴ� Mesh�� Material
	//iSize : Instancing Data�� ũ��(2D,3D ����)
	//strShaderName : �ش� RenderInstancing���� ����� �ν��Ͻ̿� Shader
	//-------------------------------------------------------
	bool Init(class CMesh* pMesh, class CMaterial* pMaterial,
		int iSize, const string & strShaderName);
	//-------------------------------------------------------

	void AddInstancingData(const void* pData);
	void Render(float fTime);
	void Clear();
};

