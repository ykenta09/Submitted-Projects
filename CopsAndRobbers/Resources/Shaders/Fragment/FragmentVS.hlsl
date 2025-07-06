#include "Fragment.hlsli"

// 頂点シェーダーのメイン関数
PS_INPUT main(VS_INPUT input)
{
   // 出力構造体を初期化
   PS_INPUT output = (PS_INPUT) 0;
   // テクスチャ座標、カラー、テクスチャ座標をそのまま出力
   output.Pos = float4(input.Pos, 1);
   output.Color = input.Color;
   output.Tex = input.Tex;
   
   return output;
}

