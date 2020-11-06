#pragma once

#include "GameEngine.h"

class CDevice
{
private:
	//그래픽 장치 하드웨어에 대한 소프트웨어 제어기
	//--------------------------------------------

	//기능 지원 점검과 자원 할당에 사용
	ID3D11Device* m_pDevice;

	//렌더 대상을 설정하고 자원들을 그래픽스 파이프 라인에
	//묶으며 GPU가 수행할 렌더링 명령들을 지시하는데 사용
	ID3D11DeviceContext* m_pContext;

	//--------------------------------------------

	//Page Flipping을 사용하기 위해 필요한 인터페이스
	IDXGISwapChain* m_pSwapChain;

	//후면 버퍼를 파이프라인에 묶기위해 필요한 뷰
	ID3D11RenderTargetView* m_pTargetView;

	//깊이 스텐실 버퍼를 파이프라인에 묶기 위해 필요한 뷰
	ID3D11DepthStencilView* m_pDepthView;

	Resolution m_tRS;
	HWND m_hWnd;

	//기본 바탕 색
	float m_fClearColor[4];


public:
	//Get,Set함수
	//--------------------------
	ID3D11Device* GetDevice() const
	{
		return m_pDevice;
	}
	ID3D11DeviceContext* GetContext() const
	{
		return m_pContext;
	}
	IDXGISwapChain* GetSwapChain()const
	{
		return m_pSwapChain;
	}
	Resolution GetResolution() const
	{
		return m_tRS;
	}
	Vector2 GetWindowRatio() const;
	//--------------------------

public:
	bool Init(HWND hWnd, int iWidth, int iHeight, bool bWindowMode = true);
	void ClearTarget();
	void Render();

	DECLARE_SINGLE(CDevice)
};