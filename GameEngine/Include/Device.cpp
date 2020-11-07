#include "Device.h"

DEFINITION_SINGLE(CDevice)

CDevice::CDevice()
	: m_pDevice(nullptr), m_pContext(nullptr), m_pSwapChain(nullptr),
	m_pTargetView(nullptr), m_pDepthView(nullptr),m_p2DTarget(nullptr),
	m_p2DFactory(nullptr)
{
	memset(m_fClearColor, 0, sizeof(float) * 4);
	m_fClearColor[2] = 1.f;
}

CDevice::~CDevice()
{
	SAFE_RELEASE(m_p2DFactory);
	SAFE_RELEASE(m_p2DTarget);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pTargetView);
	SAFE_RELEASE(m_pDepthView);

	//Context는 Release하기전 ClearState함수를 호출하여
	//파이프라인의 모든 설정을 초기상태로 클리어한다.
	if (m_pContext)
		m_pContext->ClearState();

	SAFE_RELEASE(m_pContext);
	SAFE_RELEASE(m_pDevice);
}

Vector2 CDevice::GetWindowRatio() const
{
	// 윈도우 창의 크기 / 화면 해상도를 반환
	RECT    rc = {};

	GetClientRect(m_hWnd, &rc);

	int iWidth = rc.right - rc.left;
	int iHeight = rc.bottom - rc.top;

	return Vector2(m_tRS.iWidth / (float)iWidth, m_tRS.iHeight / (float)iHeight);
}

ID2D1RenderTarget* CDevice::Get2DRenderTarget() const
{
	return m_p2DTarget;
}

ID2D1Factory* CDevice::Get2DFactory() const
{
	return m_p2DFactory;
}

