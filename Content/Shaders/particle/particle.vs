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
  float4 PrevPos;//runtime data
  float Rotation;
  float Lifetime;
  float timepassed;
  float temp;
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

  float4 posWS;// = mul(float4(input.position.xyz, 1), modelToWorld);
  //posWS += Position;
  posWS.w = 1;
  //float4x4 worldToClip = mul(worldToCamera, cameraToClip);
  float4x4 scalar = float4x4(modelToWorld[0][0], 0.0, 0.0, 0.0,
                             0.0, modelToWorld[1][1], 0.0, 0.0,
                             0.0, 0.0, modelToWorld[2][2], 0.0,
                             0.0, 0.0, 0.0, 1.0);
  float4x4 tempCam = transpose(worldToCamera);
  tempCam = mul(tempCam, scalar);

  tempCam[3][0] = modelToWorld[3][0];
  tempCam[3][1] = modelToWorld[3][1];
  tempCam[3][2] = modelToWorld[3][2];

  //tempCam[0][0] *= modelToWorld[0][0];
  //tempCam[1][1] *= modelToWorld[1][1];
  //tempCam[2][2] *= modelToWorld[2][2];

  tempCam[3][3] = 1.0f;
  tempCam[0][3] = 0.0;
  tempCam[1][3] = 0.0;
  tempCam[2][3] = 0.0;

  //float4x4 realworldspace = mul(modelToWorld, tempCam);
  posWS = mul(float4(input.position.xyz, 1), tempCam);
  //posWS.x = 0.0f;
  float4 posVS = mul(posWS, worldToCamera);
  //posVS.z = 10.0f;
  //posVS += Position;
  float4 posCS = mul(posVS, cameraToClip);

  output.position = posCS;

  output.color = Color;
  output.tex = input.tex;
  return output;
}