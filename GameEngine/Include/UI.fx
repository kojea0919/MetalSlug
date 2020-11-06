
#include "Share.fx"

struct VS_INPUT_UI
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

struct VS_OUTPUT_UI
{
	// �������� �̸� �տ� SV_ �� �ٴ°͵��� �ִ�.
	// �� ������ �ý��� ����̴�. �������ָ� �ٸ������� �ٲ㼭�� �ȵȴ�.
	// �ý��ۿ��� ����� Ư���� �����̴�.
	float4	vPos	: SV_POSITION;
	float2	vUV		: TEXCOORD;
	float4	vColor	: COLOR;
};

VS_OUTPUT_UI UITexVS(VS_INPUT_UI input)
{
	VS_OUTPUT_UI	output = (VS_OUTPUT_UI)0;

	float3	vPos = input.vPos - g_vMeshSize * g_vPivot;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vColor = input.vColor;
	output.vUV = input.vUV;

	return output;
}

PS_OUTPUT_SINGLE UITexPS(VS_OUTPUT_UI input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vColor = g_DiffuseTex.Sample(g_Anisotropic, input.vUV) * g_vMtrlDif;

	return output;
}
