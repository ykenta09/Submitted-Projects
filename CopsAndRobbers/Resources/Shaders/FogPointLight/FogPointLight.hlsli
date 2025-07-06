
// ワールド・ビュー・プロジェクション行列を格納する定数バッファ（b0にバインド）
cbuffer MatrixBuffer : register(b0)
{
   matrix World;       // ワールド行列（モデルの位置・回転・スケール変換）
   matrix View;        // ビュー行列（カメラの視点変換）
   matrix Projection;  // プロジェクション行列（射影変換）
};

// フォグ用パラメータを格納する定数バッファ（b1にバインド）
cbuffer FogBuffer : register(b1)
{
   float4 FogColor;          // フォグの色（RGBA）
   float  FogStart;          // フォグの開始距離
   float  FogEnd;            // フォグが最大になる距離
   float2 padding;           // パディング（16バイト境界合わせ用、未使用）
   float3 CameraPosition;    // カメラのワールド座標
   float  padding2;          // パディング（16バイト境界合わせ用、未使用）
};

// モデルテクスチャの
cbuffer UVScaleBuffer : register(b2)
{
   float2 UVScale;
   float2 padding3;
}

//ライトの定数バッファ(ディレクショナルライト)
cbuffer LightBuffer : register(b3)
{
   float3 lightDirection;           //ライトの方向(ワールド空間、正規済み）
   float padding4;
   float4 lightDiffuseColor;        //ライトのディフューズ
   float4 ambientColor;             //アンビエント色
};

#define MAX_POINT_LIGHTS 5

//ポイントライトの構造体とバッファ
struct PointLight
{
   float3 Position;        //ライトの座標
   float range;            //広さ
   float3 color;           //ライトの色
   float intensity;        //ライトの強さ
};
cbuffer PointLightBuffer : register(b4)
{
   PointLight pointLights[MAX_POINT_LIGHTS];
   int numLights;
   float3 padding5;
}

// 頂点シェーダーへの入力構造体（頂点バッファ1頂点分のデータを受け取る）
struct VS_INPUT
{
   float3 Position   : POSITION; // 頂点の座標（ワールド空間）
   float3 Normal     : NORMAL;     // 法線ベクトル
   float4 Color      : COLOR; // 頂点カラー（RGBA）
   float2 TexCoord   : TEXCOORD0; // テクスチャ座標（UV）
};

// 頂点シェーダーからピクセルシェーダーへの出力構造体
struct PS_INPUT
{
   float4 Position : SV_POSITION; // クリップ空間の頂点座標（ラスタライザ用）
   float2 TexCoord : TEXCOORD0; // テクスチャ座標（UV、ピクセルシェーダーに渡す）
   float3 WorldPos : TEXCOORD1; // ワールド空間の頂点座標（フォグやライティング用）
   float3 WorldNormal : TEXCOORD2; // ワールド空間法線（追加）
};