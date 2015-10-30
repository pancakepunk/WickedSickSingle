
cbuffer DrawBuffer
{
  float4x4  modelToWorld;
  float4x4  worldToClip;
  float4    color;
  float3    camPos;
};

struct VertexInput
{
  float3 position : POSITION;
  float3 normal : NORMAL;
};

struct PixelInputType
{
  float4 position : SV_POSITION;
  float4 color : COLOR;
  float4 normal : NORMAL;
  float4 camPos : POSITION0;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInput input)
{
  PixelInputType output;
  float4 posWorldSpace = mul(float4(input.position, 1), modelToWorld);
  //posWorldSpace.y = 11.0;
  float4 posClipSpace = mul(posWorldSpace, worldToClip);
  output.position = posClipSpace;
  output.normal = mul(float4(input.normal, 0), modelToWorld);
  output.color = float4(1.0,0.0,0.0,1.0);//color;
  output.camPos = float4(camPos, 1);
  return output;
}