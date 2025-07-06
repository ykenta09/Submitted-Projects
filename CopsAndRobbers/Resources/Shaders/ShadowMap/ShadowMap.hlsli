/*
    共通で使用するパラメータ
*/

// DirectXTKで標準的に定義されているパラメータ
Texture2D<float4> Texture : register(t0);
sampler Sampler : register(s0);

cbuffer Parameters : register(b0)
{
   float4x4 WorldViewProj;
};


// ライト用定数バッファ
cbuffer LightParameters : register(b1)
{
   float4x4 lightViewProjection : packoffset(c0);
   float4 lightPosition : packoffset(c4);
   float3 lightDirirection : packoffset(c5);
   float3 lightAmbient : packoffset(c6);
};

// シャドウマップの頂点シェーダ入力用
struct VS_SM_Input
{
   float4 Position : SV_Position;
};

// シャドウマップのピクセルシェーダ入力用
struct PS_SM_Input
{
   float4 Position : SV_Position;
};
