#include "CircularFade.hlsli"

// �o�͂��钸�_����萔�Ŏw��i�����4���l�p�`�̒��_�j
static const int vnum = 4;

// �l���̒��_�ʒu���`�����z��i�X�N���[�����W�n�ł̃I�t�Z�b�g�j
static const float4 offset_array[vnum] =
{
   float4(-1.0f, 1.0f, 0.0f, 0.0f), // ����
    float4(1.0f, 1.0f, 0.0f, 0.0f), // �E��
    float4(-1.0f, -1.0f, 0.0f, 0.0f), // ����
    float4(1.0f, -1.0f, 0.0f, 0.0f), // �E��
};

// �W�I���g���V�F�[�_�[�{��
// [maxvertexcount(vnum)] �����ōő�o�͒��_�����w��i�K�{�j[1][3]
// point PS_INPUT input[1] : ���͂�1���_�̃|�C���g�v���~�e�B�u
// inout TriangleStream<PS_INPUT> output : �o�̓X�g���[��
[maxvertexcount(vnum)]
void main(
    point PS_INPUT input[1],
    inout TriangleStream<PS_INPUT> output
)
{
    // 4���_�����[�v���Ďl�p�`�𐶐�
   for (int i = 0; i < vnum; i++)
   {
      PS_INPUT element;

        // �I�t�Z�b�g�z�񂩂�X�N���[�����W���擾
      float4 res = offset_array[i];

        // ���͒��_�̍��W�Ƀ��[���h�s��ŃI�t�Z�b�g�����Z
      element.Pos = input[0].Pos + mul(offset_array[i], matWorld);

        // �r���[�E�v���W�F�N�V�����s��ō��W�ϊ�
      element.Pos = mul(element.Pos, matView);
      element.Pos = mul(element.Pos, matProj);

        // ���_�J���[�����̂܂܃R�s�[
      element.Color = input[0].Color;

        // �e�N�X�`�����W�iUV�j���l�p�`�p�ɕϊ�
      element.Tex.x = offset_array[i].x + 1.0f;
      element.Tex.y = -offset_array[i].y + 1.0f;
      element.Tex /= 2.0f;

        // ���_���X�g���[���ɒǉ�
      output.Append(element);
   }
    // �X�g���b�v���I��
   output.RestartStrip();
}
