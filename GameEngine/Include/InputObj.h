#pragma once

#include "Input.h"

//키가 눌렸을 때 오브젝트에서 처리하는 함수를 등록하는 구조체
//----------------------------------------------
typedef struct _tagInputAxis
{
	char strName[64];
	void* pObj;
	vector<function<void(float, float)>> vecFunc;

	_tagInputAxis()
		: pObj(nullptr)
	{
		memset(strName, 0, 64);
	}
}InputAxis,*PInputAxis;


typedef struct _tagInputActoin
{
	char strName[64];
	void* pObj;
	vector<function<void(float)>> vecFunc[KT_END];

	_tagInputActoin()
		: pObj(nullptr)
	{
		memset(strName, 0, 64);
	}
}InputAction,*PInputAction;
//----------------------------------------------



class CInputObj
{
	friend class CGameObject;

private:
	CInputObj();
	~CInputObj();

private:
	unordered_map<string, PInputAxis>	m_mapAxis;
	unordered_map<string, PInputAction> m_mapAction;

public:
	//Axis,Action 추가함수
	//----------------------------------------------
	template<typename T>
	void AddBindAxis(const string& strName, T* pObj, void(T::* pFunc)(float, float))
	{
		//등록된게 없는 경우는 pass
		//-----------------------------------------------------
		PBindAxis pAxis = GET_SINGLE(CInput)->FindAxis(strName);
		if (!pAxis)
			return;
		//-----------------------------------------------------

		PInputAxis pNewAxis = FindAxis(strName);

		if (!pNewAxis)
		{
			pNewAxis = new InputAxis;

			m_mapAxis.insert(make_pair(strName, pNewAxis));

			GET_SINGLE(CInput)->AddBindAxis<CInputObj>(strName, this, &CInputObj::CallAxisFunction);
		}

		strcpy_s(pNewAxis->strName, pAxis->strName);
		pNewAxis->pObj = pObj;

		pNewAxis->vecFunc.push_back(bind(pFunc, pObj, placeholders::_1, placeholders::_2));
	}

	template<typename T>
	void AddBindAction(const string& strName, KEY_TYPE eType, T* pObj, void (T::* pFunc)(float))
	{
		PBindAction pAction = GET_SINGLE(CInput)->FindAction(strName);

		if (!pAction)
			return;

		PInputAction	pNewAction = FindAction(strName);

		if (!pNewAction)
		{
			pNewAction = new InputAction;

			m_mapAction.insert(make_pair(strName, pNewAction));

			GET_SINGLE(CInput)->AddBindAction<CInputObj>(strName, eType, this, &CInputObj::CallActionFunction);
		}

		strcpy_s(pNewAction->strName, pAction->strName);
		pNewAction->pObj = pObj;

		pNewAction->vecFunc[eType].push_back(bind(pFunc, pObj, placeholders::_1, placeholders::_2));
	}
	//----------------------------------------------

private:
	//Find함수
	//-----------------------------------------
	PInputAxis FindAxis(const string& strName);
	PInputAction FindAction(const string& strName);
	//-----------------------------------------

public:
	//입력에 대한 처리 callback함수 호출
	//--------------------------------------------------------------------
	void CallAxisFunction(const char* pName, float fScale, float fTime);
	void CallActionFunction(const char* pName, KEY_TYPE eType, float fTime);
	//--------------------------------------------------------------------
};

