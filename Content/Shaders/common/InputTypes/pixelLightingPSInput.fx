struct PixelInputType
{
  float4 positionClip : SV_POSITION;
  float4 positionWorld : POSITION0;
  float4 normal : NORMAL;
  float4 color : COLOR;
  float2 tex : TEXCOORD0;
};