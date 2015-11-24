#include "../common/lightingHelpers.fx"

#include "../common/InputTypes/vertexInput.fx"
struct PixelInputType
{
  float4x4 modelToWorld : MTW;
  float4 positionClip : SV_POSITION;
  float4 positionWorld : POSITION0;
  float4 positionModel : POSITION1;
  float3 normal : NORMAL;
  float3 tangent : TANGENT;
  float3 bitangent : BITANGENT;
  float2 tex : TEXCOORD;
};






////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInput input)
{
  PixelInputType output;
  float4 posWS = mul(float4(input.position.xyz, 1), modelToWorld);
  float4 posCS = mul(posWS, worldToClip);
  //float4 normalWS = normalize(mul(float4(input.normal, 0), modelToWorld));

  output.modelToWorld = modelToWorld;
  output.positionModel = float4(input.position.xyz, 1.0);
  output.positionWorld = posWS;
  output.positionClip = posCS;
  output.normal = normalize(mul(float4(input.normal, 0), modelToWorld).xyz);
  output.tangent = (mul(float4(input.tangent, 0), modelToWorld).xyz);
  output.bitangent = (mul(float4(input.bitangent, 0), modelToWorld).xyz);



  output.tex = input.tex;
  return output;
}