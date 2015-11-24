#include "../common/lightingHelpers.fx"

#include "../common/InputTypes/vertexInput.fx"
struct PixelInputType
{
  float4 positionClip : SV_POSITION;
  float4 positionWorld : POSITION0;
  float4 positionModel : POSITION1;
  float4 normal : NORMAL;
  float4 color : COLOR;
  float2 tex : TEXCOORD0;
};






////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInput input)
{
  PixelInputType output;

  float4 posWS = mul(float4(input.position.xyz, 1), modelToWorld);
  float4 posCS = mul(posWS, worldToClip);
  float4 normalWS = normalize(mul(float4(input.normal, 0), modelToWorld));

  output.positionModel = float4(input.position.xyz, 1);
  output.positionWorld = posWS;
  output.positionClip = posCS;
  output.normal = normalWS;
  output.color = float4(0,0,0,1);
  output.tex = input.tex;
  return output;
}