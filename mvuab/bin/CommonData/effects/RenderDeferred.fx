#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

float4 mainPS(in float2 UV : TEXCOORD0) : COLOR
{
	float4 Albedo 		= tex2D(S0PointSampler, UV); //Albedo (float3) + (float) SpecularFactor
	float4 Ambient 		= tex2D(S1PointSampler, UV); //AmbientLight (float3) + (float) SpecularPow
	float4 NormalColor 	= tex2D(S2PointSampler, UV); //Normal (float3) + (float) SelfIlumAmount
	float4 Depth 		= tex2D(S3PointSampler, UV); //Depth (float4)
	
	float4 	l_DiffuseColor 		= Albedo;
	float 	l_SpecularFactor 	= Albedo.a;
	float3 	l_NormalColor 		= NormalColor.xyz;
	float	l_SelfIlumAcum      = NormalColor.w;
	float3  l_WorldPosition     = GetPositionFromZDepthView(Depth.x, UV, g_ViewInverseMatrix, g_ProjectionInverseMatrix);
	float3  l_WorldNormal 		= Texture2Normal(l_NormalColor);
	
	float4 l_PixelColor = float4(0.0, 0.0, 0.0, 1.0);
	float LightAmount = 1.0;
	
	//-------------------------------------------------------------------------------------------------
	// LIGHT CALCULATIONS
	//-------------------------------------------------------------------------------------------------
	if(g_LightsEnabled[0] == 1 && l_SelfIlumAcum == 0) 
	{
		float3 l_Normal 			= normalize(l_WorldNormal);
		float3 l_LightVector		= l_WorldPosition - g_LightsPosition[0];
		float  l_Length 			= length( l_LightVector );
		float3 l_LightDirection 	= l_LightVector / l_Length;
		float  l_Attenuation 		= DistanceAttenuation(0, l_Length );
		
		if(DIR_LIGHT == g_LightsType[0])
		{
			l_LightDirection = normalize(g_LightsDirection[0]);
		}
		else if(SPOT_LIGHT == g_LightsType[0])
		{	
			l_Attenuation = l_Attenuation * SpotAttenuation(0, l_LightDirection);
		}
		
		float  lCommon = l_Attenuation * g_LightsColor[0] * g_LightsIntensity[0];
		float3 l_DiffuseContrib = l_DiffuseColor*saturate(dot(l_Normal,-l_LightDirection)) * lCommon;
		
		float3 l_Hn = normalize( normalize( g_CameraPosition - l_WorldPosition ) - l_LightDirection );
		float3 l_SpecularContrib = pow(saturate(dot(l_Normal,l_Hn)),g_SpecularExponent) * lCommon * l_SpecularFactor * g_LightsSpecularIntensity[0];
		
		l_PixelColor = l_PixelColor + float4( l_DiffuseContrib + l_SpecularContrib, 1.0);
	}
	
	//-------------------------------------------------------------------------------------------------
	// SHADOW MAP
	//-------------------------------------------------------------------------------------------------
	float4 PosLight = mul( float4(l_WorldPosition.xyz, 1.0), g_ViewToLightProjMatrix );
	
	//transform from RT space to texture space.
    float2 ShadowTexC = 0.5 * PosLight.xy / PosLight.w + float2( 0.5, 0.5 );
    ShadowTexC.y = 1.0f - ShadowTexC.y;
	
	//if( g_UseShadowMapStatic )
	{	
		if(!(ShadowTexC.x<0.0 || ShadowTexC.x>1.0 || ShadowTexC.y<0.0 || ShadowTexC.y>1.0))
		{
			LightAmount = (tex2D( ShadowMapTextureSampler, ShadowTexC ) + SHADOW_EPSILON) < (PosLight.z / PosLight.w)? 0.0f: 1.0f;  
		}
	}
	
	if( g_UseShadowMapDynamic )
	{	
		if( !(ShadowTexC.x<0.0 || ShadowTexC.x>1.0 || ShadowTexC.y<0.0 || ShadowTexC.y>1.0) )
		{
			LightAmount = (tex2D( ShadowMapTextureDynamicSampler, ShadowTexC ) + SHADOW_EPSILON) < (PosLight.z / PosLight.w)? 0.5f: 1.0f;  
		}
	}
	
	return float4(l_PixelColor.xyz * LightAmount, 1.0);
}

technique RenderDeferredTechnique {
	pass p0 {

		//AlphaBlendEnable = true;
		//BlendOp=Add;
		//SrcBlend = one;
		//DestBlend = one;
		VertexShader = null;
		PixelShader = compile ps_3_0 mainPS();
	}
}
