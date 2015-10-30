
cbuffer DrawBuffer
{
  float4x4  modelToClip;
  float4    color;
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
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInput input)
{
  PixelInputType output;
  output.position = mul(float4(input.position.xyz, 1), modelToClip);
  output.color = color;
  return output;
}