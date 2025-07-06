#include "CircularFade.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

// アルファ値を計算する関数（円形の領域で alpha を 0 にする）
float CalcAlpha(float baseAlpha, float2 uv, float r)
{
   float aspect = 1280.0f / 720.0f;

   // uv の x 座標にアスペクト比を適用
   uv.x *= aspect;

   // 中心からの距離を計算
   float dist = distance(uv, float2(0.5f * aspect, 0.5f));

   // 中心からの距離が半径以上なら 0.0f、そうでなければ 1.0f
   return (dist >= r) ? 0.0f : 1.0f;
}

float4 main(PS_INPUT input) : SV_TARGET
{
   // 真っ黒ピクセル
   float4 outputb = float4(0, 0, 0, 0);
   float2 uv = input.Tex;

   // sin(time) を使って 0 ～ 1 の範囲に変換
   float r = sin(time.x); // 0～1の範囲で円の半径を変化させる
   r *= 1.5f;

   // アルファ値を計算
   float alpha = CalcAlpha(outputb.a, uv, r);

   return float4((float3) outputb.xyz, alpha);
}
