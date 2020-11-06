
#include "Share.fx"

struct VS_INPUT_COLLIDER
{
	float3	vPos	: POSITION;
};

struct VS_OUTPUT_COLLIDER
{
	float4	vPos	: SV_POSITION;
};

VS_OUTPUT_COLLIDER ColliderVS(VS_INPUT_COLLIDER input)
{
	VS_OUTPUT_COLLIDER output = (VS_OUTPUT_COLLIDER)0;

	output.vPos = mul(float4(input.vPos, 1.f), g_matWVP);

	return output;
}

PS_OUTPUT_SINGLE ColliderPS(VS_OUTPUT_COLLIDER input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vColor = g_vMtrlDif;

	return output;
}
