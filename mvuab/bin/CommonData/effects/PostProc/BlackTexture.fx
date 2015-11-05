#include "../samplers.fxh"
#include "../globals.fxh"

float4 BlackTexture(in float2 UV : TEXCOORD0) : COLOR
{
    return float4(0, 0, 0, 1);
}

technique BlackTextureTechnique
{
	pass p0
	{
	
        VertexShader = null;
		PixelShader = compile ps_3_0 BlackTexture();
	}
}
