struct PointLight
{
  float3    attnConstants;
  float derp;
  float3    lightPos;
  float herp;
  float3    ambientIntensity;
  float fuckyou;
  float3    diffuseIntensity;
  float paddingistheworst;
  float3    specularIntensity;

  float    pad0;

};

cbuffer PointLightBuffer
{
  int numPointLights;
  int3  pad1;
  PointLight pointLights[8];
};