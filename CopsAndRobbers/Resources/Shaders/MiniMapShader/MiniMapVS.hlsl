#include "MiniMap.hlsli"

// ���_�V�F�[�_�[�̃��C���֐�
PS_INPUT main(VS_INPUT input)
{
   // �o�͍\���̂�������
   PS_INPUT output = (PS_INPUT) 0;
    
   // ���_�J���[�����̂܂܏o�͂ɓn���i��Ԃ���ăs�N�Z���V�F�[�_�ɓn��j
   output.Position = mul(float4(input.Pos, 1.0f), matWorld);
   output.Position = mul(output.Position, matView);
   output.Position = mul(output.Position, matProj);
   // �e�N�X�`�����W�����̂܂܏o��
   output.Color = input.Color;
   output.TexCoord = input.Tex;
    
   return output;
}
