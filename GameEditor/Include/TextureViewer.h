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
	//ShaderResourceView 생성 함수
	bool CreateResourceView();

private:
	virtual void SetShaderName();

private:
	//상수버퍼 Setting
	virtual void SetCBuffer();

private:
	void InitSampler();

private:
	//현재 불러온 이미지에 대한 정보
	//-----------------------------

	//이미지
	ScratchImage* m_pImage;

	//ShaderResourceView
	ID3D11ShaderResourceView* m_pSRV;

	//이미지 크기 정보
	//-----------------
	unsigned int	m_iWidth;
	unsigned int	m_iHeight;
	//-----------------

	//-----------------------------

	//Sampler
	ID3D11SamplerState* m_pPointSmp;
};

