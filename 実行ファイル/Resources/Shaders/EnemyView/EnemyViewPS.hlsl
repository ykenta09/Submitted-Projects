#include "EnemyView.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{

   bool isBlocked = false;
    
   return input.Color;
}
