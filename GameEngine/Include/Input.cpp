#include "Input.h"
#include "UI/MouseObj.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/CameraManager.h"
#include "Component/Camera.h"

DEFINITION_SINGLE(CInput)

CInput::CInput()
	: m_pInput(nullptr),m_pKeyboard(nullptr),
	m_pMouse(nullptr),m_bLButtonClick(false),
	m_bRButtonClick(false),m_bShowCursor(false)
{

}

CInput::~CInput()
{
	SAFE_RELEASE(m_pMouseObj);
	SAFE_DELETE_VECLIST(m_KeyList);
	SAFE_DELETE_MAP(m_mapAction);
	SAFE_DELETE_MAP(m_mapAxis);

	//��ġ ���� ����
	//---------------------------
	if (m_pKeyboard)
		m_pKeyboard->Unacquire();

	if (m_pMouse)
		m_pMouse->Unacquire();
	//---------------------------

	SAFE_RELEASE(m_pMouse);
	SAFE_RELEASE(m_pKeyboard);
	SAFE_RELEASE(m_pInput);
}

Vector2 CInput::GetMousePos()	const
{
	return m_vMousePos;
}

Vector2 CInput::GetMouseWorldPos()	const
{
	return m_vMouseWorldPos;
}

bool CInput::InitDInput()
{
	//��ġ �ʱ�ȭ
	//-----------------------------------------
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, nullptr)))
		return false;

	if (FAILED(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return false;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return false;
	//-----------------------------------------

	return true;
}

bool CInput::InitWindow()
{
	return true;
}

void CInput::ReadKeyboard()
{
	//�迭�� ���� Ű���� ������ ����
	HRESULT result = m_pKeyboard->GetDeviceState(256, m_KeyState);

	//������ ��� ���� �ٽ� ������
	//---------------------------------------------
	if (FAILED(result))
	{
		//DIERR_INPUTLOST : �Է� ��ġ�� ���� ���� ������ ��������
		//DIERR_NOTACQUIRED : ��ġ�� ������ ���� ���	
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			m_pKeyboard->Acquire();
	}
	//---------------------------------------------
}

void CInput::ReadMouse()
{
	//���콺 ���� ����
	HRESULT result = m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);

	//������ ��� ���� �ٽ� ������
	//---------------------------------------------
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			m_pMouse->Acquire();
	}
	//---------------------------------------------
}

void CInput::CreateMouse()
{
	//���콺 ��ü �����ؼ� Scene�� Setting
	//---------------------------------
	m_pMouseObj = new CMouseObj;

	//m_pMouseObj->m_pScene = GET_SINGLE(CSceneManager)->GetScene();

	if (!m_pMouseObj->Init())
	{
		SAFE_RELEASE(m_pMouseObj);
		return;
	}

	m_pMouseObj->SetRelativePos(m_vMousePos.x, m_vMousePos.y, 0.f);

	ShowCursor(FALSE);


	//---------------------------------
}

void CInput::Start()
{
	if (m_pMouseObj)
		m_pMouseObj->Start();
}

bool CInput::Init(HINSTANCE hInst, HWND hWnd)
{
	m_hWnd = hWnd;

	//DInput ����
	HRESULT hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_pInput, nullptr);

	//DInput�� �����ϸ� DInput�� ����ϱ� ���� Ÿ�� ����
	//--------------------------------------

	m_eInputType = INPUT_TYPE::DINPUT;

	if (FAILED(hr))
		m_eInputType = INPUT_TYPE::WINDOW;

	//--------------------------------------

	//�� Ÿ�Կ� �´� �ʱ�ȭ
	//--------------------------------------
	switch (m_eInputType)
	{
	case INPUT_TYPE::WINDOW:
		if (!InitWindow())
			return false;
		break;
	case INPUT_TYPE::DINPUT:
		if (!InitDInput())
			return false;
		break;
	}
	//--------------------------------------

	//���콺 �Է� ���
	//--------------------------------------
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	m_vMousePos = Vector2((float)ptMouse.x, (float)ptMouse.y);

	AddActionKey("MouseLButton", VK_LBUTTON);
	AddActionKey("MouseRButton", VK_RBUTTON);

	AddBindAction("MoouseLButton", KT_DOWN, this, &CInput::LButtonDown);
	AddBindAction("MoouseLButton", KT_UP, this, &CInput::LButtonUp);

	AddBindAction("MoouseRButton", KT_DOWN, this, &CInput::RButtonDown);
	AddBindAction("MoouseRButton", KT_UP, this, &CInput::RButtonUp);
	//--------------------------------------


	return true;
}

