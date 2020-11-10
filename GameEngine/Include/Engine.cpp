#include "Engine.h"
#include "Device.h"
#include "Scene/SceneManager.h"
#include "Timer.h"
#include "PathManager.h"
#include "Resource/ResourceManager.h"
#include "Resource/Mesh2D.h"
#include "Resource/ShaderManager.h"
#include "Resource/Shader.h"
#include "Render/RenderManager.h"
#include "Input.h"
#include "CollisionManager.h"
#include "UI/UIManager.h"

DEFINITION_SINGLE(CEngine)

bool CEngine::m_bLoop = true;

CEngine::CEngine()
{
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(74);

	m_pTimer = nullptr;
	m_pGameInstance = new CGameInstance;
}

CEngine::~CEngine()
{
	CoUninitialize();
	DESTROY_SINGLE(CSceneManager);

	SAFE_DELETE(m_pGameInstance);
	DESTROY_SINGLE(CUIManager);
	DESTROY_SINGLE(CCollisionManager);
	DESTROY_SINGLE(CInput);
	DESTROY_SINGLE(CRenderManager);
	DESTROY_SINGLE(CResourceManager);
	DESTROY_SINGLE(CPathManager);

	SAFE_DELETE(m_pTimer);

	DESTROY_SINGLE(CDevice);
}

// ������ â�� �������� ������ָ鼭 �ʱ�ȭ�� �Ѵ�.
bool CEngine::Init(const TCHAR* pClass, 
	const TCHAR* pTitle, HINSTANCE hInst, int iIconID, 
	int iSmallIconID, int iWidth, int iHeight,
	bool bWindowMode)
{
	m_hInst = hInst;
	m_tRS.iWidth = iWidth;
	m_tRS.iHeight = iHeight;

	Register(pClass, iIconID, iSmallIconID);
	Create(pClass, pTitle);

	return Init(m_hInst, m_hWnd, iWidth, iHeight, bWindowMode);
}

// �̹� ������� ������ â�� ������ �ʱ�ȭ�Ѵ�.
bool CEngine::Init(HINSTANCE hInst, HWND hWnd,
	int iWidth, int iHeight, bool bWindowMode)
{
	m_hWnd = hWnd;
	m_hInst = hInst;
	m_tRS.iWidth = iWidth;
	m_tRS.iHeight = iHeight;

	// ����̽��� �ʱ�ȭ�Ѵ�.
	if (!GET_SINGLE(CDevice)->Init(hWnd, iWidth, iHeight, bWindowMode))
		return false;

	// ��ΰ����� �ʱ�ȭ
	if (!GET_SINGLE(CPathManager)->Init())
		return false;

	// ���ҽ� ������ �ʱ�ȭ
	if (!GET_SINGLE(CResourceManager)->Init())
		return false;

	// ������ ������ �ʱ�ȭ
	if (!GET_SINGLE(CRenderManager)->Init())
		return false;

	// UI������ �ʱ�ȭ
	if (!GET_SINGLE(CUIManager)->Init())
		return false;

	// �Է� ������ �ʱ�ȭ
	if (!GET_SINGLE(CInput)->Init(m_hInst, m_hWnd))
		return false;

	// �浹 ������ �ʱ�ȭ
	if (!GET_SINGLE(CCollisionManager)->Init())
		return false;

	// Ÿ�̸Ӹ� �����Ѵ�.
	m_pTimer = new CTimer;

	m_pTimer->Init();

	m_pTimer->Start();

	// �������ڸ� �ʱ�ȭ�Ѵ�.
	if (!GET_SINGLE(CSceneManager)->Init())
		return false;

	//���콺 ��ü ����
	//GET_SINGLE(CInput)->CreateMouse();

	return true;
}

