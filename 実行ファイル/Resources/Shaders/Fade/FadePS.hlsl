#include "Fade.hlsli" 

// �e�N�X�`���it0, t1�Ƀo�C���h�j
Texture2D tex : register(t0); // ���C���̃e�N�X�`��
Texture2D tex2 : register(t1); // �T�u�̃e�N�X�`���i���g�p�j
SamplerState samLinear : register(s0); // ���`��ԃT���v���[


// �A���t�@�l�i�����x�j���v�Z����֐�
float CalcAlpha(float baseAlpha, float2 uv)
{
   float ret = baseAlpha; // ��{�̃A���t�@�l�ŏ�����

   // time.y > 0.5 �Ȃ�t�F�[�h�C����
   if (time.y > 0.5) // �t�F�[�h�C���̏ꍇ
   {
      ret = 1.0f - time.x; // time.x��0��1�œ������s����
   }
   // time.z > 0.5 �Ȃ�t�F�[�h�A�E�g��
   else if (time.z > 0.5) // �t�F�[�h�A�E�g�̏ꍇ
   {
      ret = time.x; // time.x��0��1�ŕs����������
   }

   return ret; // �v�Z�����A���t�@�l��Ԃ�
}

// �s�N�Z���V�F�[�_�[�̃G���g���|�C���g
float4 main(PS_INPUT input) : SV_TARGET
{
   // �o�͐F��^�����ŏ������iRGB=0, Alpha=0�j
   float4 outputb = float4(0, 0, 0, 0);

   // �e�N�X�`�����W���擾
   float2 uv = input.Tex;
      
   // �t�F�[�h��Ԃɉ����ăA���t�@�l���v�Z
   outputb.a = CalcAlpha(outputb.a, uv);

   // �s�N�Z���J���[��Ԃ�
   return outputb;
}
