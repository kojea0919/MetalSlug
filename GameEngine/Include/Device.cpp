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

	//Context�� Release�ϱ��� ClearState�Լ��� ȣ���Ͽ�
	//������������ ��� ������ �ʱ���·� Ŭ�����Ѵ�.
	if (m_pContext)
		m_pContext->ClearState();

	SAFE_RELEASE(m_pContext);
	SAFE_RELEASE(m_pDevice);
}

Vector2 CDevice::GetWindowRatio() const
{
	// ������ â�� ũ�� / ȭ�� �ػ󵵸� ��ȯ
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
	//����� ��� ���忡�� ����� ������ Ȱ��ȭ �Ϸ��� D3D11_CREATE_DEVICE_DEBUG�÷��װ� �ݵ�� �ʿ��ϴ�.
	//�� �÷��׸� �����ϸ� Direct3D�� VC++ ��� â�� ����� �޽����� ������.
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	//BGRA������ �����ϴ� Device�� �����ϱ� ���� �÷���
	iFlag |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	//��ɼ���
	//GPU�� �����ϴ� ��ɵ��� ������ ����
	//��� ���� 11�� �����ϴ� GPU�� �ݵ�� Direct3D 11�� ��� ���� ��ü�� �����ؾ� �Ѵ�.
	//���� ������� �ϵ��� Ư�� ��� ������ �������� �ʴ� ��� �� ���� �������� �����ϴ�
	//������ ����� ���� �ִ�.
	D3D_FEATURE_LEVEL eFLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL eFLevel1 = D3D_FEATURE_LEVEL_11_0;

	//SwapChain�� Ư¡�� ����ִ� ����ü
	//SwapChain�̶� ���� ���ۿ� �ĸ� ���۰� �����ϸ� �ĸ� ���ۿ� �׸��� �׸���
	//�׸��� �׸��� ���� �Ϸ�Ǹ� �ĸ� ���۸� ������۷� ���� ���۸� �ĸ� ���۷� ��ȯ�Ͽ�
	//ȭ�鿡 ����ϴ� ���� ���Ѵ�.
	DXGI_SWAP_CHAIN_DESC tSwapchainDesc = {};

	//BufferDesc : �����ϰ��� �ϴ� back buffer�� �Ӽ� 
	//----------------------------------------------------
	//���� ũ��
	//--------------------------------------
	tSwapchainDesc.BufferDesc.Width = iWidth;
	tSwapchainDesc.BufferDesc.Height = iHeight;
	//--------------------------------------

	//�ȼ����� : RGBA�� ���� 8bit�� 0~1���� ����
	tSwapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//ȭ�� �ֻ��� : 60Hz = 1�ʴ� 60�� �׷����� �۾�
	//--------------------------------------
	tSwapchainDesc.BufferDesc.RefreshRate.Numerator = 1;
	tSwapchainDesc.BufferDesc.RefreshRate.Denominator = 60;
	//--------------------------------------

	//�־��� ������� �ػ󵵿� �°� �̹����� �þ�� ���
	//----------------------------------------------------
	tSwapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;//���� Setting X
	//----------------------------------------------------

	//�����Ͱ� �̹����� ����µ� ����ϴ� ����� �����ϴ� �÷���
	//----------------------------------------------------
	tSwapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;//��ĵ ���� ���� ���� X
	//----------------------------------------------------
	//----------------------------------------------------

	//BackBuffer �� ����
	tSwapchainDesc.BufferCount = 1;

	//���� �뵵 ����(���� Ÿ�� ��¿�)
	tSwapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	tSwapchainDesc.OutputWindow = hWnd;

	//��Ƽ ���ø�
	//-----------------------------------
	tSwapchainDesc.SampleDesc.Count = 1;
	tSwapchainDesc.SampleDesc.Quality = 0;
	//-----------------------------------

	//â���
	tSwapchainDesc.Windowed = bWindowMode;

	//Present�Լ��� ȣ���ϰ� ���÷����� �ȼ��� ó���ϱ� ���� �ɼ�
	tSwapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	//D3D11CreateDeviceAndSwapChain �Լ� : Device�� SwapChain�� ���ÿ� �����ϴ� �Լ�
	//IDXGIAdapter * : ��ġ�� ���� �� ����� ���� ����Ϳ� ���� ������
	//D3D_DRIVER_TYPE : D3D_DRIVER_TYPE�� ���� �� �ִ� ����̹� ����
	//HMODULE : ����Ʈ���� �����Ͷ������� �����Ǿ� �ִ� DLL�� �ڵ��� ����
	//UINT : ����� Dx11�� API ���̾�
	//FeautureLevel
	//UINT : ����ϴ� FeatureLevel�� ����
	//SDKVersion : ����ϰ� �ִ� DX SDK����
	//SwapChainDesc : SwapChain�� ���������� ������ ����ü�� ������
	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		0, iFlag, &eFLevel, 1, D3D11_SDK_VERSION, &tSwapchainDesc,
		&m_pSwapChain, &m_pDevice, &eFLevel1, &m_pContext)))
		return false;

	//���߿� ����� ������� ����ü
	D3D11_FEATURE_DATA_THREADING tThread = {};
	HRESULT result = m_pDevice->CheckFeatureSupport(D3D11_FEATURE_THREADING,
		&tThread, sizeof(tThread));

	//����ۿ� �ȼ��� �����ϱ� ���� Texture������ �۾�
	ID3D11Texture2D* pBackBuffer = nullptr;
	//GetBuffer�Լ� : Swap Chain�� �� ���� �� �ϳ��� �׼���
	//0���� �����ϴ� ���� �ε����Դϴ�.
	//���� ü���� ���� ȿ���� DXGI_SWAP_EFFECT_DISCARD �̸��� �޼���� ù ��° ���ۿ��� �׼��� ���� �� ��Ȳ������ �ε����� 0���� ����
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	//__uuidof������
	//���� GUID(���� ���� �ĺ���) ���� ����Ʈ����� ���Ǵ� ���� ����
	//COM(Component Object Model)������ GUID�� ���� ����� �������̽��� �����ϱ� ���� ����ϰ�
	//�� ȣȯ���� ���� �� �ִ� �� ���� ������Ʈ�� ������ �������̽� �̸��� ����ϴ���
	//������ �������̽��� GUID�� ���� ������ ���������ϴ�.
	//__uuidof�����ڴ� �ش� �������̽��� GUID�� �������� �����ڴ�.


	 // ����Ÿ�� �並 �����ؼ� �� Ÿ���� ������� Texture�� ������ �ְ� �ϰ�
	// �ű⿡ ����� �� �ְ� �Ѵ�.
	m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pTargetView);

	SAFE_RELEASE(pBackBuffer);

	//���̹��� ����
	D3D11_TEXTURE2D_DESC tDepthDesc = {};

	tDepthDesc.Width = iWidth;
	tDepthDesc.Height = iHeight;
	tDepthDesc.ArraySize = 1;

	//*���ٽ� ���۶�?*//
	tDepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//���̴� 24bit ���ٽ��� 8��Ʈ z���� ����
	tDepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;//���������ο� ���ε��ϱ����� �÷���
	tDepthDesc.Usage = D3D11_USAGE_DEFAULT;
	tDepthDesc.SampleDesc.Count = 1;
	tDepthDesc.SampleDesc.Quality = 0;
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
	//------------------------------------------------------
	D3D11_VIEWPORT  tVP = {};

	tVP.Width = (float)iWidth;
	tVP.Height = (float)iHeight;
	tVP.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &tVP);
	//------------------------------------------------------

	//2D Factory ����
	//------------------------------------------------------
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_p2DFactory)))
		return false;
	//------------------------------------------------------

	//2D,3D�� ���� Surface�� ����� �� �ֵ��� BackBuffer��
	//Surface�� �޾ƿ´�.
	//------------------------------------------------------
	IDXGISurface* pBackSurface = nullptr;

	m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackSurface));
	//------------------------------------------------------

	//2D�� RenderTarget Setting
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
