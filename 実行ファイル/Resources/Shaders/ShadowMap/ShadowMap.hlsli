/*
    ���ʂŎg�p����p�����[�^
*/

// DirectXTK�ŕW���I�ɒ�`����Ă���p�����[�^
Texture2D<float4> Texture : register(t0);
sampler Sampler : register(s0);

cbuffer Parameters : register(b0)
{
   float4x4 WorldViewProj;
};


// ���C�g�p�萔�o�b�t�@
cbuffer LightParameters : register(b1)
{
   float4x4 lightViewProjection : packoffset(c0);
   float4 lightPosition : packoffset(c4);
   float3 lightDirirection : packoffset(c5);
   float3 lightAmbient : packoffset(c6);
};

// �V���h�E�}�b�v�̒��_�V�F�[�_���͗p
struct VS_SM_Input
{
   float4 Position : SV_Position;
};

// �V���h�E�}�b�v�̃s�N�Z���V�F�[�_���͗p
struct PS_SM_Input
{
   float4 Position : SV_Position;
};
