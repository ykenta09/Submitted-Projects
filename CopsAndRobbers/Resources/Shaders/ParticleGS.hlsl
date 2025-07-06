#include "Particle.hlsli"

// ���_���i�|��1����4���_�j
static const int vnum = 4;

// �|���̊e���_�̃I�t�Z�b�g�B�p�[�e�B�N�����S����ɂ������[�J�����W�n
static const float4 offset_array[vnum] =
{
	float4(-1.0f,  1.0f, 0.0f, 0.0f),	//	����
	float4( 1.0f,  1.0f, 0.0f, 0.0f),	//	�E��
	float4(-1.0f, -1.0f, 0.0f, 0.0f),	//	����
	float4( 1.0f, -1.0f, 0.0f, 0.0f),	//	�E��

};

// �ő�o�͒��_�����w��i����4���_�j
[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],				 // �W�I���g���V�F�[�_�[�̓��́i1�̒��_���p�[�e�B�N���̒��S���j
	inout TriangleStream< PS_INPUT > output	 // �o�̓X�g���[���B�O�p�`�X�g���b�v�Ƃ��Ē��_���o��
)
{
   // �|���p��4���_�����[�v�Ő���
	for (int i = 0; i < vnum; i++)
	{
		PS_INPUT element;

		float4 res = offset_array[i];
		 // �p�[�e�B�N�����S�ʒu�ɃI�t�Z�b�g�������āA���_�̃��[���h��ԍ��W���v�Z
		element.Pos = input[0].Pos + mul(offset_array[i], matWorld);

	    //// �r���[�ϊ��E�v���W�F�N�V�����s��
		element.Pos = mul(element.Pos, matView);
		element.Pos = mul(element.Pos, matProj);

		 // �e�N�X�`�����W�� [-1,1] -> [0,1] �͈̔͂ɕϊ�
		element.Color = input[0].Color;
		element.Tex.x =  offset_array[i].x + 1.0f;
		element.Tex.y = -offset_array[i].y + 1.0f;
		element.Tex /= 2.0f;
		
		// ���_���o�̓X�g���[���ɒǉ�
		output.Append(element);
	}
    // �O�p�`�X�g���b�v����؂�
	output.RestartStrip();
}