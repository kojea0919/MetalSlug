#include "Ref.h"

CRef::CRef()
	: m_iRefCount(1),m_bActive(true),
	m_bEnable(true)
{
}

CRef::CRef(const CRef& ref)
{
	*this = ref;
	m_iRefCount = 1;
}

CRef::~CRef()
{
}

void CRef::AddRef()
{
	++m_iRefCount;
}

int CRef::Release()
{
	--m_iRefCount;


	if (m_iRefCount == 0)
	{
		delete this;
		return 0;
	}

	return m_iRefCount;
}

void CRef::Save(FILE* pFile)
{
	fwrite(&m_bEnable, sizeof(m_bEnable), 1, pFile);

	int iLen = (int)m_strName.length();
	fwrite(&iLen, sizeof(iLen), 1, pFile);

	if (iLen > 0)
		fwrite(m_strName.c_str(), 1, iLen, pFile);
}

void CRef::Load(FILE* pFile)
{
	fread(&m_bEnable, sizeof(m_bEnable), 1, pFile);
	
	int iLen = 0;
	char strName[256] = {};
	fread(&iLen, sizeof(iLen), 1, pFile);
	if (iLen > 0)
	{
		fread(strName, 1, iLen, pFile);
		m_strName = strName;
	}

}
