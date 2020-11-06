#include "InputObj.h"
#include "Input.h"

CInputObj::CInputObj()
{
}

CInputObj::~CInputObj()
{
	{
		auto iter = m_mapAxis.begin();
		auto iterEnd = m_mapAxis.end();

		for (; iter != iterEnd; ++iter)
		{
			GET_SINGLE(CInput)->DeleteAxisFunction(iter->first, this);
		}
	}

	{
		auto iter = m_mapAction.begin();
		auto iterEnd = m_mapAction.end();

		for (; iter != iterEnd; ++iter)
		{
			for (size_t iCnt = 0; iCnt < KT_END; ++iCnt)
			{
				GET_SINGLE(CInput)->DeleteActionFunction(iter->first, this,(KEY_TYPE)iCnt);
			}
		}

		SAFE_DELETE_MAP(m_mapAxis);
		SAFE_DELETE_MAP(m_mapAction);
	}
}

PInputAxis CInputObj::FindAxis(const string& strName)
{
	auto iter = m_mapAxis.find(strName);

	if (iter == m_mapAxis.end())
		return nullptr;

	return iter->second;
}

PInputAction CInputObj::FindAction(const string& strName)
{
	auto iter = m_mapAction.find(strName);

	if (iter == m_mapAction.end())
		return nullptr;

	return iter->second;
}

void CInputObj::CallAxisFunction(const char* pName, float fScale, float fTime)
{
	//Input클래스에서 해당 키가 눌리면 호출
	//-----------------------------------------
	PInputAxis pAxis = FindAxis(pName);

	if (!pAxis)
		return;

	size_t iLen = pAxis->vecFunc.size();
	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
	{
		pAxis->vecFunc[iCnt](fScale, fTime);
	}
	//-----------------------------------------
}

void CInputObj::CallActionFunction(const char* pName, KEY_TYPE eType, float fTime)
{
	//Input클래스에서 해당 키가 눌리면 호출
	//-----------------------------------------
	PInputAction pAction = FindAction(pName);

	if (!pAction)
		return;

	size_t iLen = pAction->vecFunc[eType].size();
	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
	{
		pAction->vecFunc[eType][iCnt](fTime);
	}
	//-----------------------------------------
}
