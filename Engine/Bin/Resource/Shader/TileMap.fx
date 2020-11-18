
#include "Share.fx"

struct VS_INPUT_TILE
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    float4 vColor : COLOR;
    matrix matWVP : WORLD;
    float3 vMeshSize : MESHSIZE;
    float3 vMeshPivot : PIVOT;
};

struct VS_OUTPUT_TILE
{
    float4 vPos : SV_POSITION;
    float2 vUV : TEXCOORD;
    float4 vColor : COLOR;
};

VS_OUTPUT_TILE TileMapVS(VS_INPUT_TILE input)
{
    VS_OUTPUT_TILE output = (VS_OUTPUT_TILE)0;

    float3 vPos = input.vPos - g_vMeshSize * g_vPivot;

    output.vPos = mul(float4(vPos, 1.f), input.matWVP);
    output.vColor = input.vColor;
    output.vUV = input.vUV;

    return output;
}

PS_OUTPUT_SINGLE TileMapPS(VS_OUTPUT_TILE input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

    output.vColor = g_DiffuseTex.Sample(g_Anisotropic, input.vUV) * g_vMtrlDif;

    return output;
}