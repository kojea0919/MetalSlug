#pragma once

#include "GameEngine.h"

#define DIK_MOUSELBUTTON	0xfc
#define DIK_MOUSERBUTTON	0xfd
#define DIK_MOUSEWHEEL		0xfe

#define MOUSEWHEELUP		0xdd
#define MOUSEWHEELDOWN		0xde

//Key입력 종류 열거형
//-------------------
enum KEY_TYPE
{
	KT_DOWN,
	KT_UP,
	KT_DBLCLICK,
	KT_END
};
//-------------------

//조합 Key
//-------------------
enum KEY_UNION
{
	KU_CONTROL,
	KU_ALT,
	KU_SHIFT,
	KU_END
};
//-------------------

enum MOUSE_INFO_TYPE
{
	MIT_LBUTTON,
	MIT_RBUTTON,
	MIT_END
};

//입력으로 사용할 Key정보를 이 구조체를 사용해서 등록하고
//눌렸는지, 눌렸으면 눌린 시간 정보를 가지고 있는다.
//------------------------------------------------------
typedef struct _tagKeyInfo
{
	char		cKey;
	float		fPushTime;
	bool		bPush;
	int			iRefCount;

	_tagKeyInfo()
		: iRefCount(0), cKey(0), fPushTime(0.f), bPush(false)
	{
	}
}KeyInfo, * PKeyInfo;
//------------------------------------------------------

//InputObj Axis에 등록한 함수를 호출해주는 함수를 등록
//------------------------------------------------------
typedef struct _tagObjBindAxisFunc
{
	void* pObj;
	function<void(const char*, float, float)>	Func;

	_tagObjBindAxisFunc()
		: pObj(nullptr)
	{
	}
}ObjBindAxisFunc,&PObjBindAxisFunc;
//------------------------------------------------------

//입력 중에서 Scale값이 필요한 Key정보를 가지는 구조체
//------------------------------------------------------
typedef struct _tagKeyAxis
{
	PKeyInfo	pInfo;
	float		fScale;

	_tagKeyAxis()
		: pInfo(nullptr), fScale(0.f)
	{
	}
}KeyAxis, * PKeyAxis;
//------------------------------------------------------

//KeyAxis에 대한 Key가 눌리면 호출할 함수와 Bind해주는 구조체
//------------------------------------------------------
typedef struct _tagBindAxis
{
	char		strName[64];
	float		fScale;
	vector<KeyAxis> vecKey;

	vector<ObjBindAxisFunc> vecCallback;

	int			iRefCount;

	_tagBindAxis()
		: fScale(0.f), iRefCount(0)
	{
		memset(strName, 0, 64);
	}
}BindAxis, * PBindAxis;
//------------------------------------------------------

//InputObj Action에 등록한 함수를 호출해주는 함수를 등록
//------------------------------------------------------
typedef struct _tagObjBindActionFunc
{
	void* pObj;
	function<void(const char*, KEY_TYPE, float)>	Func;

	_tagObjBindActionFunc()
		: pObj(nullptr)
	{
	}
}ObjBindActionFunc, & PObjBindActionFunc;
//------------------------------------------------------

typedef struct _tagKeyAction
{
	PKeyInfo		pInfo;
	bool			bKeyUnion[KU_END];
	bool			bPush;
	float			fPushTime;

	_tagKeyAction()
		: pInfo(nullptr), bPush(false), fPushTime(0.f)
	{
		memset(bKeyUnion, 0, KU_END);
	}
}KeyAction, * PKeyAction;


typedef struct _tagBindAction
{
	char		strName[64];
	vector<KeyAction> vecKey;

	vector<ObjBindActionFunc>	vecCallback[KT_END];

	void*		pObj;
	int			iRefCount;

	_tagBindAction()
		: pObj(nullptr), iRefCount(0)
	{
		memset(strName, 0, 64);
	}
}BindAction, * PBindAction;


//DInput을 지원하는 경우 DInput을 사용하기 위한 열거형
//--------------------
enum class INPUT_TYPE
{
	WINDOW,
	DINPUT
};
//--------------------

class CInput
{
	friend class CInputObj;

private:
	//DInput Interface
	//----------------------------
	IDirectInput8* m_pInput;
	IDirectInputDevice8* m_pKeyboard;
	IDirectInputDevice8* m_pMouse;
	//----------------------------

	//DInput사용시 받아올 키 배열
	unsigned char			m_KeyState[256];

