#include "Particle.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

// アルファ値を計算する関数（円形の領域で alpha を 0 にする）
float CalcAlpha(float baseAlpha, float2 uv)
{
   float aspect = 1280.0f / 720.0f;
   float r = saturate(time.x); // 時間に応じて r を増加させ、0.0～1.0の範囲に収める

   // uv の x 座標にアスペクト比を適用
   uv.x *= aspect;

   // 中心からの距離が r 以下なら 0.0f、r より大きいなら 1.0f
   float2 center = float2(0.5f * aspect, 0.5f);
   return (distance(uv, center) <= r) ? 0.0f : 1.0f;
}

// アルファ値を計算する関数（円形の領域で alpha を 1 にする）
float CalcAlpha2(float baseAlpha, float2 uv)
{
   float aspect = 1280.0f / 720.0f;
   float r = saturate(time.x); // 時間に応じて r を増加させ、0.0～1.0の範囲に収める

   // uv の x 座標にアスペクト比を適用
   uv.x *= aspect;

   // 中心からの距離が r 以下なら 1.0f、r より大きいなら 0.0f
   float2 center = float2(0.5f * aspect, 0.5f);
   return (distance(uv, center) <= r) ? 1.0f : 0.0f;
}

float4 main(PS_INPUT input) : SV_TARGET
{
   float4 outputb = float4(0, 0, 0, 0);
   float2 uv = input.Tex;

   // CalcAlpha関数でalphaを計算
   float alpha = CalcAlpha(outputb.a, uv);
   return float4(outputb.xyz, alpha);
}
