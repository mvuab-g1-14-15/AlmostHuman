#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

#if defined( USE_NORMAL )
TNORMAL_TAN_BI_T1_VERTEX_PS mainVS(TNORMAL_TAN_BI_T1_VERTEX IN)
{
TNORMAL_TAN_BI_T1_VERTEX_PS OUT=(TNORMAL_TAN_BI_T1_VERTEX_PS)0;
#elif defined( USE_SELF_ILUM )
TNORMAL_T2_VERTEX_PS mainVS(TNORMAL_T2_VERTEX IN)
{
TNORMAL_T2_VERTEX_PS OUT=(TNORMAL_T2_VERTEX_PS)0;
#else
TNORMAL_T1_VERTEX_PS mainVS(TNORMAL_T1_VERTEX IN)
{
	TNORMAL_T1_VERTEX_PS OUT=(TNORMAL_T1_VERTEX_PS)0;
#endif

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

    return OUT;
}

#if defined( USE_NORMAL )
TMultiRenderTargetPixel mainPS(TNORMAL_TAN_BI_T1_VERTEX_PS IN) : COLOR
#elif defined( USE_SELF_ILUM )
TMultiRenderTargetPixel mainPS(TNORMAL_T2_VERTEX_PS IN) : COLOR
#else
TMultiRenderTargetPixel mainPS(TNORMAL_T1_VERTEX_PS IN) : COLOR
#endif
{
	TMultiRenderTargetPixel OUT=(TMultiRenderTargetPixel)0;
	float4 l_DiffuseColor = tex2D(S0LinearSampler,IN.UV);
	if(l_DiffuseColor.a<0.5)
		clip(-1);

	OUT.Albedo=float4(l_DiffuseColor.xyz, g_SpecularFactor);

#if defined( USE_SELF_ILUM )
	OUT.Ambient=float4(l_DiffuseColor.xyz*g_AmbientLight*tex2D(S1LinearSampler, IN.UV2), g_SpecularExponent ); // TODO Ask Jordi si multiplicar por g_AmbientLight
#else
	OUT.Ambient=float4(l_DiffuseColor.xyz*g_AmbientLight, g_SpecularExponent );
#endif	

#if defined( USE_NORMAL )
	float3 Tn = normalize(IN.WorldTangent);
	float3 Bn = normalize(IN.WorldBinormal);
	float3 l_Normal = CalcNewNormal(Tn, Bn, tex2D(S1LinearSampler,IN.UV), normalize(IN.Normal));
#else
	float3 l_Normal = normalize(IN.Normal);
#endif
	
	OUT.Normal=float4(Normal2Texture(l_Normal), 1);
	OUT.Depth=IN.WorldPosition.z/IN.WorldPosition.w;
	
	return OUT;
}

#if defined( USE_NORMAL )
technique GenerateGBufferNormalTechnique
#elif defined( USE_SELF_ILUM )
technique GenerateGBufferSelfIlumTechnique
#else
technique GenerateGBufferTechnique
#endif
{
	pass p0
	{
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
