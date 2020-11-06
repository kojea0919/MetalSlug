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
    // �Ʒ� �÷��״� ���߿� Font ó���� �Ҷ� �ʿ��� �÷����̴�.
    // ����̽����� BGRA �ȼ������� ������ �� �ֵ��� �Ѵ�.
    iFlag |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    D3D_FEATURE_LEVEL   eFLevel = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL   eFLevel1 = D3D_FEATURE_LEVEL_11_0;

    // ����۸� ����� ���� SwapChain ������ �����Ѵ�.
    DXGI_SWAP_CHAIN_DESC    tSwapchainDesc = {};

    tSwapchainDesc.BufferDesc.Width = iWidth;
    tSwapchainDesc.BufferDesc.Height = iHeight;
    tSwapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    tSwapchainDesc.BufferDesc.RefreshRate.Numerator = 1;    // ����
    tSwapchainDesc.BufferDesc.RefreshRate.Denominator = 60; // �и�
    // �Ʒ��� Ȯ�� ��ҽ��� ������ �������� �ʴ´�.
    tSwapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    tSwapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    // ����� ���� �����Ѵ�.
    tSwapchainDesc.BufferCount = 1;
    // ���� �뵵�� �����Ѵ�.
    // �� ����۴� ����Ÿ�� ��¿����� ����Ѵ�.
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

    // ������� �ȼ��� �����ϱ� ���� Texture�� ���´�.
    ID3D11Texture2D* pBackBuffer = nullptr;

    // ���⼭ ����۸� �������� RefCount�� �����ǰ� �ȴ�.
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

    // ����Ÿ�� �並 �����ؼ� �� Ÿ���� ������� Texture�� ������ �ְ� �ϰ�
    // �ű⿡ ����� �� �ְ� �Ѵ�.
    m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pTargetView);

    SAFE_RELEASE(pBackBuffer);

    // ���̹��۸� ������ش�.
    D3D11_TEXTURE2D_DESC    tDepthDesc = {};

    tDepthDesc.Width = iWidth;
    tDepthDesc.Height = iHeight;
    tDepthDesc.ArraySize = 1;
    /*
    ���ٽǹ��۴� Ư��ȿ���� ó���ϱ� ���� �����̴�.
    Ư�� �ȼ� �κи� �׸��� �Ѵٴ��� �ϴ� ��ɵ��� ���� �� �ִ�.
    �̰� ��~�߿� �Ѵ�
    */
    tDepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    tDepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    tDepthDesc.Usage = D3D11_USAGE_DEFAULT;
    tDepthDesc.SampleDesc.Count = 1;
    tDepthDesc.SampleDesc.Quality = 0;
    // �Ʒ��� ���߿� Texture�� ����� �ٷ궧 �˾ƺ���.
    tDepthDesc.MipLevels = 1;

    ID3D11Texture2D* pDepthTex = nullptr;

    m_pDevice->CreateTexture2D(&tDepthDesc, nullptr, &pDepthTex);

    m_pDevice->CreateDepthStencilView(pDepthTex, nullptr, &m_pDepthView);

    SAFE_RELEASE(pDepthTex);

    // Device�� ������ ����Ÿ�ٺ�� ���̽��ٽǺ並 �����ش�.
    m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDepthView);

    // ����Ʈ�� ������ش�.
    // ����Ʈ�� ȭ�鿡 ��µǴ� 2D ������ �ǹ��Ѵ�.
    // 3D�� �ᱹ ȭ�鿡 ��µɶ��� 2D �̴�.
    // Viewport�� �����ؼ� DirectX11�� �̿��ؼ� ��µǴ� ������ �����Ѵ�.
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