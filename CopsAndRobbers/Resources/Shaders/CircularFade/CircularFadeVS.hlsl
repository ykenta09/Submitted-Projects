#include "CircularFade.hlsli" 

// ���_�V�F�[�_�[�̃G���g���|�C���g
PS_INPUT main(VS_INPUT input)
{
   PS_INPUT output = (PS_INPUT) 0; // �o�͍\���̂��[��������

    // ���_���W��float4�^�Ɋg�����ďo��
   output.Pos = float4(input.Pos, 1);

    // ���_�J���[�����̂܂܏o��
   output.Color = input.Color;

    // �e�N�X�`�����W�iUV�j�����̂܂܏o��
   output.Tex = input.Tex;

    // �s�N�Z���V�F�[�_�[�ɓn��
   return output;
}
