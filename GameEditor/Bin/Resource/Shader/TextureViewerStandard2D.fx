
#include "TextureViewerShare.fx"

struct VS_INPUT_2D
{
	// Vector3
	// semantic : POSITION �������͸� �̿��ϰڴ� ��� ���̴�.
	// semantic�� �̿��ؼ� �Է·��������� �̸��� ��ȣ�� �����ؼ�
	// �������� �Ѵ�.
	// �̸� �ڿ� ��ȣ�� �Ⱥ��̸� 0������ ó���Ѵ�.
	float3	vPos	: POSITION;
	float2	vUV		: TEXCOORD;
	float4	vColor	: COLOR;
};

struct VS_OUTPUT_2D
{
	// �������� �̸� �տ� SV_ �� �ٴ°͵��� �ִ�.
	// �� ������ �ý��� ����̴�. �������ָ� �ٸ������� �ٲ㼭�� �ȵȴ�.
	// �ý��ۿ��� ����� Ư���� �����̴�.
	float4	vPos	: SV_POSITION;
	float2	vUV		: TEXCOORD;
	float4	vColor	: COLOR;
};


//Editor�� ���� Shader
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