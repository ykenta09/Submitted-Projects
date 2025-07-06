#include "CircularFade.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

// �A���t�@�l���v�Z����֐��i�~�`�̗̈�� alpha �� 0 �ɂ���j
float CalcAlpha(float baseAlpha, float2 uv, float r)
{
   float aspect = 1280.0f / 720.0f;

   // uv �� x ���W�ɃA�X�y�N�g���K�p
   uv.x *= aspect;

   // ���S����̋������v�Z
   float dist = distance(uv, float2(0.5f * aspect, 0.5f));

   // ���S����̋��������a�ȏ�Ȃ� 0.0f�A�����łȂ���� 1.0f
   return (dist >= r) ? 0.0f : 1.0f;
}

float4 main(PS_INPUT input) : SV_TARGET
{
   // �^�����s�N�Z��
   float4 outputb = float4(0, 0, 0, 0);
   float2 uv = input.Tex;

   // sin(time) ���g���� 0 �` 1 �͈̔͂ɕϊ�
   float r = sin(time.x); // 0�`1�͈̔͂ŉ~�̔��a��ω�������
   r *= 1.5f;

   // �A���t�@�l���v�Z
   float alpha = CalcAlpha(outputb.a, uv, r);

   return float4((float3) outputb.xyz, alpha);
}
