
#include "TextureViewerShare.fx"

struct VS_INPUT_2D
{
	// Vector3
	// semantic : POSITION 레지스터를 이용하겠다 라는 것이다.
	// semantic을 이용해서 입력레지스터의 이름과 번호를 지정해서
	// 가져오게 한다.
	// 이름 뒤에 번호를 안붙이면 0번으로 처리한다.
	float3	vPos	: POSITION;
	float2	vUV		: TEXCOORD;
	float4	vColor	: COLOR;
};

struct VS_OUTPUT_2D
{
	// 레지스터 이름 앞에 SV_ 가 붙는것들이 있다.
	// 이 값들은 시스템 밸류이다. 지정해주면 다른곳에서 바꿔서는 안된다.
	// 시스템에서 사용할 특수한 값들이다.
	float4	vPos	: SV_POSITION;
	float2	vUV		: TEXCOORD;
	float4	vColor	: COLOR;
};


//Editor용 정점 Shader
VS_OUTPUT_2D Standard2DTextureViewVS(VS_INPUT_2D input)
{
	VS_OUTPUT_2D output = (VS_OUTPUT_2D)0;

	output.vPos = mul(float4(input.vPos, 1.f), g_matWVP);
	//output.vPos = float4(input.vPos, 1.f);
	output.vColor = input.vColor;
	output.vUV = input.vUV;

	return output;
}

PS_OUTPUT_SINGLE Standard2DTextureViewPS(VS_OUTPUT_2D input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vColor = g_DiffuseTex.Sample(g_PointSmp, input.vUV);
	//output.vColor = input.vColor;

	return output;
}

VS_OUTPUT_2D Standard2DRectViewVS(VS_INPUT_2D input)
{
	VS_OUTPUT_2D output = (VS_OUTPUT_2D)0;

	output.vPos = mul(float4(input.vPos, 1.f), g_matWVP);
	output.vColor = input.vColor;
	output.vUV = input.vUV;

	return output;
}

PS_OUTPUT_SINGLE Standard2DRectViewPS(VS_OUTPUT_2D input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vColor = input.vColor;

	return output;
}