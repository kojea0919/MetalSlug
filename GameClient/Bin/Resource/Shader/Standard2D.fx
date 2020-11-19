
#include "Share.fx"

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

struct VS_INPUT_2D_INSTANCING
{
	// Vector3
	// semantic : POSITION �������͸� �̿��ϰڴ� ��� ���̴�.
	// semantic�� �̿��ؼ� �Է·��������� �̸��� ��ȣ�� �����ؼ�
	// �������� �Ѵ�.
	// �̸� �ڿ� ��ȣ�� �Ⱥ��̸� 0������ ó���Ѵ�.
	float3 vPos : POSITION;
	float2 vUV : TEXCOORD;
	float4 vColor : COLOR;
	matrix matWVP : WORLD;
	float3 vMeshSize : MESHSIZE;
	float3 vMeshPivot : PIVOT;
	float2 vFrameStart : FRAMESTART;
	float2 vFrameEnd : FRAMEEND;
	float2 vImageSize : IMAGESIZE;
};

VS_OUTPUT_2D Standard2DInstancing(VS_INPUT_2D_INSTANCING input)
{
	VS_OUTPUT_2D output = (VS_OUTPUT_2D)0;

	float3 vPos = input.vPos - input.vMeshSize * input.vMeshPivot;

	output.vPos = mul(float4(vPos, 1.f), input.matWVP);
	output.vColor = input.vColor;
	output.vUV = ComputeSpriteUV(input.vUV, input.vFrameStart, input.vFrameEnd, input.vImageSize);

	return output;
}

// ���� input�� �������Ͱ� �����Ǿ� �ֱ� ������ ���ڸ� �ȳѰ��־
// �Է·������Ϳ� �� �ִ� ���� �����ͼ� ����ϰ� �ȴ�.
VS_OUTPUT_2D Standard2DColorVS(VS_INPUT_2D input)
{
	VS_OUTPUT_2D	output = (VS_OUTPUT_2D)0;

	float3	vPos = input.vPos - g_vMeshSize * g_vPivot;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vColor = input.vColor;
	output.vUV = input.vUV;

	return output;
}

PS_OUTPUT_SINGLE Standard2DColorPS(VS_OUTPUT_2D input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	if (g_iMtrlDifTex == 0)
		output.vColor = input.vColor * g_vMtrlDif;

	else
		output.vColor = g_DiffuseTex.Sample(g_Anisotropic, input.vUV) * g_vMtrlDif;

	return output;
}

VS_OUTPUT_2D Standard2DTexVS(VS_INPUT_2D input)
{
	VS_OUTPUT_2D	output = (VS_OUTPUT_2D)0;

	float3	vPos = input.vPos - g_vMeshSize * g_vPivot;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vColor = input.vColor;
	output.vUV = input.vUV;

	return output;
}

PS_OUTPUT_SINGLE Standard2DTexPS(VS_OUTPUT_2D input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	float4 vColor = g_DiffuseTex.Sample(g_Anisotropic, input.vUV);

	if (vColor.a <= 0.03f)
		clip(-1);

	output.vColor = vColor * g_vMtrlDif;

	return output;
}

//Sprite�� ���� Shader
VS_OUTPUT_2D Standard2DSpriteVS(VS_INPUT_2D input)
{
	VS_OUTPUT_2D	output = (VS_OUTPUT_2D)0;

	float3	vPos = input.vPos - g_vMeshSize * g_vPivot;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vColor = input.vColor;
	output.vUV = ComputeSpriteUV(input.vUV);

	return output;
}

PS_OUTPUT_SINGLE Standard2DSpritePS(VS_OUTPUT_2D input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	float4 vColor = g_DiffuseTex.Sample(g_Anisotropic, input.vUV);

	if (vColor.a <= 0.03f)
		clip(-1);

	output.vColor = vColor * g_vMtrlDif;

	return output;
}

//Random����� PixelShader
PS_OUTPUT_SINGLE Standard2DRandomColorPS(VS_OUTPUT_2D input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	float4 outputcolor = g_DiffuseTex.Sample(g_Anisotropic, input.vUV) * float4(RandomR(input.vUV,g_vDeltaTime.x), RandomG(input.vUV, g_vDeltaTime.x), RandomB(input.vUV, g_vDeltaTime.x), 1);
	//float4 outputcolor = g_DiffuseTex.Sample(g_Anisotropic, input.vUV) * float4(0, 0, 0, 1);
	output.vColor = outputcolor;

	return output;
}