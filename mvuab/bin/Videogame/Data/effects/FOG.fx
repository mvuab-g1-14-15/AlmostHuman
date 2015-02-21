#include "samplers.fxh"
#include "globals.fxh"

float4 FogPS(in float2 UV : TEXCOORD0) : COLOR
{
	float4 l_Diffuse  = tex2D(S0LinearSampler, UV);
    float4 l_FogColor = float4(0.7f, 0.7f, 0.7f, 1.0f);
    
    float d = length(GetPositionFromZDepthView(tex2D(S1LinearSampler, UV).x, UV) - g_CameraPosition);
    //return CalcLinearFog(d, 30, 100, l_FogColor, l_Diffuse);
    return CalcExpFog(d, 0.02, l_FogColor, l_Diffuse);
}

technique FOGTechnique
{
	pass p0
	{
		AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		
        VertexShader = null;
		PixelShader = compile ps_3_0 FogPS();
	}
}
