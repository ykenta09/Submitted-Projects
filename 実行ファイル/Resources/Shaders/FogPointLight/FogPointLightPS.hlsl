#include "FogPointLight.hlsli" 

// メッシュのテクスチャ（t0にバインド）
Texture2D g_MeshTexture : register(t0);
// テクスチャサンプラー（s0にバインド）
SamplerState MeshTextureSampler : register(s0);

//キューブマップ
TextureCube<float> g_PointLightShadowCubeMap : register(t1);
//比較サンプラー
SamplerComparisonState g_ShadowSampler : register(s1);
// 通常のサンプラー（比較なし）
SamplerState DebugSampler : register(s2);


//ポイントライトの遮蔽判定関数
float PointLightShadow(PointLight light, float3 worldPos)
{   
   float3 toLight = worldPos - light.Position;
   float currentDepth = length(toLight) / light.range;
       
   // バイアスも適宜調整
   return g_PointLightShadowCubeMap.SampleCmpLevelZero(
        g_ShadowSampler,
        normalize(toLight), //キューブマップの方向ベクトル
        currentDepth - 0.001f //シャドウアクネ対策
    );
}

//ディレクショナルライトの計算
float3 CalcDirectionalLight(float3 normal, float3 texColor)
{
   //ライトの方向を取得
   float3 lightDir = normalize(lightDirection);
   //ディフューズライティング
   float NdotL = saturate(dot(normal, -lightDir)); // ライトは通常「逆方向」を指定するため
   float3 diffuse = NdotL * lightDiffuseColor.rgb;
   //アンビエント
   float3 ambient = ambientColor.rgb;
   
   // 初期ライティング結果   
   return texColor.rgb * (diffuse + ambient);
}

//ポイントライトの合計計算
float3 CalcPointLights(float3 worldPos, float3 normal)
{
   float3 sum = float3(0, 0, 0);
   for (int i = 0; i < numLights; i++)
   {
      float3 toLight = pointLights[i].Position - worldPos;
      float distance = length(toLight);
      
      //光の減衰
      float distanceNormalized = distance / pointLights[i].range;
      float attenuation = 1.0f / (1.0f + 25.0f * distanceNormalized * distanceNormalized);
      
      float3 lightDir = normalize(toLight);
      float NdotL = saturate(dot(normal, lightDir));
      
      //シャドウ判定を使う場合はここで呼ぶ
      float shadow = PointLightShadow(pointLights[i], worldPos);
      
      sum += pointLights[i].color * pointLights[i].intensity * attenuation * NdotL /** shadow*/;
   }
   return sum;
}

//フォグファクターの計算
float CalcFogFactor(float3 worldPos)
{
   // density（例: 0.05～0.2）をパラメータとして定義
   float density = 0.07f;
   //カメラとの距離を計算
   float distanceToCamera = length(worldPos - CameraPosition);
   // フォグ計算
   float fogFactor = 1.0 - exp(-pow(distanceToCamera * density, 2));
   return saturate(fogFactor);
}

// ピクセルシェーダーのエントリポイント
float4 main(PS_INPUT input) : SV_TARGET
{
   //テクスチャサイズの調整
   float2 uv = input.TexCoord * UVScale;
   // テクスチャから色をサンプリング
   float4 texColor = g_MeshTexture.Sample(MeshTextureSampler, uv);

   //法線を正規化
   float3 normal = normalize(input.WorldNormal);

   //ディレクショナルライト
   float3 dirLight = CalcDirectionalLight(normal, texColor.rgb);
   
    // 初期ライティング結果
   float3 litColor = dirLight;
   
   //ポイントライト
   float3 pointLight = CalcPointLights(input.WorldPos, normal);
   
   //ライティングの合成
   litColor += texColor.rgb * pointLight;

   //フォグ
   float fogFactor = CalcFogFactor(input.WorldPos);
   // テクスチャ色とフォグ色をfogFactorで線形補間
   float3 finalColor = lerp(litColor - texColor.rgb * pointLight, FogColor.rgb, fogFactor);
   finalColor += texColor.rgb * pointLight;

   // フォグ合成（ポイントライト以外の色にのみフォグを適用）
   return float4(finalColor, 1.0f);

   
   //// --- 以下はデバッグ用の返り値例 ---    
   ////キューブマップを可視化-----------------------------------------------------------------------------
   ////return float4(shadow, shadow, shadow, 1.0);
   
   ////シャドウマップの値を可視化-------------------------------------------------------------------------
   //float3 toLight = input.WorldPos - pointLights[1].Position;
   //float depth = length(toLight) / pointLights[1].range;
   //float shadowValue = g_PointLightShadowCubeMap.SampleLevel(DebugSampler, normalize(toLight), 0).r;
   //return float4(shadowValue, shadowValue, shadowValue, 1.0);
   //法線を可視化----------------------------------------------------------------------------------------
   return float4(normal * 0.5 + 0.5, 1.0);
   
   //通常のテクスチャ描画-------------------------------------------------------------------------------
   return g_MeshTexture.Sample(MeshTextureSampler, uv);
}

