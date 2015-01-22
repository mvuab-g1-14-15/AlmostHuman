/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

#include "globals.fxh"

struct ApplitationToVertex
{
	float4 vertexPosition	: POSITION0;
	float3 vertexNormal		: NORMAL0;
	float2 vertexTexture 	: TEXCOORD0;
};

struct VertexToPixel
{
	float4 vertexPosition	: POSITION0;
	float2 vertexTexture	: TEXCOORD0;
	
	float3 worldPosition	: TEXCOORD1;
	float3 worldNormal		: TEXCOORD2;
};


float4 CalcPointLighting(float3 lightPosition, float3 worldPosition, float3 worldNormal, float4 diffuseColor, float4 specularColor)
{
	float3 vertexToLight  = normalize(lightPosition - worldPosition);
	float3 vertexToCamera = normalize(g_ViewInverseMatrix[3].xyz - worldPosition);
	
	float outDiffuseColor  = saturate(dot(worldNormal, vertexToLight));
	float outSpecularColor = pow(saturate(dot(worldNormal, normalize(vertexToCamera + vertexToLight))), 50.0);
	
	float dstToLight = length(lightPosition - worldPosition);
	float attenuation = 1.0f / (1.0f + 1.0f * pow(dstToLight, 2.0f));
	
	return attenuation * (float4(outDiffuseColor * diffuseColor.xyz, diffuseColor.a) + float4(outSpecularColor * specularColor.xyz, 0));
}

float4 CalcSpotLighting(float3 lightPosition, float3 lightDirection, float angle, float angleFallOff, float3 worldPosition, float3 worldNormal, float4 diffuseColor, float4 specularColor)
{
	float3 vertexToLight  = normalize(lightPosition - worldPosition);
	float3 vertexToCamera = normalize(g_ViewInverseMatrix[3].xyz - worldPosition);
	
	float outDiffuseColor  = saturate(dot(worldNormal, vertexToLight));
	float outSpecularColor = pow(saturate(dot(worldNormal, normalize(vertexToCamera + vertexToLight))), 50.0);
	
	float l_Angle = cos(Deg2Rad(angle) / 2.0);
	float l_AngleFallOff = cos(Deg2Rad(angleFallOff) / 2.0);
	
	float l_SpotDotValue = dot(vertexToLight, lightDirection);
	float attenuation = saturate((l_SpotDotValue-l_AngleFallOff)/(l_Angle-l_AngleFallOff));
	
	return attenuation * (float4(outDiffuseColor * diffuseColor.xyz, diffuseColor.a) + float4(outSpecularColor * specularColor.xyz, 0));
}

VertexToPixel mainVS(ApplitationToVertex IN)
{
	VertexToPixel l_OUT = (VertexToPixel) 0;
	
	l_OUT.vertexPosition = mul(IN.vertexPosition, g_WorldViewProj);
	l_OUT.vertexTexture = IN.vertexTexture;
	
	l_OUT.worldPosition = mul(IN.vertexPosition, (float3x3) g_WorldMatrix);
	l_OUT.worldNormal = mul(IN.vertexNormal, (float3x3) g_WorldMatrix);
	
	
	return l_OUT;
}

float4 mainPS(VertexToPixel IN) : COLOR
{
	return float4(1,0,0,1);
	float3 l_Normal = normalize(IN.worldNormal);
	float3 l_Position = IN.worldPosition;
	
	float4 diffuse  = 1.0;
	float4 specular = 1.0;
	
	float4 finalColor = 0.0;
	
	for(int i = 0; i < MAX_LIGHTS_BY_SHADER; i++)
	{
		if(g_LightsEnabled[i]) 
		{
			if(POINT_LIGHT == g_LightsType[i])
			{
				finalColor += CalcPointLighting(g_LightsPosition[i], l_Position, l_Normal, diffuse, specular);
			}
			else if(SPOT_LIGHT == g_LightsType[i])
			{
				finalColor += CalcSpotLighting(g_LightsPosition[i], g_LightsDirection[i], g_LightsAngle[i], g_LightsFallOff[i], l_Position, l_Normal, diffuse, specular);
			}
		}
	}
		
	return finalColor;
}

technique MultiLightsTechnique
{
	pass p0
	{
		CullMode = None;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}