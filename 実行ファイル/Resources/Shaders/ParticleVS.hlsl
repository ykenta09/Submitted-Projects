#include "Particle.hlsli"

// ���_�V�F�[�_�[�̃��C���֐�
PS_INPUT main(VS_INPUT input)
{
   PS_INPUT output = (PS_INPUT) 0; // �o�͍\���̂��������i�S�����o�[�� 0 �ɂ���j

    // ���_�ʒu�� w = 1 �������� float3 -> float4 �ɕϊ��i�������W�n�ւ̕ϊ��j
   output.Pos = float4(input.Pos, 1);

    // ���_�J���[�����̂܂܏o�͂ɓn���i��Ԃ���ăs�N�Z���V�F�[�_�ɓn��j
   output.Color = input.Color;

    // �e�N�X�`�����W�����̂܂܏o�́i������s�N�Z���V�F�[�_�Ŏg����j
   output.Tex = input.Tex;

   return output; // �s�N�Z���V�F�[�_�ɓn���f�[�^��Ԃ�
}
