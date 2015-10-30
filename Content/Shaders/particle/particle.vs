#include "../common/InputTypes/vertexInput.fx"
cbuffer MatrixBuf
{
  float4x4 modelToWorld;
  float4x4 worldToCamera;
  float4x4 cameraToClip;
};

cbuffer Particle
{
  float4 Position;
  float4 Velocity;
  float4 Color;
  
  float4 Scale;
  float Rotation;
  float Lifetime;

  float4 PrevPos;//runtime data
};



struct PixelInputType
{
  float4 color : COLOR;
  float4 position : SV_POSITION;
  float2 tex : TEXCOORD;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInput input)
{
  PixelInputType output;

  float4 posWS = mul(float4(input.position.xyz, 1), modelToWorld);
  //posWS += Position;
  posWS.w = 1;
  //float4x4 worldToClip = mul(worldToCamera, cameraToClip);
  float4 posVS = mul(posWS, worldToCamera);
  //posVS += Position;
  float4 posCS = mul(posVS, cameraToClip);

  output.position = posCS;

  output.color = Color;
  output.tex = input.tex;
  return output;
}