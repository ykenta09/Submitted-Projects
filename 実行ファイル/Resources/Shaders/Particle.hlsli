//�萔�o�b�t�@
cbuffer ConstBuffer	: register(b0)
{
	matrix matWorld;	//���[���h�s��
	matrix matView;		//�r���[�s��
	matrix matProj;		//�ˉe�s��
	float4 diffuse;		//�f�B�t���[�Y�J���[
    float4 time;		//�^�C��
};

//���_�V�F�[�_�[�ւ̓��͍\����
struct VS_INPUT
{
	float3 Pos : POSITION;	  //���_�̈ʒu
	float4 Color: COLOR;	  //���_�J���[
	float2 Tex : TEXCOORD;	  //�e�N�X�`�����W(uv)
};

//�s�N�Z���V�F�[�_�[�̓��͍\����
struct PS_INPUT	  
{
	float4 Pos : SV_POSITION;	 //�ԊҌ�̉�ʋ�ԍ��W
	float4 Color: COLOR;		 //��Ԃ��ꂽ���_�J���[
	float2 Tex : TEXCOORD;		 //	��Ԃ��ꂽ�e�N�X�`�����W(uv)
};