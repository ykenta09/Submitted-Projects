#include "Fragment.hlsli"

Texture2D tex : register(t0);            // テクスチャリソース1
Texture2D tex2 : register(t1);           // テクスチャリソース2
SamplerState samLinear : register(s0);   // サンプラーステート


float4 main(PS_INPUT input) : SV_TARGET
{
   //入力されたuvを取得
   float2 uv = input.Tex;

   // プレイヤーと敵の距離を取得
   float currentDistance = distance.x;    //現在の距離
   float prevDistance = distance.y;       //前回の距離

   // 距離が20.0未満で、四角形の境界部分の場合に処理を行う
   if (currentDistance < 20.0 && (uv.x < 0.1 || uv.x > 0.9 || uv.y < 0.1 || uv.y > 0.9))
   {
      // 四角の内側に行くほど透明になるようにグラデーションを加える
      float baseAlpha = 1.0 - (min(min(uv.x, 1.0 - uv.x), min(uv.y, 1.0 - uv.y)) / 0.1); // グラデーションを増やす

      // 距離に応じてどくどくする効果を変化させる
      float pulseSlow = sin(time.x * 1.5);
      float pulseFast = sin(time.x * 3.0f);
      float pulse = lerp(pulseSlow, pulseFast, step(prevDistance, 15.0) * smoothstep(10.0, 5.0, currentDistance));
      // step(prevDistance, 15.0)は、前回の距離が15以上の場合に1.0を返す
      // smoothstep(10.0, 5.0, currentDistance)は、距離が10から5の範囲で0から1に変化する
      
      // パルスの影響を増やす
      float pulseMultiplier = clamp(pulse * 0.6, 0.1, 0.6); // 範囲を制限
      // 0.4を変えると、パルスの影響が強くなる
      // 0.1と0.6は、パルスが最小値と最大値を決める
      
      // グラデーション部分は固定のアルファ値を使用し、パルスはその上に乗算
      return float4(0.8, 0.0, 0.0, baseAlpha * pulseMultiplier); // 赤色をアルファ値で調整
   }
   else
   {
      return float4(0.0, 0.0, 0.0, 0.0); // 中央部分または遠い場合は透明
   }
}
