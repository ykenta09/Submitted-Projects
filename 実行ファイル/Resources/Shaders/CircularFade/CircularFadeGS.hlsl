#include "CircularFade.hlsli"

// 出力する頂点数を定数で指定（今回は4つ＝四角形の頂点）
static const int vnum = 4;

// 四隅の頂点位置を定義した配列（スクリーン座標系でのオフセット）
static const float4 offset_array[vnum] =
{
   float4(-1.0f, 1.0f, 0.0f, 0.0f), // 左上
    float4(1.0f, 1.0f, 0.0f, 0.0f), // 右上
    float4(-1.0f, -1.0f, 0.0f, 0.0f), // 左下
    float4(1.0f, -1.0f, 0.0f, 0.0f), // 右下
};

// ジオメトリシェーダー本体
// [maxvertexcount(vnum)] 属性で最大出力頂点数を指定（必須）[1][3]
// point PS_INPUT input[1] : 入力は1頂点のポイントプリミティブ
// inout TriangleStream<PS_INPUT> output : 出力ストリーム
[maxvertexcount(vnum)]
void main(
    point PS_INPUT input[1],
    inout TriangleStream<PS_INPUT> output
)
{
    // 4頂点分ループして四角形を生成
   for (int i = 0; i < vnum; i++)
   {
      PS_INPUT element;

        // オフセット配列からスクリーン座標を取得
      float4 res = offset_array[i];

        // 入力頂点の座標にワールド行列でオフセットを加算
      element.Pos = input[0].Pos + mul(offset_array[i], matWorld);

        // ビュー・プロジェクション行列で座標変換
      element.Pos = mul(element.Pos, matView);
      element.Pos = mul(element.Pos, matProj);

        // 頂点カラーをそのままコピー
      element.Color = input[0].Color;

        // テクスチャ座標（UV）を四角形用に変換
      element.Tex.x = offset_array[i].x + 1.0f;
      element.Tex.y = -offset_array[i].y + 1.0f;
      element.Tex /= 2.0f;

        // 頂点をストリームに追加
      output.Append(element);
   }
    // ストリップを終了
   output.RestartStrip();
}
