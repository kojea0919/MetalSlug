
#include "Share.fx"

struct VS_INPUT_UI
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

struct VS_OUTPUT_UI
{
	// 레지스터 이름 앞에 SV_ 가 붙는것들이 있다.
	// 이 값들은 시스템 밸류이다. 지정해주면 다른곳에서 바꿔서는 안된다.
	// 시스템에서 사용할 특수한 값들이다.
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

    //체크가 된 경우 Check이미지 Setting
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