void CInput::Update(float fTime)
{
	//Key�� �Էµƴ��� Ȯ��
	//----------------------------
	switch (m_eInputType)
	{
	case INPUT_TYPE::WINDOW:
		UpdateWindowKeyInfo(fTime);
		break;
	case INPUT_TYPE::DINPUT:
		ReadKeyboard();
		ReadMouse();
		UpdateDInputKeyInfo(fTime);
		break;
	}
	//----------------------------

	//�Էµ� Key ó��
	//----------------------------
	UpdateAxis(fTime);
	UpdateAction(fTime);
	UpdateMouse(fTime);
	//----------------------------
}

void CInput::Render(float fTime)
{
	if (m_pMouseObj)
	{
		m_pMouseObj->Render(fTime);
		m_pMouseObj->PostRender(fTime);
	}
}

void CInput::UpdateWindowKeyInfo(float fTime)
{
	//KeyList�� ��ȸ�Ͽ� ��ϵ� Key�� ���ȴ��� Ȯ��
	//----------------------------------------------
	auto	iter = m_KeyList.begin();
	auto	iterEnd = m_KeyList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (GetAsyncKeyState((*iter)->cKey) & 0x8000)
		{
			(*iter)->bPush = true;
			(*iter)->fPushTime += fTime;
		}

		else
		{
			(*iter)->bPush = false;
			(*iter)->fPushTime = 0.f;
		}
	}
	//----------------------------------------------
}

void CInput::UpdateDInputKeyInfo(float fTime)
{
	//KeyList�� ��ȸ�Ͽ� ��ϵ� Key�� ���ȴ��� Ȯ��
	//----------------------------------------------
	auto	iter = m_KeyList.begin();
	auto	iterEnd = m_KeyList.end();

	for (; iter != iterEnd; ++iter)
	{
		//VK -> DK
		unsigned char	cKey = ConvertKey((*iter)->cKey);

		switch (cKey)
		{
		case DIK_MOUSELBUTTON:
			if (m_tMouseState.rgbButtons[0])
			{
				(*iter)->bPush = true;
				(*iter)->fPushTime += fTime;
			}
			else
			{
				(*iter)->bPush = false;
				(*iter)->fPushTime = 0.f;
			}
			break;
		case DIK_MOUSERBUTTON:
			if (m_tMouseState.rgbButtons[1])
			{
				(*iter)->bPush = true;
				(*iter)->fPushTime += fTime;
			}

			else
			{
				(*iter)->bPush = false;
				(*iter)->fPushTime = 0.f;
			}
			break;
		case DIK_MOUSEWHEEL:
			break;
		default:
			if (m_KeyState[cKey] & 0x80)
			{
				(*iter)->bPush = true;
				(*iter)->fPushTime += fTime;
			}

			else
			{
				(*iter)->bPush = false;
				(*iter)->fPushTime = 0.f;
			}
			break;
		}
	}
	//----------------------------------------------
}

