#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

UBER_VERTEX_PS mainVS(UBER_VERTEX_VS IN)
{
	UBER_VERTEX_PS OUT=(UBER_VERTEX_PS)0;
    OUT.HPosition=mul(float4(IN.Position, 1.0), g_WorldViewProj);
    OUT.UV=IN.UV;
    OUT.Normal=mul(IN.Normal, (float3x3)g_WorldMatrix);
    OUT.WorldPosition=OUT.HPosition;

#if defined( USE_NORMAL )
	OUT.WorldTangent = float4( mul(IN.Tangent.xyz, (float3x3)g_WorldMatrix), 1.0);
	OUT.WorldBinormal = float4(mul(IN.Binormal.xyz, (float3x3)g_WorldMatrix),1.0);
#endif

#if defined( USE_SELF_ILUM )
	OUT.UV2 = IN.UV2;
#endif

#if defined( USE_DIFFUSE_COLOR )
	OUT.Color = IN.Color;
#endif

    return OUT;
}

TMultiRenderTargetPixel mainPS(UBER_VERTEX_PS IN) : COLOR
{
	TMultiRenderTargetPixel OUT=(TMultiRenderTargetPixel)0;
	float4 l_DiffuseColor = tex2D(S0LinearSampler,IN.UV);
	
	if(g_UseDebugColor)
		l_DiffuseColor = float4(1,0,0,0);
		
#if defined( USE_DIFFUSE_COLOR )
		l_DiffuseColor = IN.Color;
#endif

	if(l_DiffuseColor.a<0.5)
		clip(-1);
		
	OUT.Albedo=float4(l_DiffuseColor.xyz, g_SpecularFactor);
	
#if defined( USE_SELF_ILUM )
	l_DiffuseColor = l_DiffuseColor+tex2D(S1LinearSampler, IN.UV2);
#elif defined( USE_RNM )
	float3 l_LightMap = GetRadiosityNormalMapBySamplers(FaceNn, FaceNormal, FaceTangent, FaceBinormal, IN.UV2, S2LinearSampler, S3LinearSampler, S4LinearSampler);
	l_DiffuseColor = l_DiffuseColor + l_LightMap;
#endif
	
	OUT.Ambient=float4(l_DiffuseColor.xyz*g_AmbientLight, g_SpecularExponent );

#if defined( USE_NORMAL )
	#if defined( USE_SELF_ILUM )
		float4 l_TexNormal = tex2D(S2LinearSampler,IN.UV);
	#else
		float4 l_TexNormal = tex2D(S1LinearSampler,IN.UV);
	#endif
	float3 l_Normal = CalcNewNormal( normalize(IN.WorldTangent), 
									 normalize(IN.WorldBinormal),
									 l_TexNormal,
									 normalize(IN.Normal));
#else
	float3 l_Normal = normalize(IN.Normal);
#endif
	
	OUT.Normal=float4(Normal2Texture(l_Normal), 1);
	OUT.Depth=IN.WorldPosition.z/IN.WorldPosition.w;
	
	return OUT;
}

technique TECHNIQUE_NAME
{
	pass p0
	{
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
