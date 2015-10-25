#include "../globals.fxh"
#include "../samplers.fxh"


float StartRange = 0.75;
float EndRange   = 1.00;
  
float4 DamagePS( in float2 UV : TEXCOORD0 ) : COLOR
{
	//1.0 - saturate((g_Life - StartRange)/( EndRange-StartRange));
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
