
// ���[���h�E�r���[�E�v���W�F�N�V�����s����i�[����萔�o�b�t�@�ib0�Ƀo�C���h�j
cbuffer MatrixBuffer : register(b0)
{
   matrix World;       // ���[���h�s��i���f���̈ʒu�E��]�E�X�P�[���ϊ��j
   matrix View;        // �r���[�s��i�J�����̎��_�ϊ��j
   matrix Projection;  // �v���W�F�N�V�����s��i�ˉe�ϊ��j
};

// �t�H�O�p�p�����[�^���i�[����萔�o�b�t�@�ib1�Ƀo�C���h�j
cbuffer FogBuffer : register(b1)
{
   float4 FogColor;          // �t�H�O�̐F�iRGBA�j
   float  FogStart;          // �t�H�O�̊J�n����
   float  FogEnd;            // �t�H�O���ő�ɂȂ鋗��
   float2 padding;           // �p�f�B���O�i16�o�C�g���E���킹�p�A���g�p�j
   float3 CameraPosition;    // �J�����̃��[���h���W
   float  padding2;          // �p�f�B���O�i16�o�C�g���E���킹�p�A���g�p�j
};

// ���f���e�N�X�`����
cbuffer UVScaleBuffer : register(b2)
{
   float2 UVScale;
   float2 padding3;
}

//���C�g�̒萔�o�b�t�@(�f�B���N�V���i�����C�g)
cbuffer LightBuffer : register(b3)
{
   float3 lightDirection;           //���C�g�̕���(���[���h��ԁA���K�ς݁j
   float padding4;
   float4 lightDiffuseColor;        //���C�g�̃f�B�t���[�Y
   float4 ambientColor;             //�A���r�G���g�F
};

#define MAX_POINT_LIGHTS 5

//�|�C���g���C�g�̍\���̂ƃo�b�t�@
struct PointLight
{
   float3 Position;        //���C�g�̍��W
   float range;            //�L��
   float3 color;           //���C�g�̐F
   float intensity;        //���C�g�̋���
};
cbuffer PointLightBuffer : register(b4)
{
   PointLight pointLights[MAX_POINT_LIGHTS];
   int numLights;
   float3 padding5;
}

// ���_�V�F�[�_�[�ւ̓��͍\���́i���_�o�b�t�@1���_���̃f�[�^���󂯎��j
struct VS_INPUT
{
   float3 Position   : POSITION; // ���_�̍��W�i���[���h��ԁj
   float3 Normal     : NORMAL;     // �@���x�N�g��
   float4 Color      : COLOR; // ���_�J���[�iRGBA�j
   float2 TexCoord   : TEXCOORD0; // �e�N�X�`�����W�iUV�j
};

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̏o�͍\����
struct PS_INPUT
{
   float4 Position : SV_POSITION; // �N���b�v��Ԃ̒��_���W�i���X�^���C�U�p�j
   float2 TexCoord : TEXCOORD0; // �e�N�X�`�����W�iUV�A�s�N�Z���V�F�[�_�[�ɓn���j
   float3 WorldPos : TEXCOORD1; // ���[���h��Ԃ̒��_���W�i�t�H�O�⃉�C�e�B���O�p�j
   float3 WorldNormal : TEXCOORD2; // ���[���h��Ԗ@���i�ǉ��j
};