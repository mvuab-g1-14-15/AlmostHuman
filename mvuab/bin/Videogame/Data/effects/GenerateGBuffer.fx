#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

struct TMultiRenderTargetPixel
{
	float4 Albedo : COLOR0; //Albedo (float3) + (float) SpecularFactor
	float4 Ambient : COLOR1; //AmbientLight (float3) + (float) SpecularPow
	float4 Normal : COLOR2; //Normal (float3) + (float) Not used
	float4 Depth : COLOR3; //Depth (float4)
};

float3 Normal2Texture(float3 Normal)
{
	return Normal*0.5+0.5;
}
float3 Texture2Normal(float3 Color)
{
	return (Color-0.5)*2;
}

/*float3 GetPositionFromZDepthView(float ZDepthView, float2 UV, float4x4 InverseViewMatrix, float4x4 InverseProjectionMatrix)
{
	float3 l_PositionView=GetPositionFromZDepthViewInViewCoordinates(ZDepthView, UV,
	InverseProjectionMatrix);
	return mul(float4(l_PositionView,1.0), InverseViewMatrix).xyz;
}

float3 GetPositionFromZDepthViewInViewCoordinates(float ZDepthView, float2 UV, float4x4 InverseProjectionMatrix)
{
	// Get the depth value for this pixel
	// Get x/w and y/w from the viewport position
	float x = UV.x * 2 - 1;
	float y = (1 - UV.y) * 2 - 1;
	float4 l_ProjectedPos = float4(x, y, ZDepthView, 1.0);
	// Transform by the inverse projection matrix
	float4 l_PositionVS = mul(l_ProjectedPos, InverseProjection atrix);
	// Divide by w to get the view-space position
	return l_PositionVS.xyz / l_PositionVS.w;
}*/

struct VertexPS
{
	float4 HPosition : POSITION;
	float2 Pos : TEXCOORD0;
    float3 Normal : TEXCOORD1;
	float2 UV : TEXCOORD2;
};

VertexPS mainVS(TNORMAL_T1_VERTEX IN)
{
	VertexPS OUT=(VertexPS)0;
	OUT.HPosition=mul(float4(IN.Position, 1.0), g_WorldViewProj);
	OUT.Pos.xy=OUT.HPosition.zw;
	OUT.Normal = mul(IN.Normal, (float3x3)g_WorldMatrix);
	OUT.UV=IN.UV;
	return OUT;
}

TMultiRenderTargetPixel mainPS(VertexPS IN) : COLOR
{
	TMultiRenderTargetPixel OUT = (TMultiRenderTargetPixel)0;
	
	float4 l_Albedo = float4( (float3)tex2D(S0LinearSampler, IN.UV), 1.0f);
	float4 l_Ambient = float4( (float3)g_AmbientLight, 1.0f);
	float4 l_Normal = float4(Normal2Texture(normalize(IN.Normal)), 0.0f);
	float4 l_Depth=IN.Pos.x/IN.Pos.y;
	
	OUT.Albedo = l_Albedo;
	OUT.Ambient = l_Ambient;
	OUT.Normal = l_Normal;
	OUT.Depth = l_Depth;
	
	return OUT;
}

float4 RenderLightDeferredShadingPS(TMultiRenderTargetPixel IN) : COLOR
{
	return float4(1.0, 0.0, 0.0, 1.0);
}

technique GenerateGBufferTechnique
{
	pass p0
	{
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
