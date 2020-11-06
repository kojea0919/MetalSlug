#pragma once

#include "GameInstance.h"
#include "GameEngine.h"

class CEngine
{
private:
	static bool		m_bLoop;

private:
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	Resolution	m_tRS;

	class CTimer* m_pTimer;
	
	class CGameInstance* m_pGameInstance;

	function<void(float)>	m_EditorUpdateCallback;
	function<void()>	m_EditorClearCallback;

public:
	CGameInstance* GetGameInstance()	const
	{
		return m_pGameInstance;
	}

	HINSTANCE GetWindowInstance()	const
	{
		return m_hInst;
	}

	HWND GetWindowHandle()	const
	{
		return m_hWnd;
	}

	Resolution GetResolution()	const
	{
		return m_tRS;
	}

	bool IsExit()	const
	{
		return !m_bLoop;
	}

public:
	bool Init(const TCHAR* pClass, const TCHAR* pTitle,
		HINSTANCE hInst, int iIconID, int iSmallIconID,
		int iWidth, int iHeight, bool bWindowMode = true);
	bool Init(HINSTANCE hInst, HWND hWnd,
		int iWidth, int iHeight, bool bWindowMode = true);
	int Run();
	void Logic();

private:
	int Update(float fTime);
	int PostUpdate(float fTime);
	int Collision(float fTime);
	int PrevRender(float fTime);
	int Render(float fTime);
	int PostRender(float fTime);

private:
	ATOM Register(const TCHAR* pClass, int iIconID,
		int iSmallIconID);
	BOOL Create(const TCHAR* pClass,
		const TCHAR* pTitle);

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	template <typename T>
	void SetEditorUpdateCallback(T* pObj, void(T::* pFunc)(float))
	{
		m_EditorUpdateCallback = bind(pFunc, pObj, placeholders::_1);
	}

	template <typename T>
	void SetEditorClearCallback(T* pObj, void(T::* pFunc)())
	{
		m_EditorClearCallback = bind(pFunc, pObj);
	}

	template <typename T>
	void SetGameInstance()
	{
		SAFE_DELETE(m_pGameInstance);

		m_pGameInstance = new T;

		if (!m_pGameInstance->Init())
		{
			SAFE_DELETE(m_pGameInstance);
			return;
		}
	}

	DECLARE_SINGLE(CEngine)
};

