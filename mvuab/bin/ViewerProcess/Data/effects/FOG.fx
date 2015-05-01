// http://www.iquilezles.org/www/articles/fog/fog.htm
// http://www.terathon.com/lengyel/Lengyel-UnifiedFog.pdf

#include "samplers.fxh"
#include "globals.fxh"

float4 FogPS(in float2 UV : TEXCOORD0) : COLOR
{
	float4 l_Diffuse  = tex2D(S0LinearSampler, UV);
    float4 l_FogColor = float4(0.5f, 0.6f, 0.7f, 1.0f);
    float4 Depth = tex2D(S1PointSampler, UV); //Depth (float4)
	
	float3 l_Position = GetPositionFromZDepthView( Depth.x, UV, g_ViewInverseMatrix, g_ProjectionInverseMatrix );
    float l_DistanceFromCameraToPixel = length(l_Position - g_CameraPosition);
	
	if(g_FogFun == 1) return CalcExpFog(l_DistanceFromCameraToPixel, g_FodExp, l_FogColor, l_Diffuse);
	else if(g_FogFun == 2) return CalcLinearFog(l_DistanceFromCameraToPixel, g_FogStar, g_FogEnd, l_FogColor, l_Diffuse);
	else return CalcGroundFog(l_FogColor, l_Diffuse, l_DistanceFromCameraToPixel, g_CameraPosition, normalize(l_Position - g_CameraPosition));
}

technique FogTechnique
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
