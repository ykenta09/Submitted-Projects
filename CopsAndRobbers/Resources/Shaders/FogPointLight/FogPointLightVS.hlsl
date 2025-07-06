#include "FogPointLight.hlsli" 

// ���_�V�F�[�_�[�̃G���g���|�C���g
PS_INPUT main(VS_INPUT input)
{
   PS_INPUT output = (PS_INPUT) 0; // �o�͍\���̂��������i�S�����o�[�� 0 �ɂ���j

   // ���_���W�����[���h��Ԃɕϊ��ifloat3��float4�Ɋg������World�s����|����j
   float4 worldPos = mul(float4(input.Position.xyz, 1.0f), World);

   // ���[���h��Ԃ̒��_���W���r���[�E�v���W�F�N�V�����s��ŕϊ����A�N���b�v��ԍ��W�ɂ���
   output.Position = mul(worldPos, mul(View, Projection));

   // ���[���h��Ԃ̒��_���W�ixyz�����̂݁j���o��
   output.WorldPos = worldPos.xyz;
   
   // �e�N�X�`�����W�iUV�j�����̂܂܏o��
   output.TexCoord = input.TexCoord;

   // �@�������[���h��Ԃɕϊ��i���[���h�s��̋t�]�u�s����g���̂���ʓI�j
    // �ȈՔłƂ��ă��[���h�s��̉�]�����݂̂��g���ꍇ�͉��L�̂悤��
   float3 worldNormal = mul(input.Normal, (float3x3) World);
   output.WorldNormal = normalize(worldNormal); // ���K��
   
   // ���_�V�F�[�_�[�̏o�͂�Ԃ�
   return output;
}
