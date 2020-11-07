
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


#define LeftRight	0
#define RightLeft	1
#define TopBottom	2
#define BottomTop	3

cbuffer Bar : register(b10)
{
	int		g_iBarDir;
	float	g_fBarPercent;
	float2	g_vBarEmpty;
}


VS_OUTPUT_UI UIBarVS(VS_INPUT_UI input)
{
    VS_OUTPUT_UI output = (VS_OUTPUT_UI)0;

    float3 vPos = input.vPos - g_vMeshSize * g_vPivot;

    output.vPos = mul(float4(vPos, 1.f), g_matWVP);
    output.vColor = input.vColor;
    output.vUV = input.vUV;

    return output;
}

PS_OUTPUT_SINGLE UIBarPS(VS_OUTPUT_UI input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

    bool bRender = true;

    if (g_iBarDir == LeftRight)
    {
        if (input.vUV.x > g_fBarPercent)
            bRender = false;
    }

    if (g_iBarDir == RightLeft)
    {
        if (input.vUV.x < 1.f - g_fBarPercent)
            bRender = false;
    }

    if (g_iBarDir == TopBottom)
    {
        if (input.vUV.y > g_fBarPercent)
            bRender = false;
    }

    if (g_iBarDir == BottomTop)
    {
        if (input.vUV.y < 1.f - g_fBarPercent)
            bRender = false;
    }

    if (!bRender)
        clip(-1);

    output.vColor = g_DiffuseTex.Sample(g_Anisotropic, input.vUV) * g_vMtrlDif;

    return output;
}

cbuffer Button : register(b11)
{
    int     g_iCheck;
    float3  g_vButtonEmpty;
}

Texture2D g_CheckTexture :register(t10);

PS_OUTPUT_SINGLE UICheckPS(VS_OUTPUT_UI input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

    //üũ�� �� ��� Check�̹��� Setting
    if (g_iCheck == 0)
        output.vColor = g_DiffuseTex.Sample(g_Anisotropic, input.vUV) * g_vMtrlDif;

    else
    {
        float4 vCheckColor = g_CheckTexture.Sample(g_Anisotropic, input.vUV);
        float4 vColor = g_DiffuseTex.Sample(g_Anisotropic, input.vUV);

        output.vColor.rgb = (vCheckColor.rgb * vCheckColor.a + vColor.rgb * (1.f - vCheckColor.a)) * g_vMtrlDif.rgb;
        output.vColor.a = vColor.a * g_vMtrlDif.a;
    }

    return output;
}