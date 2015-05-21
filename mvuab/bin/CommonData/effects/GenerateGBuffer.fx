#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

UBER_VERTEX_PS mainVS(UBER_VERTEX_VS IN)
{
	UBER_VERTEX_PS OUT=(UBER_VERTEX_PS)0;
    
#if defined( USE_CAL3D_HW )
	float3 l_Normal  = normalize(IN.Normal).xyz;
	float3 l_Tangent = IN.Tangent.xyz;
	
	CalcAnimatedNormalTangent(IN.Normal.xyz, IN.Tangent.xyz, IN.Indices, IN.Weight, l_Normal, l_Tangent);
	float3 l_Position = CalcAnimtedPos(float4(IN.Position.xyz,1.0), IN.Indices, IN.Weight);
	
	// Modify the x coordinate ( symmetry )
	l_Position.x = -l_Position.x;
	float4 l_WorldPosition = float4(l_Position, 1.0);
	
	OUT.WorldPosition = mul(l_WorldPosition, g_WorldMatrix);
	
	
	OUT.Normal = normalize(mul(l_Normal, g_WorldMatrix));
	OUT.WorldNormal = OUT.Normal;
	
	l_Normal = l_Normal* -1;
	
	OUT.WorldTangent = float4(normalize(mul(l_Tangent, (float3x3)g_WorldMatrix)), 0.0);
	OUT.WorldBinormal = float4(mul(cross(l_Tangent,l_Normal), (float3x3)g_WorldMatrix), 0.0);
	
	OUT.HPosition = mul(l_WorldPosition, g_WorldViewProj );
#else
	OUT.HPosition=mul(float4(IN.Position, 1.0), g_WorldViewProj);
    OUT.Normal=mul(IN.Normal, (float3x3)g_WorldMatrix);
    OUT.WorldPosition=OUT.HPosition;
#endif

	OUT.UV=IN.UV;

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
		l_DiffuseColor = g_DebugColor;
		
#if defined( USE_DIFFUSE_COLOR )
		l_DiffuseColor = IN.Color;
#endif

	if(l_DiffuseColor.a<0.5)
		clip(-1);
		
	OUT.Albedo=float4(l_DiffuseColor.xyz, g_SpecularFactor);
	float3 l_AmbientColor = float3(0,0,0);
#if defined( USE_SELF_ILUM )
	l_AmbientColor = l_DiffuseColor+tex2D(S1LinearSampler, IN.UV2)*2; //Darle más potencia para que se acerque más a lo que se ve en el MAX
#elif defined( USE_RNM )
	float3 l_LightMap = GetRadiosityNormalMapBySamplers(FaceNn, FaceNormal, FaceTangent, FaceBinormal, IN.UV2, S2LinearSampler, S3LinearSampler, S4LinearSampler);
	l_AmbientColor = l_DiffuseColor * l_LightMap * 2; //Darle más potencia para que se acerque más a lo que se ve en el MAX
#else
	l_AmbientColor = l_DiffuseColor.xyz * g_AmbientLight;
#endif
	
	OUT.Ambient=float4( l_AmbientColor, g_SpecularExponent );

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
#if defined( USE_CAL3D_HW )
		CullMode = NONE;
#endif
	
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
