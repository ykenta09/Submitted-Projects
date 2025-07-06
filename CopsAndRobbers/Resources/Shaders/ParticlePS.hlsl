#include "Particle.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

// �A���t�@�l���v�Z����֐��i�~�`�̗̈�� alpha �� 0 �ɂ���j
float CalcAlpha(float baseAlpha, float2 uv)
{
   float aspect = 1280.0f / 720.0f;
   float r = saturate(time.x); // ���Ԃɉ����� r �𑝉������A0.0�`1.0�͈̔͂Ɏ��߂�

   // uv �� x ���W�ɃA�X�y�N�g���K�p
   uv.x *= aspect;

   // ���S����̋����� r �ȉ��Ȃ� 0.0f�Ar ���傫���Ȃ� 1.0f
   float2 center = float2(0.5f * aspect, 0.5f);
   return (distance(uv, center) <= r) ? 0.0f : 1.0f;
}

// �A���t�@�l���v�Z����֐��i�~�`�̗̈�� alpha �� 1 �ɂ���j
float CalcAlpha2(float baseAlpha, float2 uv)
{
   float aspect = 1280.0f / 720.0f;
   float r = saturate(time.x); // ���Ԃɉ����� r �𑝉������A0.0�`1.0�͈̔͂Ɏ��߂�

   // uv �� x ���W�ɃA�X�y�N�g���K�p
   uv.x *= aspect;

   // ���S����̋����� r �ȉ��Ȃ� 1.0f�Ar ���傫���Ȃ� 0.0f
   float2 center = float2(0.5f * aspect, 0.5f);
   return (distance(uv, center) <= r) ? 1.0f : 0.0f;
}

float4 main(PS_INPUT input) : SV_TARGET
{
   float4 outputb = float4(0, 0, 0, 0);
   float2 uv = input.Tex;

   // CalcAlpha�֐���alpha���v�Z
   float alpha = CalcAlpha(outputb.a, uv);
   return float4(outputb.xyz, alpha);
}
