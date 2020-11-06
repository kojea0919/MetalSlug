#pragma once

#include "../Engine.h"
#include "Scene.h"

class CSceneManager
{
private:
	CScene* m_pScene;
	CScene* m_pSceneNext;
	bool	m_bChange;

	function<void(class CGameObject*)>	m_EditorCreateFunc;
	function<class CGameObject* (int)>	m_CreateObjectFunction;
	function<class CGameMode* (int)>	m_CreateGameModeFunction;

public:
	void SetSequenceFileName(const char* pFileName)
	{
		if(m_pScene)
			m_pScene->SetSequenceFileName(pFileName);
	}

	void AddSequenceResource(const char* pFileName)
	{
		if (m_pScene)
			m_pScene->AddSequenceResource(pFileName);
	}

public:
	bool IsChange()	const
	{
		return m_bChange;
	}

	class CScene* GetScene()	const
	{
		return m_pScene;
	}
	
	class CScene* GetNextScene() const
	{
		return m_pSceneNext;
	}

public:
	bool Init();
	bool Update(float fTime);
	bool PostUpdate(float fTime);
	bool Collision(float fTime);
	void PrevRender(float fTime);
	void Render(float fTime);
	void PostRender(float fTime);

private:
	bool ChangeScene();

public:
	class CScene* CreateNextScene();

public:
	void CreateObjectCallback(class CGameObject* pObj);
	class CGameObject* LoadObjectCallback(int iObjType);
	class CGameMode* LoadGameModeCallback(int iGameModeType);

	template <typename T>
	void SetEditorCreateFunction(T* pObj, void(T::* pFunc)(CGameObject*))
	{
		m_EditorCreateFunc = bind(pFunc, pObj, placeholders::_1);
	}

	template <typename T>
	bool SetGameMode(bool bCurrent = true)
	{
		if (bCurrent)
			return m_pScene->SetGameMode<T>();

		return m_pSceneNext->SetGameMode<T>();
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

public:
	void SaveScene(const char* pFileName, const string& strPathName = DATA_PATH);
	void SaveSceneFullPath(const char* pFullPath);
	void LoadScene(const char* pFileName, const string& strPathName = DATA_PATH);
	void LoadSceneFullPath(const char* pFullPath);

	DECLARE_SINGLE(CSceneManager)
};

