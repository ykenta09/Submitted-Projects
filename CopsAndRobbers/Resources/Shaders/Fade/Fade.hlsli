// �萔�o�b�t�@�ib0�Ƀo�C���h�j: �s���萔�l��CPU������GPU�֓n��
cbuffer ConstBuffer : register(b0)
{
   matrix matWorld;      // ���[���h�s��
   matrix matView;       // �r���[�s��
   matrix matProj;       // �v���W�F�N�V�����s��
   float4 diffuse;       // �f�B�t���[�Y�J���[
   float4 time;          // ���Ԃ�A�j���[�V�������Ŏg��float4�萔
};

// ���_�V�F�[�_�[�ւ̓��͍\����
struct VS_INPUT
{
   float3 Pos : POSITION;     // ���_�̍��W�i���[���h��ԁj
   float4 Color : COLOR;      // ���_�J���[�iRGBA�j
   float2 Tex : TEXCOORD;     // �e�N�X�`�����W�iUV�j
};

// �s�N�Z���V�F�[�_�[�ւ̓��͍\���́i���_�V�F�[�_�[����n�����j
struct PS_INPUT
{
   float4 Pos : SV_POSITION;     // �N���b�v��Ԃ̒��_���W�i���X�^���C�U�p�A�K�{�j[1][2][3][4]
   float4 Color : COLOR;         // ��Ԃ��ꂽ���_�J���[�iRGBA�j
   float2 Tex : TEXCOORD;        // ��Ԃ��ꂽ�e�N�X�`�����W�iUV�j
};
