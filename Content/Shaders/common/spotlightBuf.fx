struct SpotLight
{
  float3    attnConstants;
  float3    lightPos;

  float3    ambientIntensity;
  float3    diffuseIntensity;
  float3    specularIntensity;

  float3    lightDir;

  float     phi;
  float     theta;
  float     spotFalloff;
  float3    pad0;
};

cbuffer SpotLightBuffer
{
  int numSpotLights;
  int3  pad0;
  SpotLight spotLights[8];
};