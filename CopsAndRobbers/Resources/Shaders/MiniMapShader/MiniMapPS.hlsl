#include "MiniMap.hlsli"

Texture2D texQuad : register(t0);      // �e�N�X�`�����\�[�X
SamplerState samLinear : register(s0); // �T���v���[�X�e�[�g

float4 main(PS_INPUT input) : SV_TARGET
{
   //���͂��ꂽuv���擾
   float2 uv = input.TexCoord;
   
   //uv�ʒu�̃e�N�X�`���̐F���擾����
   float4 quadColor = texQuad.Sample(samLinear, uv);
   
    // �~�`�e�N�X�`�����`�悳��Ă��镔���̃A���t�@�l�� 0 �ɂ���
   if (quadColor.a > 0.0f)
   {
      return float4(0.0f, 0.0f, 0.0f, 0.0f); // �����ŕ\���i�f�o�b�O�p�j
   }

   //�A���t�@�l������ꏊ�̓O���[��\��
   return float4(0.4f, 0.4f, 0.4f, 0.98f);;
}
