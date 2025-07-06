#include "MiniMap.hlsli"

// 頂点シェーダーのメイン関数
PS_INPUT main(VS_INPUT input)
{
   // 出力構造体を初期化
   PS_INPUT output = (PS_INPUT) 0;
    
   // 頂点カラーをそのまま出力に渡す（補間されてピクセルシェーダに渡る）
   output.Position = mul(float4(input.Pos, 1.0f), matWorld);
   output.Position = mul(output.Position, matView);
   output.Position = mul(output.Position, matProj);
   // テクスチャ座標もそのまま出力
   output.Color = input.Color;
   output.TexCoord = input.Tex;
    
   return output;
}
