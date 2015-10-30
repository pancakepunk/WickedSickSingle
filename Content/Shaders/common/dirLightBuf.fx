struct DirLight
{
  float3    ambientIntensity;
  float3    diffuseIntensity;
  float3    specularIntensity;
  float3    lightDir;
};

cbuffer DirLightBuffer
{
  int   numDirLights;
  int3  pad2;
  DirLight dirLights[8];
};