void CInput::UpdateAxis(float fTime)
{
	auto iter = m_mapAxis.begin();
	auto iterEnd = m_mapAxis.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->fScale = 0.f;

		//�ش� Axis�� ��ϵ� Ű���� ��ȸ�Ͽ� ���� Key�鿡 ����
		//Scale���� ����
		//---------------------------------------------------------
		size_t iKeySize = iter->second->vecKey.size();
		
		for (size_t iCnt = 0; iCnt < iKeySize; ++iCnt)
		{
			if (iter->second->vecKey[iCnt].pInfo->bPush)
				iter->second->fScale += iter->second->vecKey[iCnt].fScale;
		}
		//---------------------------------------------------------

		//callback�Լ� ȣ��
		//---------------------------------------------------------
		size_t iCallbackSize = iter->second->vecCallback.size();

		for (size_t iCnt = 0; iCnt < iCallbackSize; ++iCnt)
		{
			iter->second->vecCallback[iCnt].Func(iter->first.c_str(),iter->second->fScale,fTime);
		}
		//---------------------------------------------------------
	}
}

void CInput::UpdateAction(float fTime)
{
	auto iter = m_mapAction.begin();
	auto iterEnd = m_mapAction.end();

	for (; iter != iterEnd; ++iter)
	{
		size_t iKeySize = iter->second->vecKey.size();

		for (size_t iCnt = 0; iCnt < iKeySize; ++iCnt)
		{
			//���� ��� ó�� ���ȴ��� Ȯ��
			//---------------------------------------------
			if (iter->second->vecKey[iCnt].pInfo->bPush)
			{
				if (!iter->second->vecKey[iCnt].bPush)
				{
					iter->second->vecKey[iCnt].bPush = true;
					
					size_t iCallbackSize = iter->second->vecCallback[KT_DOWN].size();

					for (size_t iCallbackCnt = 0; iCallbackCnt < iCallbackSize; ++iCallbackCnt)
					{
						iter->second->vecCallback[KT_DOWN][iCallbackCnt].Func(iter->first.c_str(),KT_DOWN,fTime);
					}
				}
			}
			//---------------------------------------------

			//������ ���� ��� �����ٰ� ������ �������� Ȯ��
			//---------------------------------------------
			else if (iter->second->vecKey[iCnt].bPush)
			{
				iter->second->vecKey[iCnt].bPush = false;

				size_t	iCallbackSize = iter->second->vecCallback[KT_UP].size();

				for (size_t iCallbackCnt = 0; iCallbackCnt < iCallbackSize; ++iCallbackCnt)
				{
					iter->second->vecCallback[KT_UP][iCallbackCnt].Func(iter->first.c_str(),KT_UP,fTime);
				}
			}
			//---------------------------------------------
		}
	}
}

void CInput::UpdateMouse(float fTime)
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	//������ â ũ�� Setting
	//---------------------------
	RECT rc = {};
	GetClientRect(m_hWnd, &rc);
	//---------------------------

	//���콺�� ������ â�ȿ� �ִ� ��쿡�� Ŀ���� �ȱ׸����� Setting
	//---------------------------------------------
	if ((ptMouse.x < 0 || ptMouse.x > rc.right - rc.left ||
		ptMouse.y < 0 || ptMouse.y > rc.bottom - rc.top) &&
		!m_bShowCursor)
	{
		ShowCursor(TRUE);
		m_bShowCursor = true;
	}

	else if (m_bShowCursor && (ptMouse.x >= 0 && ptMouse.x <= rc.right - rc.left &&
		ptMouse.y >= 0 && ptMouse.y <= rc.bottom - rc.top))
	{
		m_bShowCursor = false;
		ShowCursor(FALSE);
	}
	//---------------------------------------------

	//���콺 ȭ��� ��ġ Setting
	//---------------------------------------------
	Vector2 vRatio = GET_SINGLE(CDevice)->GetWindowRatio();

	Vector2 vCurMouse = Vector2((float)ptMouse.x, (float)ptMouse.y);
	vCurMouse *= vRatio;

	//y��ǥ ������
	vCurMouse.y = RESOLUTION.iHeight - vCurMouse.y;

	m_vMouseMove = vCurMouse - m_vMousePos;

	m_vMousePos = vCurMouse;

	if (m_pMouseObj)
		m_pMouseObj->SetRelativePos(vCurMouse.x, vCurMouse.y, 0.f);
	//---------------------------------------------

	//���콺 ����� ��ġ Setting
	//---------------------------------------------
	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();
	CCamera* pCamera = pScene->GetCameraManager()->GetMainCamera();

	Vector3 vCameraPos = pCamera->GetWorldPos();

	m_vMouseWorldPos = vCurMouse + Vector2(vCameraPos.x, vCameraPos.y);
	//---------------------------------------------

	if (m_pMouseObj)
	{
		m_pMouseObj->Update(fTime);
		m_pMouseObj->PostUpdate(fTime);
	}
}

