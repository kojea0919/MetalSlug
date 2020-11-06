#pragma once

#include "Resource/Mesh.h"
#include "Viewer.h"
#include "GameEngine.h"

class CTextureViewer : public CViewer
{
public:
	CTextureViewer();
	~CTextureViewer();

public:
	bool Init(class CEditDevice* pDevice);

public:
	bool SetImage(ScratchImage * pImage);

public:
	int GetWidth() const
	{
		return m_iWidth;
	}
	int GetHeight() const
	{
		return m_iHeight;
	}
	void SetViewSize(int iWidth, int iHeight)
	{
		m_iViewWidth = iWidth;
		m_iViewHeight = iHeight;
	}

private:
	virtual void SetIndexBuffer();

private:
	//ShaderResourceView ���� �Լ�
	bool CreateResourceView();

private:
	virtual void SetShaderName();

private:
	//������� Setting
	virtual void SetCBuffer();

private:
	void InitSampler();

private:
	//���� �ҷ��� �̹����� ���� ����
	//-----------------------------

	//�̹���
	ScratchImage* m_pImage;

	//ShaderResourceView
	ID3D11ShaderResourceView* m_pSRV;

	//�̹��� ũ�� ����
	//-----------------
	unsigned int	m_iWidth;
	unsigned int	m_iHeight;
	//-----------------

	//-----------------------------

	//Sampler
	ID3D11SamplerState* m_pPointSmp;
};

