#include "pch.h"
#include "EditDevice.h"

CEditDevice::CEditDevice()
    : m_pDevice(nullptr), m_pContext(nullptr),
    m_pSwapChain(nullptr), m_pTargetView(nullptr),
    m_pDepthView(nullptr)
{
	memset(m_fClearColor, 0, sizeof(float) * 4);
    m_fClearColor[0] = 1.f;
    m_fClearColor[1] = 1.f;
    m_fClearColor[2] = 1.f;
}

CEditDevice::~CEditDevice()
{
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pTargetView);
	SAFE_RELEASE(m_pDepthView);

	if (m_pContext)
		m_pContext->ClearState();

	SAFE_RELEASE(m_pContext);
	SAFE_RELEASE(m_pDevice);
}

bool CEditDevice::Init(HWND hWnd, int iWidth, int iHeight, bool bWindowMode)
{
    m_hWnd = hWnd;
    m_tRS.iWidth = iWidth;
    m_tRS.iHeight = iHeight;

    unsigned int    iFlag = 0;

#ifdef _DEBUG
    iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG
    // 아래 플래그는 나중에 Font 처리를 할때 필요한 플래그이다.
    // 디바이스에서 BGRA 픽셀포맷을 지원할 수 있도록 한다.
    iFlag |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    D3D_FEATURE_LEVEL   eFLevel = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL   eFLevel1 = D3D_FEATURE_LEVEL_11_0;

    // 백버퍼를 만들기 위한 SwapChain 정보를 설정한다.
    DXGI_SWAP_CHAIN_DESC    tSwapchainDesc = {};

    tSwapchainDesc.BufferDesc.Width = iWidth;
    tSwapchainDesc.BufferDesc.Height = iHeight;
    tSwapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    tSwapchainDesc.BufferDesc.RefreshRate.Numerator = 1;    // 분자
    tSwapchainDesc.BufferDesc.RefreshRate.Denominator = 60; // 분모
    // 아래는 확대 축소시의 배율을 지정하지 않는다.
    tSwapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    tSwapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    // 백버퍼 수를 지정한다.
    tSwapchainDesc.BufferCount = 1;
    // 버퍼 용도를 지정한다.
    // 이 백버퍼는 렌더타겟 출력용으로 사용한다.
    tSwapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    tSwapchainDesc.OutputWindow = hWnd;
    tSwapchainDesc.SampleDesc.Count = 1;
    tSwapchainDesc.SampleDesc.Quality = 0;
    tSwapchainDesc.Windowed = bWindowMode;
    tSwapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
        0, iFlag, &eFLevel, 1, D3D11_SDK_VERSION, &tSwapchainDesc,
        &m_pSwapChain, &m_pDevice, &eFLevel1, &m_pContext)))
        return false;

    D3D11_FEATURE_DATA_THREADING    tThread = {};

    HRESULT result = m_pDevice->CheckFeatureSupport(D3D11_FEATURE_THREADING,
        &tThread, sizeof(tThread));

    // 백버퍼의 픽셀을 저장하기 위한 Texture를 얻어온다.
    ID3D11Texture2D* pBackBuffer = nullptr;

    // 여기서 백버퍼를 가져오면 RefCount가 증가되게 된다.
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

    // 렌더타겟 뷰를 생성해서 이 타겟이 백버퍼의 Texture를 가지고 있게 하고
    // 거기에 출력할 수 있게 한다.
    m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pTargetView);

    SAFE_RELEASE(pBackBuffer);

    // 깊이버퍼를 만들어준다.
    D3D11_TEXTURE2D_DESC    tDepthDesc = {};

    tDepthDesc.Width = iWidth;
    tDepthDesc.Height = iHeight;
    tDepthDesc.ArraySize = 1;
    /*
    스텐실버퍼는 특수효과를 처리하기 위한 버퍼이다.
    특정 픽셀 부분만 그리게 한다던지 하는 기능들을 만들어낼 수 있다.
    이건 나~중에 한다
    */
    tDepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    tDepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    tDepthDesc.Usage = D3D11_USAGE_DEFAULT;
    tDepthDesc.SampleDesc.Count = 1;
    tDepthDesc.SampleDesc.Quality = 0;
    // 아래는 나중에 Texture를 제대로 다룰때 알아본다.
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
    D3D11_VIEWPORT  tVP = {};

    tVP.Width = (float)iWidth;
    tVP.Height = (float)iHeight;
    tVP.MaxDepth = 1.f;

    m_pContext->RSSetViewports(1, &tVP);

    return true;
}

void CEditDevice::ClearTarget()
{
    m_pContext->ClearRenderTargetView(m_pTargetView, m_fClearColor);
    m_pContext->ClearDepthStencilView(m_pDepthView,
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CEditDevice::Render()
{
    m_pSwapChain->Present(0, 0);
}