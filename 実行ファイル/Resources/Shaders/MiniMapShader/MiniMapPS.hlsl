#include "MiniMap.hlsli"

Texture2D texQuad : register(t0);      // テクスチャリソース
SamplerState samLinear : register(s0); // サンプラーステート

float4 main(PS_INPUT input) : SV_TARGET
{
   //入力されたuvを取得
   float2 uv = input.TexCoord;
   
   //uv位置のテクスチャの色を取得する
   float4 quadColor = texQuad.Sample(samLinear, uv);
   
    // 円形テクスチャが描画されている部分のアルファ値を 0 にする
   if (quadColor.a > 0.0f)
   {
      return float4(0.0f, 0.0f, 0.0f, 0.0f); // 透明で表示（デバッグ用）
   }

   //アルファ値がある場所はグレーを表示
   return float4(0.4f, 0.4f, 0.4f, 0.98f);;
}
