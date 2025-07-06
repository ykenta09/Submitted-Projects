#include "ShadowMap.hlsli" 

// �V���h�E�}�b�v�e�N�X�`��
Texture2D ShadowMapTexture : register(t1);

// �V���h�E�}�b�v�p�e�N�X�`���T���v���[
sampler ShadowMapSampler : register(s1);

struct Input
{
   float2 TexCoord : TEXCOORD0;
   float4 PositionWS : TEXCOORD1;
   float3 NormalWS : TEXCOORD2;
   float4 LightPosPS : TEXCOORD3;
};

// �}�b�n�o���h�΍�
#define SHADOW_EPSILON 0.00005f

float4 main(Input pin) : SV_TARGET0
{
    // ���C�g�̌��������߂�
   float3 lightDir = normalize(pin.PositionWS.xyz - LightPos.xyz);

    // ���K���f�o�C�X���W�ɂ���
   pin.LightPosPS.xyz /= pin.LightPosPS.w;

    // �Q�Ƃ���V���h�E�}�b�v��UV�l�����߂�
   float2 uv = pin.LightPosPS.xy * float2(0.5f, -0.5f) + 0.5f;

    // �`�悷��s�N�Z�����e�����肷��
   float bright = (ShadowMapTexture.Sample(ShadowMapSampler, uv).x + SHADOW_EPSILON < pin.LightPosPS.z) ? 0.0f : 1.0f;

    // �f�B�t���[�Y�F�����߂�
   float4 diffuse = saturate(dot(-lightDir, normalize(pin.NormalWS)) * bright * (1.0f - LightAmbient) + LightAmbient) * DiffuseColor;

    // �e�N�X�`���F���|���� 
   float4 color = Texture.Sample(Sampler, pin.TexCoord) * diffuse;
    
   return color;
}

