
struct PS_OUTPUT_SINGLE
{
	// SV_TARGET은 렌더타겟을 의미한다.
	// DX11에서 렌더타겟은 8개까지 한꺼번에 사용이 가능한데
	// 현재 기본적인 상태라면 백버퍼의 렌더타겟이 0번에 지정되어 있다.
	// 아래는 0번타겟에 출력하겠다는 것이므로 기본 상황이라면
	// 백버퍼에 색상을 쓰겠다라는 것이다.
	float4	vColor	: SV_TARGET;
};

cbuffer TransformCBuffer	: register(b0)
{
	matrix	g_matWorld;
	matrix	g_matView;
	matrix	g_matProj;
	matrix	g_matWV;
	matrix	g_matWVP;
	float3	g_vPivot;
	float	g_fTrEmpty1;
	float3	g_vMeshSize;
	float	g_fTrEmpty2;
};

cbuffer	MaterialCBuffer	: register(b1)
{
	float4	g_vMtrlDif;
	float4	g_vMtrlAmb;
	float4	g_vMtrlSpc;
	int		g_iMtrlDifTex;
	int		g_iMtrlNrmTex;
	int		g_iMtrlSpcTex;
	int		g_iMtrlEmpty;
};

cbuffer SpriteCBuffer	: register(b2)
{
	int		g_iSpriteImageType;
	int		g_iSpriteAnimType;
	float2	g_vSpriteFrameStart;
	float2	g_vSpriteFrameEnd;
	float2	g_vSpriteImageSize;
}

cbuffer TimeCBuffer : register(b3)
{
	float4	g_vDeltaTime;
}

//선형 보간
SamplerState	g_LinearSmp : register(s0);

//점 보간
SamplerState	g_PointSmp	: register(s1);

//비등방 필터링
SamplerState	g_Anisotropic : register(s2);

Texture2D		g_DiffuseTex : register(t0);

#define IMAGE_ATLAS 0
#define IMAGE_ARRAY 1

float2 ComputeSpriteUV(float2 vUV)
{
	//이미지 타입이 Atlas인 경우에만 UV Setting
	//------------------------------------------
	if (g_iSpriteImageType == IMAGE_ARRAY)
		return vUV;
	
	float2 vResult = (float2)0.f;

	//사각형 오른쪽 점인 경우 
	if (vUV.x > 0.f)
		vResult.x = g_vSpriteFrameEnd.x / g_vSpriteImageSize.x;

	//사각형 왼쪽 점인 경우
	else
		vResult.x = g_vSpriteFrameStart.x / g_vSpriteImageSize.x;

	//사각형 위쪽 점인 경우
	if (vUV.y > 0.f)
		vResult.y = g_vSpriteFrameEnd.y / g_vSpriteImageSize.y;

	//사각형 아래쪽 점인 경우
	else
		vResult.y = g_vSpriteFrameStart.y / g_vSpriteImageSize.y;
	//------------------------------------------

	return vResult;
}

float2 ComputeSpriteUV(float2 vUV, float2 vFrameStart, float2 vFrameEnd, float2 vImageSize)
{
	float2 vResult = (float2) 0.f;

	if (vUV.x > 0.f)
		vResult.x = vFrameEnd.x / vImageSize.x;
	else
		vResult.x = vFrameStart.x / vImageSize.x;

	if (vUV.y > 0.f)
		vResult.y = vFrameEnd.y / vImageSize.y;
	else
		vResult.y = vFrameStart.y / vImageSize.y;

	return vResult;
}

float RandomR(in float2 uv,in float fDeltaTime)
{
	float fResult = (frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 43758.5453 * fDeltaTime )) + 0.3;
	return fResult;
}

float RandomG(in float2 uv, in float fDeltaTime)
{
	float fResult = (frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 13758.5453 * fDeltaTime)) + 0.3;
	return fResult;
}

float RandomB(in float2 uv, in float fDeltaTime)
{
	float fResult = (frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 23753.21 * fDeltaTime)) + 0.36;
	return fResult;
}