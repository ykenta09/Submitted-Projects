//�R���X�g�o�b�t�@
cbuffer ConstBuffer : register(b0)
{
   matrix matWorld;     //���[���h�s��
   matrix matView;      //�r���[�s��
   matrix matProj;      //�v���W�F�N�V�����s��
   float4 diffuse;      //�f�B�t���[�Y�J���[
};

//���_�V�F�[�_�[�ւ̓��͍\����
struct VS_INPUT
{
   float3 Pos : POSITION;  //���_�̈ʒu
   float4 Color : COLOR;   //���_�J���[
   float2 Tex : TEXCOORD;  //�e�N�X�`�����W(uv)
};

//�s�N�Z���V�F�[�_�[�̓��͍\����
struct PS_INPUT
{
   float4 Position : SV_POSITION;     //�ԊҌ�̉�ʋ�ԍ��W
   float4 Color : COLOR;              //��Ԃ��ꂽ���_�J���[
   float2 TexCoord : TEXCOORD0;       //��Ԃ��ꂽ�e�N�X�`�����W(uv)
};