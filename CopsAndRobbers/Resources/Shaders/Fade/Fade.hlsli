// 定数バッファ（b0にバインド）: 行列や定数値をCPU側からGPUへ渡す
cbuffer ConstBuffer : register(b0)
{
   matrix matWorld;      // ワールド行列
   matrix matView;       // ビュー行列
   matrix matProj;       // プロジェクション行列
   float4 diffuse;       // ディフューズカラー
   float4 time;          // 時間やアニメーション等で使うfloat4定数
};

// 頂点シェーダーへの入力構造体
struct VS_INPUT
{
   float3 Pos : POSITION;     // 頂点の座標（ワールド空間）
   float4 Color : COLOR;      // 頂点カラー（RGBA）
   float2 Tex : TEXCOORD;     // テクスチャ座標（UV）
};

// ピクセルシェーダーへの入力構造体（頂点シェーダーから渡される）
struct PS_INPUT
{
   float4 Pos : SV_POSITION;     // クリップ空間の頂点座標（ラスタライザ用、必須）[1][2][3][4]
   float4 Color : COLOR;         // 補間された頂点カラー（RGBA）
   float2 Tex : TEXCOORD;        // 補間されたテクスチャ座標（UV）
};
