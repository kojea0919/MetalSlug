
struct PS_OUTPUT_SINGLE
{
	// SV_TARGET�� ����Ÿ���� �ǹ��Ѵ�.
	// DX11���� ����Ÿ���� 8������ �Ѳ����� ����� �����ѵ�
	// ���� �⺻���� ���¶�� ������� ����Ÿ���� 0���� �����Ǿ� �ִ�.
	// �Ʒ��� 0��Ÿ�ٿ� ����ϰڴٴ� ���̹Ƿ� �⺻ ��Ȳ�̶��
	// ����ۿ� ������ ���ڴٶ�� ���̴�.
	float4	vColor	: SV_TARGET;
};

cbuffer TextureSize	: register(b0)
{
	matrix	g_matWVP;
};

//�� ����
SamplerState	g_PointSmp	: register(s0);

Texture2D		g_DiffuseTex : register(t0);