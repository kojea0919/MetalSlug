#pragma once

#include "GameEngine.h"
#include "Resource/GraphicShader.h"

class CEditDevice
{
public:
	CEditDevice();
	~CEditDevice();

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pTargetView;
	ID3D11DepthStencilView* m_pDepthView;
	Resolution				m_tRS;
	HWND					m_hWnd;
	float					m_fClearColor[4];

public:
	ID3D11Device* GetDevice() const
	{
		return m_pDevice;
	}
	ID3D11DeviceContext* GetContext()	const
	{
		return m_pContext;
	}
	IDXGISwapChain* GetSwapChain()	const
	{
		return m_pSwapChain;
	}
	Resolution GetResolution()	const
	{
		return m_tRS;
	}

public:
	bool Init(HWND hWnd, int iWidth, int iHeight, bool bWindowMode = true);
	void ClearTarget();
	void Render();
};