void CInput::DeleteAxisKey(const string& strName)
{
	auto	iter = m_mapAxis.find(strName);

	if (iter == m_mapAxis.end())
		return;

	auto KeyListiter = m_KeyList.begin();
	auto KeyListiterEnd = m_KeyList.end();

	for (; KeyListiter != KeyListiterEnd; ++KeyListiter)
	{
		//������� AxisKey�� ��ϵ� Key���� ���� Count�� ��� ���δ�.
		//check ���⼭ ���� ã�� �ʿ���� DeleteKeyiter RefCount�� ��� �ٿ��ָ� �����ʳ�?
		//������ AddAxisKey������ ������ ����Ű���� �ϴµ�
		//------------------------------------------------------------
		auto DeleteKeyiter = iter->second->vecKey.begin();
		auto DeleteKeyiterEnd = iter->second->vecKey.end();

		for (; DeleteKeyiter != DeleteKeyiterEnd; ++DeleteKeyiter)
		{
			if ((*KeyListiter)->cKey == (*DeleteKeyiter).pInfo->cKey)
			{
				--(*KeyListiter)->iRefCount;


			}
		}
		//------------------------------------------------------------
	}

	//Axis�� ���� Count�� ���̰� ���̻� �������� ������ �����Ѵ�.
	//------------------------------------------
	--iter->second->iRefCount;

	if (iter->second->iRefCount == 0)
	{
		SAFE_DELETE(iter->second);

		m_mapAxis.erase(iter);
	}
	//------------------------------------------
}

void CInput::DeleteAxisFunction(const string& strName, void* pObj)
{
	//�ش� Axis�� �����ϴ� ��쿡�� ����
	//---------------------------------------
	PBindAxis pAxis = FindAxis(strName);
	if (!pAxis)
		return;
	//---------------------------------------

	//Action�� �����ϴ� ��� �ش� Object�� ����� �Լ��� ����
	//---------------------------------------
	size_t iLen = pAxis->vecCallback.size();

	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
	{
		if (pAxis->vecCallback[iCnt].pObj == pObj)
		{
			auto iter = pAxis->vecCallback.begin() + iCnt;
			pAxis->vecCallback.erase(iter);
			break;
		}
	}
	//---------------------------------------
}

void CInput::AddAxisKey(const string& strName, char cKey, float fScale)
{
	PBindAxis pAxis = FindAxis(strName);

	if (!pAxis)
	{
		pAxis = new BindAxis;
		strcpy_s(pAxis->strName, strName.c_str());

		m_mapAxis.insert(make_pair(strName, pAxis));
	}

	//��ϵ��� ���� Key�� ��� ���ο� KeyInfo�� �����Ѵ�.
	//---------------------------------------------
	PKeyInfo	pInfo = nullptr;

	auto iter = m_KeyList.begin();
	auto iterEnd = m_KeyList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->cKey == cKey)
		{
			pInfo = *iter;
			break;
		}
	}

	if (!pInfo)
	{
		pInfo = new KeyInfo;
		pInfo->cKey = cKey;
		m_KeyList.push_back(pInfo);
	}

	++pInfo->iRefCount;
	//---------------------------------------------

	KeyAxis tKey = {};
	tKey.pInfo = pInfo;
	tKey.fScale = fScale;
	pAxis->vecKey.push_back(tKey);
}

