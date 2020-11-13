#pragma once

#include "../Engine.h"
#include "GameMode.h"
#include "UIViewport.h"

class CScene
{
	friend class CSceneManager;

private:
	CScene();
	~CScene();

private:
	CGameMode* m_pGameMode;
	
	//Scene에서 관리하는 것
	//------------------------------------
	//Resource 관리자
	class CSceneResource* m_pResource;
	
	//카메라 관리자
	class CCameraManager* m_pCameraManager;

	//충돌 관리자
	class CSceneCollision* m_pCollisionManager;
	
	//UI Viewport
	class CUIViewport* m_pUIViewport;
	//------------------------------------

	//해당 Scene에서 사용하는 Sequence List를 관리하는 파일이름
	char m_strAnimSequenceFileName[MAX_PATH];

	//Sequece List
	vector<string>			m_vecAniFileName;


public:
	//Set함수
	//------------------------------------
	void SetSequenceFileName(const char* pFileName);

	void AddSequenceResource(const char* pFileName);
	//------------------------------------

public:
	//Get함수
	//------------------------------------
	CGameMode* GetGameMode()	const
	{
		return m_pGameMode;
	}

	class CSceneResource* GetResourceManager()	const
	{
		return m_pResource;
	}

	class CCameraManager* GetCameraManager()	const
	{
		return m_pCameraManager;
	}

	class CSceneCollision* GetCollisionManager()	const
	{
		return m_pCollisionManager;
	}

	const char* GetAnimSequenceFileName() const
	{
		return m_strAnimSequenceFileName;
	}

	const vector<class CUIControl*>& GetUIVec() const;
	//------------------------------------
private:
	//Scene에 속한 Object List
	list<class CGameObject*>	m_ObjList;

	//에디터에서 Object생성시 호출할 함수를 등록
	function<void(class CGameObject*)>	m_EditorCreateFunc;

	//에디엍에서 UI생성시 호출할 함수를 등록
	function<void(class CUIObject*)>	m_EditorUICreateFunc;

	//에디터에서 불러오기를 할 때 GameObject,GameMode를 상속받아
	//구현한 클래스 타입을 생성하여 주소를 반환해주는 함수를 등록
	//-----------------------------------------------------------
	function<class CGameObject* (int)>	m_CreateObjectFunction;
	function<class CGameMode* (int)>	m_CreateGameModeFunction;
	//-----------------------------------------------------------

	Vector3			m_vWorldStart;
	Vector3			m_vWorldSize;

public:
	Vector3 GetWorldSize() const
	{
		return m_vWorldSize;
	}

	Vector3 GetWorldStart() const
	{
		return m_vWorldStart;
	}

	void SetWorldStart(float x, float y, float z)
	{
		m_vWorldStart = Vector3(x, y, z);
	}

	void SetWorldSize(float x, float y, float z)
	{
		m_vWorldSize = Vector3(x, y, z);
	}

	const list<class CGameObject*>* GetObjList()	const
	{
		return &m_ObjList;
	}

	class CGameObject* FindObject(const string& strName);

public:
	bool Init();
	void Start();
	void Update(float fTime);
	void PostUpdate(float fTime);
	void Collision(float fTime);
	void PrevRender(float fTime);
	void Render(float fTime);
	void PostRender(float fTime);

public:
	template <typename T>
	bool SetGameMode()
	{
		SAFE_DELETE(m_pGameMode);

		m_pGameMode = new T;

		m_pGameMode->m_pScene = this;

		if (!m_pGameMode->Init())
		{
			SAFE_DELETE(m_pGameMode);
			return false;
		}

		return true;
	}

	template <typename T>
	T* CreateObject(const string& strName)
	{
		T* pObj = new T;

		pObj->SetName(strName);
		pObj->m_pScene = this;

		if (!pObj->Init())
		{
			SAFE_RELEASE(pObj);
			return nullptr;
		}

		pObj->AddRef();
		m_ObjList.push_back(pObj);

		if (m_EditorCreateFunc)
			m_EditorCreateFunc(pObj);

		return pObj;
	}

	template<typename T>
	T* CreateUIObject(const string& strName)
	{
		T* pObj = new T;

		pObj->SetName(strName);
		pObj->m_pScene = this;

		if (!pObj->Init())
		{
			SAFE_RELEASE(pObj);
			return nullptr;
		}

		m_pUIViewport->AddUI(pObj);

		if (m_EditorUICreateFunc)
			m_EditorUICreateFunc(pObj);

		return pObj;
	}

	template <typename T>
	void SetEditorCreateFunction(T* pObj, void(T::* pFunc)(CGameObject*))
	{
		m_EditorCreateFunc = bind(pFunc, pObj, placeholders::_1);
	}

	template <typename T>
	void SetEditorUICreateFunction(T* pObj, void(T::* pFunc)(CUIObject*))
	{
		m_EditorUICreateFunc = bind(pFunc, pObj, placeholders::_1);
	}

	template <typename T>
	void SetCreateObjectFunction(T* pObj, class CGameObject* (T::* pFunc)(int))
	{
		m_CreateObjectFunction = bind(pFunc, pObj, placeholders::_1);
	}

	template <typename T>
	void SetCreateGameModeFunction(T* pObj, class CGameMode* (T::* pFunc)(int))
	{
		m_CreateGameModeFunction = bind(pFunc, pObj, placeholders::_1);
	}

private:
	//해당 Scene에서 사용하는 Sequence Load하기
	//--------------------------------------------
	bool LoadAnimation2DSequence();
	bool LoadAnimation2D(FILE * pFile);
	bool LoadAniFile(const char* pAniFileName);
	//--------------------------------------------

	//AnimNotify Setting
	void SetAnimNotify();

public:
	void SaveScene(const char* pFileName, const string& strPathName = DATA_PATH);
	void SaveSceneFullPath(const char* pFullPath);
	void LoadScene(const char* pFileName, const string& strPathName = DATA_PATH);
	void LoadSceneFullPath(const char* pFullPath);
};

