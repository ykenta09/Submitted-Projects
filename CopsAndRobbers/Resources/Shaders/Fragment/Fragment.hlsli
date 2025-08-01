//定数バッファ
cbuffer ConstBuffer	: register(b0)
{
    matrix matWorld;    //ワールド行列
	matrix matView;		//ビュー行列
	matrix matProj;		//射影行列
	float4 diffuse;		//ディフューズカラー
    float4 time;		//タイム
    float4 distance;	//距離
};

//頂点シェーダーへの入力構造体
struct VS_INPUT
{
    float3 Pos : POSITION;    //頂点の位置
	float4 Color: COLOR;	  //頂点カラー
	float2 Tex : TEXCOORD;	  //テクスチャ座標(uv)
};

//ピクセルシェーダーの入力構造体
struct PS_INPUT
{
   float4 Pos : SV_POSITION;     //返還後の画面空間座標
   float4 Color : COLOR;		 //補間された頂点カラー
   float2 Tex : TEXCOORD;		 //補間されたテクスチャ座標(uv)
};

