#include "samplers.fxh"
#include "globals.fxh"

float4 FogPS(in float2 UV : TEXCOORD0) : COLOR
{	
	return float4(1,0,0,0.5);
}

technique FOGTechnique
{
	pass p0
	{
		AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		
		PixelShader = compile ps_3_0 FogPS();
	}
}