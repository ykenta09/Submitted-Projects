#include "Fragment.hlsli"

// ���_�V�F�[�_�[�̃��C���֐�
PS_INPUT main(VS_INPUT input)
{
   // �o�͍\���̂�������
   PS_INPUT output = (PS_INPUT) 0;
   // �e�N�X�`�����W�A�J���[�A�e�N�X�`�����W�����̂܂܏o��
   output.Pos = float4(input.Pos, 1);
   output.Color = input.Color;
   output.Tex = input.Tex;
   
   return output;
}

