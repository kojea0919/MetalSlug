#pragma once

#include "Shader.h"

class CComputeShader :
	public CShader
{
	friend class CShaderManager;
	friend class CSceneResource;

protected:
	CComputeShader();
	virtual ~CComputeShader();

public:
	virtual void SetShader();
};

