
struct PS_OUTPUT_SINGLE
{
	// SV_TARGET�� ����Ÿ���� �ǹ��Ѵ�.
	// DX11���� ����Ÿ���� 8������ �Ѳ����� ����� �����ѵ�
	// ���� �⺻���� ���¶�� ������� ����Ÿ���� 0���� �����Ǿ� �ִ�.
	// �Ʒ��� 0��Ÿ�ٿ� ����ϰڴٴ� ���̹Ƿ� �⺻ ��Ȳ�̶��
	// ����ۿ� ������ ���ڴٶ�� ���̴�.
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

//���� ����
SamplerState	g_LinearSmp : register(s0);

//�� ����
SamplerState	g_PointSmp	: register(s1);

//���� ���͸�
SamplerState	g_Anisotropic : register(s2);

Texture2D		g_DiffuseTex : register(t0);

#define IMAGE_ATLAS 0
#define IMAGE_ARRAY 1

float2 ComputeSpriteUV(float2 vUV)
{
	//�̹��� Ÿ���� Atlas�� ��쿡�� UV Setting
	//------------------------------------------
	if (g_iSpriteImageType == IMAGE_ARRAY)
		return vUV;
	
	float2 vResult = (float2)0.f;

	//�簢�� ������ ���� ��� 
	if (vUV.x > 0.f)
		vResult.x = g_vSpriteFrameEnd.x / g_vSpriteImageSize.x;

	//�簢�� ���� ���� ���
	else
		vResult.x = g_vSpriteFrameStart.x / g_vSpriteImageSize.x;

	//�簢�� ���� ���� ���
	if (vUV.y > 0.f)
		vResult.y = g_vSpriteFrameEnd.y / g_vSpriteImageSize.y;

	//�簢�� �Ʒ��� ���� ���
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