void CInput::DeleteActionKey(const string& strName)
{
	auto	iter = m_mapAction.find(strName);

	if (iter == m_mapAction.end())
		return;

	SAFE_DELETE(iter->second);

	m_mapAction.erase(iter);
}

void CInput::DeleteActionFunction(const string& strName, void* pObj, KEY_TYPE eType)
{
	//�ش� Action�� �����ϴ� ��쿡�� ����
	//---------------------------------------
	PBindAction pAction = FindAction(strName);
	if (!pAction)
		return;
	//---------------------------------------

	//Action�� �����ϴ� ��� �ش� Object�� ����� �Լ��� ����
	//---------------------------------------
	size_t iLen = pAction->vecCallback[eType].size();

	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
	{
		if (pAction->vecCallback[eType][iCnt].pObj == pObj)
		{
			auto iter = pAction->vecCallback[eType].begin() + iCnt;
			pAction->vecCallback[eType].erase(iter);
			break;
		}
	}
	//---------------------------------------
}

void CInput::AddActionKey(const string& strName, char cKey)
{
	PBindAction	pAction = FindAction(strName);

	if (!pAction)
	{
		pAction = new BindAction;
		m_mapAction.insert(make_pair(strName, pAction));

		strcpy_s(pAction->strName, strName.c_str());
	}

	PKeyInfo	pInfo = nullptr;

	auto	iter = m_KeyList.begin();
	auto	iterEnd = m_KeyList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->cKey == cKey)
		{
			pInfo = *iter;
			break;
		}
	}

	if (!pInfo)
	{
		pInfo = new KeyInfo;
		pInfo->cKey = cKey;
		m_KeyList.push_back(pInfo);
	}

	KeyAction	tKey = {};
	tKey.pInfo = pInfo;
	pAction->vecKey.push_back(tKey);
}

void CInput::AddActionKeyUnion(const string& strName, char cKey, KEY_UNION eType)
{
}

PBindAxis CInput::FindAxis(const string& strName)
{
	auto iter = m_mapAxis.find(strName);

	if (iter == m_mapAxis.end())
		return nullptr;

	return iter->second;
}

PBindAction CInput::FindAction(const string& strName)
{
	auto iter = m_mapAction.find(strName);

	if (iter == m_mapAction.end())
		return nullptr;

	return iter->second;
}

void CInput::LButtonDown(const char * pName, KEY_TYPE eType, float fTime)
{
	m_bLButtonClick = true;
}

void CInput::RButtonDown(const char* pName, KEY_TYPE eType, float fTime)
{
	m_bRButtonClick = true;
}

void CInput::LButtonUp(const char* pName, KEY_TYPE eType, float fTime)
{
	m_bLButtonClick = false;
}

void CInput::RButtonUp(const char* pName, KEY_TYPE eType, float fTime)
{
	m_bRButtonClick = false;
}

