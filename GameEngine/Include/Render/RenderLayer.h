#pragma once

#include "../GameEngine.h"

class CRenderLayer
{
	friend class CRenderManager;

private:
	CRenderLayer();
	~CRenderLayer();

private:
	string	m_strName;

	//Layer마다 출력 우선도를 Setting
	int		m_iSortOrder;

public:
	string GetName() const
	{
		return m_strName;
	}

	int GetSortOrder() const
	{
		return m_iSortOrder;
	}

public:
	void SetName(const string& strName)
	{
		m_strName = strName;
	}

	void SetSortOrder(int iSortOrder)
	{
		m_iSortOrder = iSortOrder;
	}

private:
	vector<class CPrimitiveComponent*>	m_vecRender;
	list<class CRenderInstancing*>		m_RenderInstancingList;
	bool								m_b2D;

public:
	void Set2D()
	{
		m_b2D = true;
	}

public:
	void AddPrimitiveComponent(class CPrimitiveComponent* pComponent);
	void Render(float fTime);
	void Clear();

private:
	static bool SortY(class CPrimitiveComponent* pSrc,
		class CPrimitiveComponent* pDest);

};

