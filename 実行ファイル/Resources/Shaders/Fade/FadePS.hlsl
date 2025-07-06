#include "Fade.hlsli" 

// テクスチャ（t0, t1にバインド）
Texture2D tex : register(t0); // メインのテクスチャ
Texture2D tex2 : register(t1); // サブのテクスチャ（未使用）
SamplerState samLinear : register(s0); // 線形補間サンプラー


// アルファ値（透明度）を計算する関数
float CalcAlpha(float baseAlpha, float2 uv)
{
   float ret = baseAlpha; // 基本のアルファ値で初期化

   // time.y > 0.5 ならフェードイン中
   if (time.y > 0.5) // フェードインの場合
   {
      ret = 1.0f - time.x; // time.xが0→1で透明→不透明
   }
   // time.z > 0.5 ならフェードアウト中
   else if (time.z > 0.5) // フェードアウトの場合
   {
      ret = time.x; // time.xが0→1で不透明→透明
   }

   return ret; // 計算したアルファ値を返す
}

// ピクセルシェーダーのエントリポイント
float4 main(PS_INPUT input) : SV_TARGET
{
   // 出力色を真っ黒で初期化（RGB=0, Alpha=0）
   float4 outputb = float4(0, 0, 0, 0);

   // テクスチャ座標を取得
   float2 uv = input.Tex;
      
   // フェード状態に応じてアルファ値を計算
   outputb.a = CalcAlpha(outputb.a, uv);

   // ピクセルカラーを返す
   return outputb;
}
