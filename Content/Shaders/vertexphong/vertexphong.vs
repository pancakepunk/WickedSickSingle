#include "../common/lightingHelpers.fx"

#include "../common/InputTypes/vertexLightingPSInput.fx"
#include "../common/InputTypes/vertexInput.fx"




////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInput input)
{
  PixelInputType output;
  
  float4 posWS = mul(float4(input.position.xyz, 1), modelToWorld);
  float4 posCS = mul(posWS, worldToClip);
  float3 fullView = cameraPos - posWS.xyz;
  float  viewLen = length(fullView);
  float3 view = fullView/viewLen;
  float4 normalWS = normalize(mul(float4(input.normal, 0), modelToWorld));
  float fogScalar = calcFog(fogNear, fogFar, viewLen);
  float4 globalAmbient = globalAmbientColor * globalAmbientIntensity;

  float4 emissive = emissiveColor * emissiveConstant;

  float4 finalColor = float4(0,0,0,1);
  finalColor += emissive;
  finalColor += sumDirLightsPhong(float4(1,1,1,1),normalWS.xyz, view);
  finalColor += sumPointLightsPhong(float4(1,1,1,1),normalWS.xyz, view, posWS.xyz);
  finalColor += sumSpotLightsPhong(float4(1,1,1,1),normalWS.xyz, view, posWS.xyz);
  finalColor += globalAmbient;

  output.position = posCS;
  output.normal = normalWS;
  output.color = (fogScalar * finalColor) + ((1.0-fogScalar)*fogColor);
  output.tex = input.tex;
  return output;
}