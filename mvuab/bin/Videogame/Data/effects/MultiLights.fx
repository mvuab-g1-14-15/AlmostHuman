/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

#include "globals.fxh"
#include "samples.fxh"

struct ApplitationToVertex
{
    float4 vertexPosition   : POSITION0;
    float3 vertexNormal     : NORMAL0;
    float2 vertexTexture    : TEXCOORD0;
};

struct VertexToPixel
{
    float4 vertexPosition   : POSITION0;
    float2 vertexTexture    : TEXCOORD0;
    
    float3 worldPosition    : TEXCOORD1;
    float3 worldNormal      : TEXCOORD2;
};


float4 CalcOmniLight(int i, float3 worldPosition, float3 worldNormal, float4 diffuseColor, float4 specularColor)
{
    float3 vertexToLight  = normalize(g_LightsPosition[i] - worldPosition);
    float3 vertexToCamera = normalize(g_ViewInverseMatrix[3].xyz - worldPosition);
	
	float dstToLight = length(worldPosition - g_LightsPosition[i]);
    float attenuation =  1.0 - saturate((dstToLight - g_LightsStartRangeAttenuation[i]) / (g_LightsEndRangeAttenuation[i] - g_LightsStartRangeAttenuation[i]));
    
    float outDiffuseColor  = saturate(dot(vertexToLight, worldNormal));
    float outSpecularColor = pow(saturate(dot(normalize(vertexToCamera + vertexToLight), worldNormal)), 50.0);
    
    float4 finalDiffuse  = float4(attenuation * outDiffuseColor * diffuseColor.xyz, diffuseColor.a);
    float4 finalSpecular = float4(attenuation * outSpecularColor * specularColor.xyz, 1.0);
    
    return finalDiffuse + finalSpecular;
}

float4 CalcDirectionalLight(int i, float3 worldPosition, float3 worldNormal, float4 diffuseColor, float4 specularColor)
{
    float3 vertexToLight  = normalize(g_LightsPosition[i]);
    float3 vertexToCamera = normalize(g_ViewInverseMatrix[3].xyz - worldPosition);
	
	float dstToLight = length(worldPosition - g_LightsPosition[i]);
    float attenuation =  1.0 - saturate((dstToLight - g_LightsStartRangeAttenuation[i]) / (g_LightsEndRangeAttenuation[i] - g_LightsStartRangeAttenuation[i]));
    
    float outDiffuseColor  = saturate(dot(vertexToLight, worldNormal));
    float outSpecularColor = pow(saturate(dot(normalize(vertexToCamera + vertexToLight), worldNormal)), 50.0);
    
    float4 finalDiffuse  = float4(attenuation * outDiffuseColor * diffuseColor.xyz, diffuseColor.a);
    float4 finalSpecular = float4(attenuation * outSpecularColor * specularColor.xyz, 1.0);
    
    return finalDiffuse + finalSpecular;
}

float4 CalcSpotLight(int i, float3 worldPosition, float3 worldNormal, float4 diffuseColor, float4 specularColor)
{
    float3 vertexToLight  = normalize(g_LightsPosition[i] - worldPosition);
    float3 vertexToCamera = normalize(g_ViewInverseMatrix[3].xyz - worldPosition);
	
	float dstToLight = length(worldPosition - g_LightsPosition[i]);
    float attenuationDst =  1.0 - saturate((dstToLight - g_LightsStartRangeAttenuation[i]) / (g_LightsEndRangeAttenuation[i] - g_LightsStartRangeAttenuation[i]));
    
    float l_Angle = cos(Deg2Rad(g_LightsAngle[i]) / 2.0);
    float l_AngleFallOff = cos(Deg2Rad(g_LightsFallOff[i]) / 2.0);
	
	float l_SpotDotValue = dot(vertexToLight, g_LightsDirection[i]);
    float attenuationAng = saturate((l_SpotDotValue - l_AngleFallOff) / (l_Angle - l_AngleFallOff));
    
    float outDiffuseColor  = saturate(dot(vertexToLight, worldNormal));
    float outSpecularColor = pow(saturate(dot(normalize(vertexToCamera + vertexToLight), worldNormal)), 50.0);
    
    float4 finalDiffuse  = float4(attenuationAng * attenuationDst * outDiffuseColor * diffuseColor.xyz, diffuseColor.a);
    float4 finalSpecular = float4(attenuationAng * attenuationDst * outSpecularColor * specularColor.xyz, 1.0);
    
    return finalDiffuse + finalSpecular;
}

VertexToPixel mainVS(ApplitationToVertex IN)
{
    VertexToPixel l_OUT = (VertexToPixel) 0;
    
    l_OUT.vertexPosition = mul(IN.vertexPosition, g_WorldViewProj);
    l_OUT.vertexTexture = IN.vertexTexture;
    
    l_OUT.worldPosition = mul(IN.vertexPosition, g_WorldMatrix);
    l_OUT.worldPosition = mul(l_OUT.worldPosition, g_ViewMatrix);
	
    l_OUT.worldNormal = mul(IN.vertexNormal, g_WorldMatrix);
    l_OUT.worldNormal = mul(l_OUT.worldNormal, g_ViewInverseMatrix);
    
    return l_OUT;
}

float4 mainPS(VertexToPixel IN) : COLOR
{
    float3 l_Normal = normalize(IN.worldNormal);
    float3 l_Position = IN.worldPosition;
    
    float4 diffuse    = float4(1.0, 1.0, 1.0, 1.0); //tex2D(S0LineaWrapSample, IN.vertexTexture);
    float4 specular   = float4(1.0, 1.0, 1.0, 1.0);
    float4 finalColor = float4(0.0, 0.0, 0.0, 1.0);
    
    for(int i = 0; i < MAX_LIGHTS_BY_SHADER; i++)
    {
        if(g_LightsEnabled[i]) 
        {
            g_LightsPosition[i] = mul(g_LightsPosition[i], g_ViewInverseMatrix);
            
            if(OMNI_LIGHT == g_LightsType[i])
            {
                finalColor += CalcOmniLight(i, l_Position, l_Normal, diffuse, specular);
            }
            if(DIR_LIGHT == g_LightsType[i])
            {
                finalColor += CalcDirectionalLight(i, l_Position, l_Normal, diffuse, specular);
            }
            else if(SPOT_LIGHT == g_LightsType[i])
            {
                finalColor += CalcSpotLight(i, l_Position, l_Normal, diffuse, specular);
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