int CEngine::Run()
{
	MSG msg = {};

	// �⺻ �޽��� �����Դϴ�:
	while (m_bLoop)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CEngine::Logic()
{
	m_pTimer->Update();

	float	fTime = m_pTimer->GetDeltaTime();

	//DeltaTime ������� Update
	//----------------------------------
	DeltaTimeCBuffer tDeltaTimeCBuffer;
	tDeltaTimeCBuffer.m_vDeltaTime = Vector4(fTime * 20,fTime * 20,fTime * 20,1);
	GET_SINGLE(CShaderManager)->UpdateCBuffer("DeltaTime", &tDeltaTimeCBuffer);
	//----------------------------------

	GET_SINGLE(CInput)->Update(fTime);

	if (Update(fTime))
	{
		m_EditorClearCallback();
		return;
	}

	if (PostUpdate(fTime))
	{
		m_EditorClearCallback();
		return;
	}

	if (Collision(fTime))
	{
		m_EditorClearCallback();
		return;
	}

	if (m_EditorUpdateCallback)
		m_EditorUpdateCallback(fTime);

	PrevRender(fTime);
	Render(fTime);
	PostRender(fTime);
}

int CEngine::Update(float fTime)
{
	int iRet = GET_SINGLE(CSceneManager)->Update(fTime);

	return iRet;
}

int CEngine::PostUpdate(float fTime)
{
	int iRet = GET_SINGLE(CSceneManager)->PostUpdate(fTime);

	return iRet;
}

int CEngine::Collision(float fTime)
{
	int iRet = GET_SINGLE(CSceneManager)->Collision(fTime);

	return iRet;
}

int CEngine::PrevRender(float fTime)
{
	GET_SINGLE(CSceneManager)->PrevRender(fTime);

	return 0;
}

int CEngine::Render(float fTime)
{
	GET_SINGLE(CDevice)->ClearTarget();

	// �� ���̿� ����� ó���Ѵ�.
	//GET_SINGLE(CSceneManager)->Render(fTime);
	GET_SINGLE(CRenderManager)->Render(fTime);


	// UI�� ����Ѵ�.
	GET_SINGLE(CSceneManager)->Render(fTime);


	GET_SINGLE(CRenderManager)->SetState("DepthDisable");
	GET_SINGLE(CRenderManager)->SetState("AlphaBlend");

	GET_SINGLE(CInput)->Render(fTime);

	GET_SINGLE(CRenderManager)->ResetState("AlphaBlend");
	GET_SINGLE(CRenderManager)->ResetState("DepthDisable");

	GET_SINGLE(CRenderManager)->Clear();

	GET_SINGLE(CDevice)->Render();

	return 0;
}

int CEngine::PostRender(float fTime)
{
	GET_SINGLE(CSceneManager)->PostRender(fTime);

	return 0;
}

ATOM CEngine::Register(const TCHAR* pClass, int iIconID,
	int iSmallIconID)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(iIconID));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;// MAKEINTRESOURCEW(IDC_MY191101);
	wcex.lpszClassName = pClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iSmallIconID));

	return RegisterClassExW(&wcex);
}

BOOL CEngine::Create(const TCHAR* pClass, 
	const TCHAR* pTitle)
{
	int	x = 0, y = 0;

#ifdef LECTURE
	x = 2000;
	y = 100;
#endif // LECTURE


	m_hWnd = CreateWindowW(pClass, pTitle,
		WS_OVERLAPPEDWINDOW,
		x, y, m_tRS.iWidth,
		m_tRS.iHeight, nullptr, nullptr,
		m_hInst, nullptr);

	if (!m_hWnd)
	{
		return FALSE;
	}

	RECT	rc = { 0, 0, m_tRS.iWidth, m_tRS.iHeight };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(m_hWnd, HWND_TOPMOST, x, y,
		rc.right - rc.left, rc.bottom - rc.top,
		SWP_NOZORDER | SWP_NOMOVE);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return TRUE;
}

LRESULT CEngine::WndProc(HWND hWnd, UINT msg, 
	WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		m_bLoop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
