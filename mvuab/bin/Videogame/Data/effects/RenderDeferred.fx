#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

//Albedo (float3) + (float) SpecularFactor => S0XXXSampler
//AmbientLight (float3) + (float) SpecularPow => S1XXXSampler
//Normal (float3) + (float) Not used => S2XXXSampler
//Depth (float4) => S3XXXSampler

float4 mainPS(in float2 UV : TEXCOORD0) : COLOR
{
	float4 Albedo 		= tex2D(S0PointSampler, UV); //Albedo (float3) + (float) SpecularFactor
	float4 Ambient 		= tex2D(S1PointSampler, UV); //AmbientLight (float3) + (float) SpecularPow
	float4 NormalColor 	= tex2D(S2PointSampler, UV); //Normal (float3) + (float) Not used
	float4 Depth 		= tex2D(S3PointSampler, UV); //Depth (float4)
	
	float4 	l_DiffuseColor 		= Ambient;
	float 	l_SpecularFactor 	= Albedo.a;
	float4 	l_NormalColor 		= tex2D(S2PointSampler, UV);
	float	l_Depth 			= tex2D(S3LinearSampler, UV).x;
	float3 l_WorldPosition 		= GetPositionFromZDepthView(l_Depth, UV, g_ViewInverseMatrix, g_ProjectionInverseMatrix);
	float3 l_WorldNormal 		= Texture2Normal(l_NormalColor.xyz);
	
	float4 l_PixelColor = float4(0.0, 0.0, 0.0, 1.0);
	
	if(g_LightsEnabled[0] == 1) 
	{
		float3 l_Normal 			= normalize(l_WorldNormal);
		float3 l_LightDirection 	= normalize(l_WorldPosition-g_LightsPosition[0]);
		float l_Attenuation 		= DistanceAttenuation(0, l_LightDirection );
		if(DIR_LIGHT == g_LightsType[0])
		{
			l_LightDirection = normalize(g_LightsDirection[0]);
		}
		else if(SPOT_LIGHT == g_LightsType[0])
		{	
			l_LightDirection = normalize(g_LightsDirection[0]);
			l_Attenuation = l_Attenuation * SpotAttenuation(0, l_LightDirection);
		}
		
		float3 l_Hn=normalize(normalize(g_CameraPosition-l_WorldPosition)-l_LightDirection);
		float3 l_DiffuseContrib = l_DiffuseColor*saturate(dot(l_Normal,-l_LightDirection)) * l_Attenuation * g_LightsColor[0];
		float3 l_SpecularContrib = pow(saturate(dot(l_Normal,l_Hn)),g_SpecularExponent) * l_Attenuation * g_LightsColor[0] * l_SpecularFactor;
		l_PixelColor = l_PixelColor + float4( l_DiffuseContrib + l_SpecularContrib, 1.0);
	}
	
	/*float3 vPos = mul( float4(l_WorldPosition.xyz,1.0), g_ViewMatrix ).xyz;
	float4 PosLight = mul( float4(vPos, 1.0), g_ViewToLightProjMatrix );
	
	float2 ShadowTexC = 0.5 * PosLight.xy / PosLight.w + float2( 0.5, 0.5 );
	ShadowTexC.y = 1.0f - ShadowTexC.y;
	
	float LightAmount = (tex2D( ShadowMapTextureSampler, ShadowTexC ) + SHADOW_EPSILON < PosLight.z / PosLight.w)? 0.0f: 1.0f;
	// 1 – Píxel iluminado
	// 0 – Píxel en sombra
	
	return l_PixelColor * LightAmount;
	*/
	return l_PixelColor;
}

technique RenderDeferredTechnique {
	pass p0 {
		
		AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend = one;
		DestBlend = one;
		
		VertexShader = null;
		PixelShader = compile ps_3_0 mainPS();
	}
}
