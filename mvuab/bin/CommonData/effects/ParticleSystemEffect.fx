#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

struct PSVertex
{
    float4 HPosition : POSITION;
    float3 params    : TEXCOORD0;
	float3 color        : COLOR0;
};

struct PARTICLEIN
{
	float3 Position : POSITION;
	float2 uv       : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float3 params   : TEXCOORD2;
	float3 color    : COLOR0;
	float2 flipUV	: TEXCOORD3;
};

PSVertex VS(PARTICLEIN IN)
{
	PSVertex OUT = (PSVertex)0;
	
	if( IN.params.y  > 0.0 )
	{
		float3 rightVector = normalize(float3(g_ViewMatrix[0][0], g_ViewMatrix[1][0], g_ViewMatrix[2][0]));
		float3 upVector = normalize(float3(g_ViewMatrix[0][1], g_ViewMatrix[1][1], g_ViewMatrix[2][1]));
		float3 viewVector = normalize(float3(g_ViewMatrix[0][2], g_ViewMatrix[1][2], g_ViewMatrix[2][2]));
		
		float sn = sin( IN.params.y);
		float cs = cos( IN.params.y);
		
#if defined( RAY )
		float3 lPosition = IN.worldPos + ( ( IN.Position.x * rightVector  )+ (IN.Position.z*upVector)) * IN.params.x;
#else
		float3 vUpNew    = cs * rightVector - sn * upVector;
		float3 vRightNew = sn * rightVector + cs * upVector;
		float3 lPosition = IN.worldPos + ( ( IN.Position.x * vRightNew  )+ (IN.Position.z*vUpNew)) * IN.params.x;
#endif

		OUT.HPosition=mul(float4(lPosition, 1.0), g_WorldViewProj);
		
		float u = IN.uv.x;
		if( IN.flipUV.x > 0.99 ) u = 1.0 - u;
		
		float v = IN.uv.y;
		if( IN.flipUV.y > 0.99 ) v = 1.0 - v;
		
		OUT.params = float3( u, v, IN.params.z);
		OUT.color = IN.color;
	}
	
    return OUT;
}

float4 PS(PSVertex IN) : COLOR
{
	float4 lSamplerColor = tex2D(S0PointClampSampler, float2( IN.params.x, IN.params.y) );
#if defined( BLACK_AND_WHITE )
	float lAverage = (( IN.color.x + IN.color.y + IN.color.z) / 3 );
	float3 lColor =  float3( lAverage, lAverage, lAverage );
#elif defined ( RAY )
	float lAverage = (( IN.color.x + IN.color.y + IN.color.z) / 3 );
	float3 lColor =  float3( lAverage, lAverage, 1.0 );
	return float4( lSamplerColor.rgb * lColor.rgb , lSamplerColor.a);
#else
	float3 lColor = IN.color;
#endif 
	return float4( lSamplerColor.rgb * lColor.rgb , lSamplerColor.a * IN.params.z);
}


technique TECHNIQUE_NAME
{
	pass p0
	{
		//AlphaBlendEnable = false;
		//CullMode = NONE; // NONE - CW
		CullMode = NONE;
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS();
	}
}