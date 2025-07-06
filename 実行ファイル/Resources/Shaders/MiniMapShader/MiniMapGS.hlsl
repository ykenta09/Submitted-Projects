#include "MiniMap.hlsli"

// 頂点数（板ポリ1枚＝4頂点）
static const int vnum = 4;

// 板ポリの各頂点のオフセット。パーティクル中心を基準にしたローカル座標系
static const float4 offset_array[vnum] =
{
	float4(-1.0f,  1.0f, 0.0f, 0.0f),	//	左上
	float4( 1.0f,  1.0f, 0.0f, 0.0f),	//	右上
	float4(-1.0f, -1.0f, 0.0f, 0.0f),	//	左下
	float4( 1.0f, -1.0f, 0.0f, 0.0f),	//	右下

};

// 最大出力頂点数を指定（今回4頂点）
[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],			  // ジオメトリシェーダーの入力（1つの頂点＝パーティクルの中心情報）
	inout TriangleStream<PS_INPUT> output // 出力ストリーム。三角形ストリップとして頂点を出力
)
{
    // 板ポリ用の4頂点をループで生成
	for (int i = 0; i < vnum; i++)
	{
		PS_INPUT element;

		float4 res = offset_array[i];
	  
	    // パーティクル中心位置にオフセットを加えて、頂点のワールド空間座標を計算
		element.Position = input[0].Position + mul(offset_array[i], matWorld);

        // ビュー変換・プロジェクション行列
		element.Position = mul(element.Position, matView);
		element.Position = mul(element.Position, matProj);

        // テクスチャ座標を [-1,1] -> [0,1] の範囲に変換
		element.Color = input[0].Color;
		element.TexCoord.x =  offset_array[i].x + 1.0f;
		element.TexCoord.y = -offset_array[i].y + 1.0f;
		element.TexCoord /= 2.0f;
	  
	    // 頂点を出力ストリームに追加
		output.Append(element);
	}
    // 三角形ストリップを区切る
	output.RestartStrip();
}