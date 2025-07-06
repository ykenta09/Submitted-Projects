/*
   深度値描画用頂点シェーダ
*/

#include "ShadowMap.hlsli" 


// メイン関数
PS_SM_Input main(VS_SM_Input input)
{
   PS_SM_Input output;
   
   //output.Position = mul(float4(input.Position, 1.0f), WorldViewProj);

   output.Position = mul(input.Position, WorldViewProj);
   
   //output.Position = input.Position;
        
   return output;
}

