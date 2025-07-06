#include "FogPointLight.hlsli" 

// ���b�V���̃e�N�X�`���it0�Ƀo�C���h�j
Texture2D g_MeshTexture : register(t0);
// �e�N�X�`���T���v���[�is0�Ƀo�C���h�j
SamplerState MeshTextureSampler : register(s0);

//�L���[�u�}�b�v
TextureCube<float> g_PointLightShadowCubeMap : register(t1);
//��r�T���v���[
SamplerComparisonState g_ShadowSampler : register(s1);
// �ʏ�̃T���v���[�i��r�Ȃ��j
SamplerState DebugSampler : register(s2);


//�|�C���g���C�g�̎Օ�����֐�
float PointLightShadow(PointLight light, float3 worldPos)
{   
   float3 toLight = worldPos - light.Position;
   float currentDepth = length(toLight) / light.range;
       
   // �o�C�A�X���K�X����
   return g_PointLightShadowCubeMap.SampleCmpLevelZero(
        g_ShadowSampler,
        normalize(toLight), //�L���[�u�}�b�v�̕����x�N�g��
        currentDepth - 0.001f //�V���h�E�A�N�l�΍�
    );
}

//�f�B���N�V���i�����C�g�̌v�Z
float3 CalcDirectionalLight(float3 normal, float3 texColor)
{
   //���C�g�̕������擾
   float3 lightDir = normalize(lightDirection);
   //�f�B�t���[�Y���C�e�B���O
   float NdotL = saturate(dot(normal, -lightDir)); // ���C�g�͒ʏ�u�t�����v���w�肷�邽��
   float3 diffuse = NdotL * lightDiffuseColor.rgb;
   //�A���r�G���g
   float3 ambient = ambientColor.rgb;
   
   // �������C�e�B���O����   
   return texColor.rgb * (diffuse + ambient);
}

//�|�C���g���C�g�̍��v�v�Z
float3 CalcPointLights(float3 worldPos, float3 normal)
{
   float3 sum = float3(0, 0, 0);
   for (int i = 0; i < numLights; i++)
   {
      float3 toLight = pointLights[i].Position - worldPos;
      float distance = length(toLight);
      
      //���̌���
      float distanceNormalized = distance / pointLights[i].range;
      float attenuation = 1.0f / (1.0f + 25.0f * distanceNormalized * distanceNormalized);
      
      float3 lightDir = normalize(toLight);
      float NdotL = saturate(dot(normal, lightDir));
      
      //�V���h�E������g���ꍇ�͂����ŌĂ�
      float shadow = PointLightShadow(pointLights[i], worldPos);
      
      sum += pointLights[i].color * pointLights[i].intensity * attenuation * NdotL /** shadow*/;
   }
   return sum;
}

//�t�H�O�t�@�N�^�[�̌v�Z
float CalcFogFactor(float3 worldPos)
{
   // density�i��: 0.05�`0.2�j���p�����[�^�Ƃ��Ē�`
   float density = 0.07f;
   //�J�����Ƃ̋������v�Z
   float distanceToCamera = length(worldPos - CameraPosition);
   // �t�H�O�v�Z
   float fogFactor = 1.0 - exp(-pow(distanceToCamera * density, 2));
   return saturate(fogFactor);
}

// �s�N�Z���V�F�[�_�[�̃G���g���|�C���g
float4 main(PS_INPUT input) : SV_TARGET
{
   //�e�N�X�`���T�C�Y�̒���
   float2 uv = input.TexCoord * UVScale;
   // �e�N�X�`������F���T���v�����O
   float4 texColor = g_MeshTexture.Sample(MeshTextureSampler, uv);

   //�@���𐳋K��
   float3 normal = normalize(input.WorldNormal);

   //�f�B���N�V���i�����C�g
   float3 dirLight = CalcDirectionalLight(normal, texColor.rgb);
   
    // �������C�e�B���O����
   float3 litColor = dirLight;
   
   //�|�C���g���C�g
   float3 pointLight = CalcPointLights(input.WorldPos, normal);
   
   //���C�e�B���O�̍���
   litColor += texColor.rgb * pointLight;

   //�t�H�O
   float fogFactor = CalcFogFactor(input.WorldPos);
   // �e�N�X�`���F�ƃt�H�O�F��fogFactor�Ő��`���
   float3 finalColor = lerp(litColor - texColor.rgb * pointLight, FogColor.rgb, fogFactor);
   finalColor += texColor.rgb * pointLight;

   // �t�H�O�����i�|�C���g���C�g�ȊO�̐F�ɂ̂݃t�H�O��K�p�j
   return float4(finalColor, 1.0f);

   
   //// --- �ȉ��̓f�o�b�O�p�̕Ԃ�l�� ---    
   ////�L���[�u�}�b�v������-----------------------------------------------------------------------------
   ////return float4(shadow, shadow, shadow, 1.0);
   
   ////�V���h�E�}�b�v�̒l������-------------------------------------------------------------------------
   //float3 toLight = input.WorldPos - pointLights[1].Position;
   //float depth = length(toLight) / pointLights[1].range;
   //float shadowValue = g_PointLightShadowCubeMap.SampleLevel(DebugSampler, normalize(toLight), 0).r;
   //return float4(shadowValue, shadowValue, shadowValue, 1.0);
   //�@��������----------------------------------------------------------------------------------------
   return float4(normal * 0.5 + 0.5, 1.0);
   
   //�ʏ�̃e�N�X�`���`��-------------------------------------------------------------------------------
   return g_MeshTexture.Sample(MeshTextureSampler, uv);
}

