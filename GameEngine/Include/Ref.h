#pragma once

#include "Engine.h"

class CRef
{
protected:
	CRef();
	CRef(const CRef& ref);
	virtual ~CRef();

protected:
	string		m_strName;
	int			m_iRefCount;
	bool		m_bActive;
	bool		m_bEnable;

public:
	void AddRef();
	int Release();

public:
	//Get,Set함수
	//------------------
	void SetName(const string& strName)
	{
		m_strName = strName;
	}
	string GetName() const
	{
		return m_strName;
	}
	bool IsEnable() const
	{
		return m_bEnable;
	}
	bool IsActive()	const
	{
		return m_bActive;
	}
	void Enable(bool bEnable)
	{
		m_bEnable = bEnable;
	}
	void Destroy()
	{
		m_bActive = false;
	}
	int GetRefCount() const
	{
		return m_iRefCount;
	}
	//------------------


public:
	//Save,Load함수
	//----------------------------
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	//----------------------------
};

