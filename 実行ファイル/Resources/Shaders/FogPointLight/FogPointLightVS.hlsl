#include "FogPointLight.hlsli" 

// 頂点シェーダーのエントリポイント
PS_INPUT main(VS_INPUT input)
{
   PS_INPUT output = (PS_INPUT) 0; // 出力構造体を初期化（全メンバーを 0 にする）

   // 頂点座標をワールド空間に変換（float3→float4に拡張してWorld行列を掛ける）
   float4 worldPos = mul(float4(input.Position.xyz, 1.0f), World);

   // ワールド空間の頂点座標をビュー・プロジェクション行列で変換し、クリップ空間座標にする
   output.Position = mul(worldPos, mul(View, Projection));

   // ワールド空間の頂点座標（xyz成分のみ）を出力
   output.WorldPos = worldPos.xyz;
   
   // テクスチャ座標（UV）をそのまま出力
   output.TexCoord = input.TexCoord;

   // 法線をワールド空間に変換（ワールド行列の逆転置行列を使うのが一般的）
    // 簡易版としてワールド行列の回転成分のみを使う場合は下記のように
   float3 worldNormal = mul(input.Normal, (float3x3) World);
   output.WorldNormal = normalize(worldNormal); // 正規化
   
   // 頂点シェーダーの出力を返す
   return output;
}