	INPUT_TYPE				m_eInputType;
	DIMOUSESTATE			m_tMouseState;

private:
	//등록한 Key관리 List
	list<PKeyInfo>			m_KeyList;

	//등록한 입력,callback함수 관리 map
	//--------------------------------------------
	unordered_map<string, PBindAxis>	m_mapAxis;
	unordered_map<string, PBindAction>	m_mapAction;
	//--------------------------------------------

	//마우스
	//--------------------------
	class CMouseObj*	m_pMouseObj;
	Vector2				m_vMousePos;
	Vector2				m_vMouseWorldPos;
	Vector2				m_vMouseMove;
	bool				m_bLButtonClick;
	bool				m_bRButtonClick;

	bool				m_bShowCursor;
	//--------------------------

	HWND		m_hWnd;

public:
	bool IsLButtonClick() const
	{
		return m_bLButtonClick;
	}

	bool IsRButtonClick() const
	{
		return m_bRButtonClick;
	}

	Vector2 GetMousePos() const;

	Vector2 GetMouseWorldPos() const;

	Vector2 GetMouseMove() const
	{
		return m_vMouseMove;
	}

private:
	//Input Type에 따라 각각 초기화하는 함수
	//----------------
	bool InitDInput();
	bool InitWindow();
	//----------------

	//Keyboard, Mouse상태 Read
	//-------------------
	void ReadKeyboard();
	void ReadMouse();
	//-------------------

public:
	void CreateMouse();
	void Start();
	bool Init(HINSTANCE hInst, HWND hWnd);
	void Update(float fTime);
	void Render(float fTime);

private:
	//등록된 Key 입력 여부 Update
	//----------------------------------
	void UpdateWindowKeyInfo(float fTime);
	void UpdateDInputKeyInfo(float fTime);
	//----------------------------------

	//입력된 Key들 처리
	//----------------------------------
	void UpdateAxis(float fTime);
	void UpdateAction(float fTime);
	void UpdateMouse(float fTime);
	//----------------------------------

public:
	//BindAxis 등록,제거
	//------------------------------------------------
	void DeleteAxisKey(const string& strName);
	void DeleteAxisFunction(const string& strName, void* pObj);

	void AddAxisKey(const string& strName, char cKey, float fScale);
	template<typename T>
	void AddBindAxis(const string& strName, T* pObj, void (T::* pFunc)(const char*, float, float))
	{
		PBindAxis pAxis = FindAxis(strName);

		if (!pAxis)
		{
			pAxis = new BindAxis;
			strcpy_s(pAxis->strName, strName.c_str());
			m_mapAxis.insert(make_pair(strName, pAxis));
		}

		ObjBindAxisFunc tFunc;
		tFunc.pObj = pObj;
		tFunc.Func = bind(pFunc, pObj, placeholders::_1, placeholders::_2,placeholders::_3);
		pAxis->vecCallback.push_back(tFunc);
	}

	//------------------------------------------------

public:
	//BindAction 등록,제거
	//------------------------------------------------
	void DeleteActionKey(const string& strName);
	void DeleteActionFunction(const string& strName, void* pObj, KEY_TYPE eType);

	void AddActionKey(const string& strName, char cKey);
	void AddActionKeyUnion(const string& strName, char cKey, KEY_UNION eType);
	template <typename T>
	void AddBindAction(const string& strName, KEY_TYPE eType, T* pObj, void (T::* pFunc)(const char *, KEY_TYPE, float))
	{
		PBindAction pAction = FindAction(strName);

		if (!pAction)
		{
			pAction = new BindAction;
			strcpy_s(pAction->strName, strName.c_str());
			m_mapAction.insert(make_pair(strName, pAction));
		}

		ObjBindActionFunc tFunc;
		tFunc.pObj = pObj;
		tFunc.Func = bind(pFunc, pObj, placeholders::_1, placeholders::_2, placeholders::_3);
		pAction->vecCallback[eType].push_back(tFunc);
	}
	//------------------------------------------------

private:
	//Find함수
	//-------------------------------------------
	PBindAxis FindAxis(const string& strName);
	PBindAction FindAction(const string& strName);
	//-------------------------------------------

private:
	void LButtonDown(const char* pName, KEY_TYPE eType, float fTime);
	void RButtonDown(const char* pName, KEY_TYPE eType, float fTime);
	void LButtonUp(const char* pName, KEY_TYPE eType, float fTime);
	void RButtonUp(const char* pName, KEY_TYPE eType, float fTime);

public:
	unsigned char ConvertKey(unsigned char cKey);

	DECLARE_SINGLE(CInput)
};
