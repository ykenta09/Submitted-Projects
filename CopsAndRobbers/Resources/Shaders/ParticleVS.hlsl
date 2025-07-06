#include "Particle.hlsli"

// 頂点シェーダーのメイン関数
PS_INPUT main(VS_INPUT input)
{
   PS_INPUT output = (PS_INPUT) 0; // 出力構造体を初期化（全メンバーを 0 にする）

    // 頂点位置に w = 1 を加えて float3 -> float4 に変換（同次座標系への変換）
   output.Pos = float4(input.Pos, 1);

    // 頂点カラーをそのまま出力に渡す（補間されてピクセルシェーダに渡る）
   output.Color = input.Color;

    // テクスチャ座標もそのまま出力（これもピクセルシェーダで使われる）
   output.Tex = input.Tex;

   return output; // ピクセルシェーダに渡すデータを返す
}
