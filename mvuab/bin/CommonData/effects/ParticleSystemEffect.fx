#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

/*
TT1_VERTEX_PS mainVS(TT1_VERTEX_VS IN)
{
	TT1_VERTEX_PS OUT=(TT1_VERTEX_PS)0;

	float3 rightVector 	= normalize(float3(g_ViewMatrix[0][0], g_ViewMatrix[1][0], g_ViewMatrix[2][0] ));
	float3 upVector 	= normalize(float3(g_ViewMatrix[0][1], g_ViewMatrix[1][1], g_ViewMatrix[2][1] ));
	float3 viewVector	= normalize(float3(g_ViewMatrix[0][2], g_ViewMatrix[1][2], g_ViewMatrix[2][2] ));

	float3 position 	= (IN.Position.x*rightVector+IN.Position.z*upVector) * g_Size;
	OUT.Normal			= float4( viewVector.xyz, 1);;
	OUT.WorldTangent	= float4( rightVector.xyz, 1);
	OUT.WorldBinormal	= float4( upVector.xyz, 1);
	OUT.Position		= mul( float4( position.xyz, 1), g_WorldViewProj);
	OUT.WorldPosition	= OUT.Position;
	OUT.UV				= IN.UV;
	
    return OUT;
}

//Dest.rgb*One + Source.rgb*One

float4 mainPS(TT1_VERTEX_PS IN) : COLOR
{
	float4 l_Color=tex2D(S0PointSampler, IN.UV);
	return float4(l_Color.xyz*l_Color.a*0.5, l_Color.a);//float4(1, 0, 0, 1);
}

technique TECHNIQUE_NAME
{
	pass p0
	{
		CullMode = NONE;
#if defined( NOT_ALIGNED )
		
#endif
		AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend = one;
		DestBlend = one;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
*/

struct PSVertex
{
    float4 HPosition : POSITION;
    float3 params    : TEXCOORD0;
	float3 color        : COLOR0;
};

struct PARTICLEIN
{
	float3 Position  : POSITION;
	float2 uv           : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float3 params    : TEXCOORD2;
	float3 color    : COLOR0;
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
		
		float3 vUpNew    = cs * rightVector - sn * upVector;
		float3 vRightNew = sn * rightVector + cs * upVector;
	
		float3 lPosition = IN.worldPos + ( ( IN.Position.x * vRightNew  )+ (IN.Position.z*vUpNew)) * IN.params.x;

		OUT.HPosition=mul(float4(lPosition, 1.0), g_WorldViewProj);
		OUT.params = float3( IN.uv.x, IN.uv.y, IN.params.z);
		OUT.color = IN.color;
	}
	
    return OUT;
}

float4 PS(PSVertex IN) : COLOR
{
	float4 color = tex2D(S0PointClampSampler, float2( IN.params.x, IN.params.y) ); 
	return float4( color.rgb * IN.color, color.a * IN.params.z);
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