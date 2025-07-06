#include "CircularFade.hlsli" 

// 頂点シェーダーのエントリポイント
PS_INPUT main(VS_INPUT input)
{
   PS_INPUT output = (PS_INPUT) 0; // 出力構造体をゼロ初期化

    // 頂点座標をfloat4型に拡張して出力
   output.Pos = float4(input.Pos, 1);

    // 頂点カラーをそのまま出力
   output.Color = input.Color;

    // テクスチャ座標（UV）をそのまま出力
   output.Tex = input.Tex;

    // ピクセルシェーダーに渡す
   return output;
}