unsigned char CInput::ConvertKey(unsigned char cKey)
{
	if (m_eInputType == INPUT_TYPE::DINPUT)
	{
		switch (cKey)
		{
		case VK_ESCAPE:
			return DIK_ESCAPE;
		case '1':
			return DIK_1;
		case '2':
			return DIK_2;
		case '3':
			return DIK_3;
		case '4':
			return DIK_4;
		case '5':
			return DIK_5;
		case '6':
			return DIK_6;
		case '7':
			return DIK_7;
		case '8':
			return DIK_8;
		case '9':
			return DIK_9;
		case '0':
			return DIK_0;
		case VK_OEM_MINUS:
			return DIK_MINUS;
		case '=':
			return DIK_EQUALS;
		case VK_BACK:
			return DIK_BACK;
		case VK_TAB:
			return DIK_TAB;
		case 'Q':
			return DIK_Q;
		case 'W':
			return DIK_W;
		case 'E':
			return DIK_E;
		case 'R':
			return DIK_R;
		case 'T':
			return DIK_T;
		case 'Y':
			return DIK_Y;
		case 'U':
			return DIK_U;
		case 'I':
			return DIK_I;
		case 'O':
			return DIK_O;
		case 'P':
			return DIK_P;
		case '[':
			return DIK_LBRACKET;
		case ']':
			return DIK_RBRACKET;
		case VK_RETURN:
			return DIK_RETURN;
		case VK_CONTROL:
			return DIK_LCONTROL;
		case 'A':
			return DIK_A;
		case 'S':
			return DIK_S;
		case 'D':
			return DIK_D;
		case 'F':
			return DIK_F;
		case 'G':
			return DIK_G;
		case 'H':
			return DIK_H;
		case 'J':
			return DIK_J;
		case 'K':
			return DIK_K;
		case 'L':
			return DIK_L;
		case ';':
			return DIK_SEMICOLON;
		case VK_OEM_7:
			return DIK_APOSTROPHE;
		case '`':
			return DIK_GRAVE;
		case VK_SHIFT:
			return DIK_LSHIFT;
		case '\\':
			return DIK_BACKSLASH;
		case 'Z':
			return DIK_Z;
		case 'X':
			return DIK_X;
		case 'C':
			return DIK_C;
		case 'V':
			return DIK_V;
		case 'B':
			return DIK_B;
		case 'N':
			return DIK_N;
		case 'M':
			return DIK_M;
		case ',':
			return DIK_COMMA;
		case VK_OEM_PERIOD:
			return DIK_PERIOD;
		case '/':
			return DIK_SLASH;
		case VK_RSHIFT:
			return DIK_RSHIFT;
		case VK_MULTIPLY:
			return DIK_MULTIPLY;
		case VK_LMENU:
			return DIK_LMENU;
		case VK_SPACE:
			return DIK_SPACE;
		case VK_CAPITAL:
			return DIK_CAPITAL;
		case VK_F1:
			return DIK_F1;
		case VK_F2:
			return DIK_F2;
		case VK_F3:
			return DIK_F3;
		case VK_F4:
			return DIK_F4;
		case VK_F5:
			return DIK_F5;
		case VK_F6:
			return DIK_F6;
		case VK_F7:
			return DIK_F7;
		case VK_F8:
			return DIK_F8;
		case VK_F9:
			return DIK_F9;
		case VK_F10:
			return DIK_F10;
		case VK_NUMLOCK:
			return DIK_NUMLOCK;
		case VK_SCROLL:
			return DIK_SCROLL;
		case VK_F11:
			return DIK_F11;
		case VK_F12:
			return DIK_F12;
		case VK_F13:
			return DIK_F13;
		case VK_F14:
			return DIK_F14;
		case VK_F15:
			return DIK_F15;
		case VK_RCONTROL:
			return DIK_RCONTROL;
		case VK_RMENU:
			return DIK_RMENU;
		case VK_HOME:
			return DIK_HOME;
		case VK_UP:
			return DIK_UP;
		case VK_PRIOR:
			return DIK_PRIOR;
		case VK_LEFT:
			return DIK_LEFT;
		case VK_RIGHT:
			return DIK_RIGHT;
		case VK_END:
			return DIK_END;
		case VK_DOWN:
			return DIK_DOWN;
		case VK_NEXT:
			return DIK_NEXT;
		case VK_INSERT:
			return DIK_INSERT;
		case VK_DELETE:
			return DIK_DELETE;
		case VK_LBUTTON:
			return DIK_MOUSELBUTTON;
		case VK_RBUTTON:
			return DIK_MOUSERBUTTON;
		case DIK_MOUSEWHEEL:
			return DIK_MOUSEWHEEL;
		default:
			return -1;
		}
		return 0xff;
	}
	return cKey;
}
