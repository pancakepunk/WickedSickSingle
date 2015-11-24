#include "../common/lightingHelpers.fx"

#include "../common/InputTypes/vertexInput.fx"
#include "../common/InputTypes/pixellightingpsinput.fx"





////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInput input)
{
  PixelInputType output;

  float4 posWS = mul(float4(input.position.xyz, 1), modelToWorld);
  float4 posCS = mul(posWS, worldToClip);
  float4 normalWS = normalize(mul(float4(input.normal + (input.tangent * 0.00001), 0), modelToWorld));

  
  output.positionWorld = posWS;
  output.positionClip = posCS;
  output.normal = normalWS;
  output.color = float4(0,0,0,1);
  output.tex = input.tex;
  return output;
}