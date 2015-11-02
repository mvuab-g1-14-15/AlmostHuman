#include "../globals.fxh"
#include "../samplers.fxh"


float DeadRange = 30;
float4 DamagePS( in float2 UV : TEXCOORD0 ) : COLOR
{   
	float4 lFullLife = tex2D(S0LinearSampler, UV);
	float lColorDead = ( lFullLife.r + lFullLife.g + lFullLife.b ) / 3.0;
    float pFullLife = saturate( smoothstep( 0, DeadRange, g_Life ) );
    float4 lColorBandW = ( 1- pFullLife ) * lColorDead +  pFullLife * lFullLife;
	return lColorBandW;
}

technique TECHNIQUE_NAME
{
	pass p0
	{
		PixelShader = compile ps_3_0 DamagePS();
	}
}
