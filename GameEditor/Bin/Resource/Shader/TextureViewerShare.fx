
struct PS_OUTPUT_SINGLE
{
	// SV_TARGET은 렌더타겟을 의미한다.
	// DX11에서 렌더타겟은 8개까지 한꺼번에 사용이 가능한데
	// 현재 기본적인 상태라면 백버퍼의 렌더타겟이 0번에 지정되어 있다.
	// 아래는 0번타겟에 출력하겠다는 것이므로 기본 상황이라면
	// 백버퍼에 색상을 쓰겠다라는 것이다.
	float4	vColor	: SV_TARGET;
};

cbuffer TextureSize	: register(b0)
{
	matrix	g_matWVP;
};

//점 보간
SamplerState	g_PointSmp	: register(s0);

Texture2D		g_DiffuseTex : register(t0);