#include "../globals.fxh"
#include "../samplers.fxh"
  
float4 DamagePS( in float2 UV : TEXCOORD0 ) : COLOR
{
	return tex2D(S0LinearSampler, UV);
	//float4 lColor = tex2D(S0LinearSampler, UV);
	//float lColorBandW = ( lColor.r + lColor.g + lColor.b ) / 3.0;
	//return float4( 0.4, lColorBandW, lColorBandW, 1 ) ;
}

technique TECHNIQUE_NAME
{
	pass p0
	{
		PixelShader = compile ps_3_0 DamagePS();
	}
}
