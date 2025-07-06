#include "Fragment.hlsli"

Texture2D tex : register(t0);            // �e�N�X�`�����\�[�X1
Texture2D tex2 : register(t1);           // �e�N�X�`�����\�[�X2
SamplerState samLinear : register(s0);   // �T���v���[�X�e�[�g


float4 main(PS_INPUT input) : SV_TARGET
{
   //���͂��ꂽuv���擾
   float2 uv = input.Tex;

   // �v���C���[�ƓG�̋������擾
   float currentDistance = distance.x;    //���݂̋���
   float prevDistance = distance.y;       //�O��̋���

   // ������20.0�����ŁA�l�p�`�̋��E�����̏ꍇ�ɏ������s��
   if (currentDistance < 20.0 && (uv.x < 0.1 || uv.x > 0.9 || uv.y < 0.1 || uv.y > 0.9))
   {
      // �l�p�̓����ɍs���قǓ����ɂȂ�悤�ɃO���f�[�V������������
      float baseAlpha = 1.0 - (min(min(uv.x, 1.0 - uv.x), min(uv.y, 1.0 - uv.y)) / 0.1); // �O���f�[�V�����𑝂₷

      // �����ɉ����Ăǂ��ǂ�������ʂ�ω�������
      float pulseSlow = sin(time.x * 1.5);
      float pulseFast = sin(time.x * 3.0f);
      float pulse = lerp(pulseSlow, pulseFast, step(prevDistance, 15.0) * smoothstep(10.0, 5.0, currentDistance));
      // step(prevDistance, 15.0)�́A�O��̋�����15�ȏ�̏ꍇ��1.0��Ԃ�
      // smoothstep(10.0, 5.0, currentDistance)�́A������10����5�͈̔͂�0����1�ɕω�����
      
      // �p���X�̉e���𑝂₷
      float pulseMultiplier = clamp(pulse * 0.6, 0.1, 0.6); // �͈͂𐧌�
      // 0.4��ς���ƁA�p���X�̉e���������Ȃ�
      // 0.1��0.6�́A�p���X���ŏ��l�ƍő�l�����߂�
      
      // �O���f�[�V���������͌Œ�̃A���t�@�l���g�p���A�p���X�͂��̏�ɏ�Z
      return float4(0.8, 0.0, 0.0, baseAlpha * pulseMultiplier); // �ԐF���A���t�@�l�Œ���
   }
   else
   {
      return float4(0.0, 0.0, 0.0, 0.0); // ���������܂��͉����ꍇ�͓���
   }
}
