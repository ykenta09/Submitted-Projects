/*
   �[�x�l�`��p�s�N�Z���V�F�[�_
*/
#include "ShadowMap.hlsli" 

// ���C���֐�
float4 main(PS_SM_Input input) : SV_Target0
{
   return input.Position.z;
   
   return float4(input.Position.z, 0, 0, 1);
}