/*
   深度値描画用ピクセルシェーダ
*/
#include "ShadowMap.hlsli" 

// メイン関数
float4 main(PS_SM_Input input) : SV_Target0
{
   return input.Position.z;
   
   return float4(input.Position.z, 0, 0, 1);
}