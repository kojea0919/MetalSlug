#pragma once

#include "GameEngine.h"

class CDevice
{
private:
	//�׷��� ��ġ �ϵ��� ���� ����Ʈ���� �����
	//--------------------------------------------

	//��� ���� ���˰� �ڿ� �Ҵ翡 ���
	ID3D11Device* m_pDevice;

	//���� ����� �����ϰ� �ڿ����� �׷��Ƚ� ������ ���ο�
	//������ GPU�� ������ ������ ��ɵ��� �����ϴµ� ���
	ID3D11DeviceContext* m_pContext;

	//--------------------------------------------

	//Page Flipping�� ����ϱ� ���� �ʿ��� �������̽�
	IDXGISwapChain* m_pSwapChain;

	//�ĸ� ���۸� ���������ο� �������� �ʿ��� ��
	ID3D11RenderTargetView* m_pTargetView;

	//���� ���ٽ� ���۸� ���������ο� ���� ���� �ʿ��� ��
	ID3D11DepthStencilView* m_pDepthView;

	Resolution m_tRS;
	HWND m_hWnd;

	//�⺻ ���� ��
	float m_fClearColor[4];


public:
	//Get,Set�Լ�
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