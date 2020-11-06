#pragma once

#include "Viewer.h"

class CRectViewer : public CViewer
{
public:
	CRectViewer();
	virtual ~CRectViewer();

public:
	bool Init(class CEditDevice* pDevice);

public:
	bool SetRect(POINT tLT, POINT tRB);

private:
	virtual void SetIndexBuffer();

public:
	virtual void SetShaderName();

private:
	virtual void SetCBuffer();

private:
	POINT	m_tStart;
	POINT	m_tEnd;

};

