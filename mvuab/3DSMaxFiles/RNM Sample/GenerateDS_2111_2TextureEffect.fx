#include "../EffectsHeader.fxh"

/*
struct TNORMAL_TAN_BI_TEXTURED2_VERTEX_VS
{
	float3 Position : POSITION;
	float4 Normal : NORMAL;
	float4 Tangent : TANGENT;
	float4 Binormal : BINORMAL;
	float2 UV : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
 };
*/ 
struct TNORMAL_TAN_BI_TEXTURED_RNM_VERTEX_PS
{
    float4 HPosition : POSITION;
    float2 UV : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
    float3 Normal : TEXCOORD2;
	float3 WorldPosition : TEXCOORD3;
	float3 WorldTangent : TEXCOORD4;
	float3 WorldBinormal : TEXCOORD5;
	float3 FaceNormal : TEXCOORD6;
	float3 FaceTangent : TEXCOORD7;
};

float3 GetRadiosityNormalMapBySamplers(float3 Nn, float3 FaceNormal, float3 Tn, float3 Bn, float2 UV, sampler SamplerX, sampler SamplerY, sampler SamplerZ)
{
	float3x3 l_TangentSpaceMatrix;
	l_TangentSpaceMatrix[0] = normalize(Tn);
	l_TangentSpaceMatrix[1] = normalize(FaceNormal);
	l_TangentSpaceMatrix[2] = normalize(Bn);

	float3 l_LightmapX=(tex2D(SamplerX, UV).xyz);
	float3 l_LightmapY=(tex2D(SamplerY, UV).xyz);
	float3 l_LightmapZ=(tex2D(SamplerZ, UV).xyz);
	float3 l_BumpBasisX=normalize(mul(float3(0.816496580927726, 0.5773502691896258, 0 ),l_TangentSpaceMatrix));
	float3 l_BumpBasisY=normalize(mul(float3(-0.408248290463863, 0.5773502691896258,0.7071067811865475 ),l_TangentSpaceMatrix));
	float3 l_BumpBasisZ=normalize(mul(float3(-0.408248290463863, 0.5773502691896258, -0.7071067811865475),l_TangentSpaceMatrix));
	
	//http://www.valvesoftware.com/publications/2007/SIGGRAPH2007_EfficientSelfShadowedRadiosityNormalMapping.pdf
	
	float3 dp;
	dp.x=saturate( dot( Nn, l_BumpBasisX ) );
	dp.y=saturate( dot( Nn, l_BumpBasisY ) );
	dp.z=saturate( dot( Nn, l_BumpBasisZ ) );
	
	dp*=dp;
	float sum=dot(dp, float3(1.0, 1.0, 1.0));
	float3 l_DiffuseLight=dp.x*l_LightmapX+dp.y*l_LightmapY+dp.z*l_LightmapZ;
	l_DiffuseLight/=sum;
	
	return l_DiffuseLight*2;

	//float3 diffuseLighting=saturate( dot( Nn, l_BumpBasisX ) ) * l_LightmapX + saturate( dot( Nn, l_BumpBasisY ) ) * l_LightmapY + saturate( dot( Nn, l_BumpBasisZ ) ) * l_LightmapZ;
	
}
 
//--- SHADER ---//
TNORMAL_TAN_BI_TEXTURED_RNM_VERTEX_PS DeferredShadingVS(TNORMAL_TAN_BI_TEXTURED2_VERTEX_VS IN)
{
	TNORMAL_TAN_BI_TEXTURED_RNM_VERTEX_PS OUT = (TNORMAL_TAN_BI_TEXTURED_RNM_VERTEX_PS)0;
	OUT.UV = IN.UV.xy;
	OUT.UV2 = IN.UV2.xy;
	OUT.Normal =  normalize(mul(IN.Normal, (float3x3)WorldMatrixParameter));
	OUT.HPosition = mul(float4(IN.Position,1.0), WorldViewProjectionMatrixParameter);
	OUT.WorldPosition = OUT.HPosition.yzw; //lo usaremos para guardar la z y la w, necesaqrias para la profundidad
	
	OUT.FaceNormal=IN.Normal.xyz;
	OUT.FaceTangent=IN.Tangent.xyz;
	return OUT;
}

TMultiRenderTargetPixel DeferredShadingPS(TNORMAL_TAN_BI_TEXTURED_RNM_VERTEX_PS IN)
{
	TMultiRenderTargetPixel OUT = (TMultiRenderTargetPixel)0;
	float3 Nn=normalize(IN.Normal);
	float4 l_Diffuse = tex2D(S0LinearSampler , IN.UV);
	float4 l_LightMap = tex2D(S1LinearSampler, IN.UV2);
	float3 l_AmbientLight = l_Diffuse.xyz * l_LightMap.xyz;
	float4 l_Depth = IN.WorldPosition.y/IN.WorldPosition.z; // depth = z/w
	float l_SpecFactor = l_LightMap.w;
	float l_Alpha = l_Diffuse.w;
	
	if(l_Alpha<1.0)
		clip(-1);
	
	float3 FaceNormal=normalize(IN.FaceNormal);
	float3 FaceTangent=normalize(IN.FaceTangent);
	float3 FaceBinormal=normalize(cross(FaceNormal, FaceTangent));
	
	/*
	float3 Nn=normalize(IN.Normal);
	float3 Tn=normalize(IN.WorldTangent);
	float3 Bn=normalize(IN.WorldBinormal);
	float3 bump=BUMP*(tex2D(S1LinearSampler, IN.UV).xyz - float3(0.5,0.5,0.5));
	Nn = Nn + bump.x*Tn + bump.y*Bn;
	Nn = normalize(Nn);
	*/
	
	float3 bump=BUMP*(tex2D(S4LinearSampler, IN.UV).xyz - float3(0.5,0.5,0.5));
	float3 FaceNn = FaceNormal + bump.x*FaceTangent + bump.y*FaceBinormal;
	FaceNn = normalize(FaceNn);
		
	l_AmbientLight=GetRadiosityNormalMapBySamplers(FaceNn, FaceNormal, FaceTangent, FaceBinormal, IN.UV2, S1LinearSampler, S2LinearSampler, S3LinearSampler);
	//l_AmbientLight=FaceNn;
		
	OUT.RT0 = float4(l_Diffuse.xyz, l_SpecFactor);//albedo + specular factor
	OUT.RT0 = float4(float3(1,1,1), l_SpecFactor);//albedo + specular factor
	OUT.RT1 = float4(l_AmbientLight, l_Alpha); //albedo*AmbientLight + transparencia
	OUT.RT2 = float4(Normal2Texture(Nn), 1.0); //Normal  (4th float not used)
	OUT.RT3 = l_Depth; //Depth
	
	return OUT;
}

technique GenerateDS_2111_Technique
{
	pass p0
	{
		/*AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend = one;
		DestBlend = one;*/
		VertexShader = compile vs_3_0 DeferredShadingVS();
		PixelShader = compile ps_3_0 DeferredShadingPS();
	}
}