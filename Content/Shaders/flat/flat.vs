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
  float4 normalWS = normalize(mul(float4(input.normal, 0), modelToWorld));

  
  output.positionWorld = posWS;
  output.positionClip = posCS;
  output.normal = normalWS;
  output.color = float4(1,1,1,1);
  output.tex = float2(0,0);
  return output;
}