bool CDevice::Init(HWND hWnd, int iWidth, int iHeight, bool bWindowMode)
{
	m_hWnd = hWnd;
	m_tRS.iWidth = iWidth;
	m_tRS.iHeight = iHeight;

	unsigned int iFlag = 0;

#ifdef _DEBUG
	//디버그 모드 빌드에서 디버그 계층을 활성화 하려면 D3D11_CREATE_DEVICE_DEBUG플래그가 반드시 필요하다.
	//이 플래그를 지정하면 Direct3D는 VC++ 출력 창에 디버그 메시지를 보낸다.
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	//BGRA형식을 지원하는 Device를 생성하기 위한 플래그
	iFlag |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	//기능수준
	//GPU가 지원하는 기능들의 엄격한 집합
	//기능 수준 11을 지원하는 GPU는 반드시 Direct3D 11의 기능 집합 전체를 지원해야 한다.
	//또한 사용자의 하드웨어가 특정 기능 수준을 지원하지 않는 경우 더 낮은 수준으로 후퇴하는
	//전략을 사용할 수도 있다.
	D3D_FEATURE_LEVEL eFLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL eFLevel1 = D3D_FEATURE_LEVEL_11_0;

	//SwapChain의 특징을 담고있는 구조체
	//SwapChain이란 전면 버퍼와 후면 버퍼가 존재하면 후면 버퍼에 그림을 그리고
	//그림을 그리는 것이 완료되면 후면 버퍼를 전면버퍼로 전면 버퍼를 후면 버퍼로 교환하여
	//화면에 출력하는 것을 말한다.
	DXGI_SWAP_CHAIN_DESC tSwapchainDesc = {};

	//BufferDesc : 생성하고자 하는 back buffer의 속성 
	//----------------------------------------------------
	//버퍼 크기
	//--------------------------------------
	tSwapchainDesc.BufferDesc.Width = iWidth;
	tSwapchainDesc.BufferDesc.Height = iHeight;
	//--------------------------------------

	//픽셀형식 : RGBA를 각각 8bit로 0~1값을 가짐
	tSwapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//화면 주사율 : 60Hz = 1초당 60번 그려내는 작업
	//--------------------------------------
	tSwapchainDesc.BufferDesc.RefreshRate.Numerator = 1;
	tSwapchainDesc.BufferDesc.RefreshRate.Denominator = 60;
	//--------------------------------------

	//주어진 모니터의 해상도에 맞게 이미지가 늘어나는 방식
	//----------------------------------------------------
	tSwapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;//배율 Setting X
	//----------------------------------------------------

	//래스터가 이미지를 만드는데 사용하는 방법을 지정하는 플래그
	//----------------------------------------------------
	tSwapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;//스캔 라인 순서 지정 X
	//----------------------------------------------------
	//----------------------------------------------------

	//BackBuffer 수 지정
	tSwapchainDesc.BufferCount = 1;

	//버퍼 용도 지정(렌더 타겟 출력용)
	tSwapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	tSwapchainDesc.OutputWindow = hWnd;

	//멀티 샘플링
	//-----------------------------------
	tSwapchainDesc.SampleDesc.Count = 1;
	tSwapchainDesc.SampleDesc.Quality = 0;
	//-----------------------------------

	//창모드
	tSwapchainDesc.Windowed = bWindowMode;

	//Present함수를 호출하고 디스플레이의 픽셀을 처리하기 위한 옵션
	tSwapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	//D3D11CreateDeviceAndSwapChain 함수 : Device와 SwapChain을 동시에 생성하는 함수
	//IDXGIAdapter * : 장치를 만들 때 사용할 비디오 어댑터에 대한 포인터
	//D3D_DRIVER_TYPE : D3D_DRIVER_TYPE을 만들 수 있는 드라이버 유형
	//HMODULE : 소프트웨어 래스터라이저가 구현되어 있는 DLL이 핸들을 지정
	//UINT : 사용할 Dx11의 API 레이어
	//FeautureLevel
	//UINT : 사용하는 FeatureLevel의 개수
	//SDKVersion : 사용하고 있는 DX SDK버전
	//SwapChainDesc : SwapChain의 설정값들을 저장한 구조체의 포인터
	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		0, iFlag, &eFLevel, 1, D3D11_SDK_VERSION, &tSwapchainDesc,
		&m_pSwapChain, &m_pDevice, &eFLevel1, &m_pContext)))
		return false;

	//나중에 사용할 스레드용 구조체
	D3D11_FEATURE_DATA_THREADING tThread = {};
	HRESULT result = m_pDevice->CheckFeatureSupport(D3D11_FEATURE_THREADING,
		&tThread, sizeof(tThread));

	//백버퍼에 픽셀을 저장하기 위한 Texture얻어오는 작업
	ID3D11Texture2D* pBackBuffer = nullptr;
	//GetBuffer함수 : Swap Chain의 백 버퍼 중 하나에 액세스
	//0부터 시작하는 버퍼 인덱스입니다.
	//스왑 체인의 스왑 효과가 DXGI_SWAP_EFFECT_DISCARD 이면이 메서드는 첫 번째 버퍼에만 액세스 가능 이 상황에서는 인덱스를 0으로 설정
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	//__uuidof연산자
	//먼저 GUID(전역 고유 식별자) 응용 소프트웨어에서 사용되는 유사 난수
	//COM(Component Object Model)에서는 GUID를 구성 요소의 인터페이스를 구별하기 위해 사용하고
	//즉 호환되지 않을 수 있는 두 개의 컴포넌트가 동일한 인터페이스 이름을 사용하더라도
	//각각의 인터페이스는 GUID를 갖기 때문에 구별가능하다.
	//__uuidof연산자는 해당 인터페이스의 GUID를 가져오는 연산자다.


	 // 렌더타겟 뷰를 생성해서 이 타겟이 백버퍼의 Texture를 가지고 있게 하고
	// 거기에 출력할 수 있게 한다.
	m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pTargetView);

	SAFE_RELEASE(pBackBuffer);

	//깊이버퍼 생성
	D3D11_TEXTURE2D_DESC tDepthDesc = {};

	tDepthDesc.Width = iWidth;
	tDepthDesc.Height = iHeight;
	tDepthDesc.ArraySize = 1;

	//*스텐실 버퍼란?*//
	tDepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//깊이는 24bit 스텐실은 8비트 z버퍼 형식
	tDepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;//파이프라인에 바인딩하기위한 플래그
	tDepthDesc.Usage = D3D11_USAGE_DEFAULT;
	tDepthDesc.SampleDesc.Count = 1;
	tDepthDesc.SampleDesc.Quality = 0;
	tDepthDesc.MipLevels = 1;

	ID3D11Texture2D* pDepthTex = nullptr;

	m_pDevice->CreateTexture2D(&tDepthDesc, nullptr, &pDepthTex);

	m_pDevice->CreateDepthStencilView(pDepthTex, nullptr, &m_pDepthView);

	SAFE_RELEASE(pDepthTex);

	// Device에 생성한 렌더타겟뷰와 깊이스텐실뷰를 묶어준다.
	m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDepthView);

	// 뷰포트를 만들어준다.
	// 뷰포트는 화면에 출력되는 2D 영역을 의미한다.
	// 3D도 결국 화면에 출력될때는 2D 이다.
	// Viewport를 설정해서 DirectX11을 이용해서 출력되는 영역을 지정한다.
	//------------------------------------------------------
	D3D11_VIEWPORT  tVP = {};

	tVP.Width = (float)iWidth;
	tVP.Height = (float)iHeight;
	tVP.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &tVP);
	//------------------------------------------------------

	//2D Factory 생성
	//------------------------------------------------------
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_p2DFactory)))
		return false;
	//------------------------------------------------------

	//2D,3D가 같은 Surface를 사용할 수 있도록 BackBuffer의
	//Surface를 받아온다.
	//------------------------------------------------------
	IDXGISurface* pBackSurface = nullptr;

	m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackSurface));
	//------------------------------------------------------

	//2D용 RenderTarget Setting
	//------------------------------------------------------
	D2D1_RENDER_TARGET_PROPERTIES	props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	if (FAILED(m_p2DFactory->CreateDxgiSurfaceRenderTarget(pBackSurface, props, &m_p2DTarget)))
		return false;

	SAFE_RELEASE(pBackSurface);
	//------------------------------------------------------

	return true;
}

void CDevice::ClearTarget()
{
	m_pContext->ClearRenderTargetView(m_pTargetView, m_fClearColor);
	m_pContext->ClearDepthStencilView(m_pDepthView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CDevice::Render()
{
	m_pSwapChain->Present(0, 0);
}
