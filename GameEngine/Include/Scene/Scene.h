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
	
	//Scene���� �����ϴ� ��
	//------------------------------------
	//Resource ������
	class CSceneResource* m_pResource;
	
	//ī�޶� ������
	class CCameraManager* m_pCameraManager;

	//�浹 ������
	class CSceneCollision* m_pCollisionManager;
	
	//UI Viewport
	class CUIViewport* m_pUIViewport;
	//------------------------------------

	//�ش� Scene���� ����ϴ� Sequence List�� �����ϴ� �����̸�
	char m_strAnimSequenceFileName[MAX_PATH];

	//Sequece List
	vector<string>			m_vecAniFileName;


public:
	//Set�Լ�
	//------------------------------------
	void SetSequenceFileName(const char* pFileName);

	void AddSequenceResource(const char* pFileName);
	//------------------------------------

public:
	//Get�Լ�
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
	//Scene�� ���� Object List
	list<class CGameObject*>	m_ObjList;

	//�����Ϳ��� Object������ ȣ���� �Լ��� ���
	function<void(class CGameObject*)>	m_EditorCreateFunc;

	//����i���� UI������ ȣ���� �Լ��� ���
	function<void(class CUIObject*)>	m_EditorUICreateFunc;

	//�����Ϳ��� �ҷ����⸦ �� �� GameObject,GameMode�� ��ӹ޾�
	//������ Ŭ���� Ÿ���� �����Ͽ� �ּҸ� ��ȯ���ִ� �Լ��� ���
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
	//�ش� Scene���� ����ϴ� Sequence Load�ϱ�
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

