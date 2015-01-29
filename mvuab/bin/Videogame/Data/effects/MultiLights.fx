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


float4 CalcOmniLight(float3 lightPosition, float3 worldPosition, float3 worldNormal, float4 diffuseColor, float4 specularColor, float nearAt, float farAt)
{
    float3 vertexToLight  = normalize(lightPosition - worldPosition);
    float3 vertexToCamera = normalize(g_ViewInverseMatrix[3].xyz - worldPosition);
	
	float dstToLight = length(worldPosition - lightPosition);
    float attenuation =  1.0 - saturate((dstToLight - nearAt) / (farAt - nearAt));
    
    float outDiffuseColor  = saturate(dot(vertexToLight, worldNormal));
    float outSpecularColor = pow(saturate(dot(vertexToCamera, reflect(vertexToLight, worldNormal))), 2.0);
    
    float4 finalDiffuse  = float4(attenuation * outDiffuseColor * diffuseColor.xyz, diffuseColor.a);
    float4 finalSpecular = float4(attenuation * outSpecularColor * specularColor.xyz, 0.0);
    
    return finalDiffuse + finalSpecular;
}

float4 CalcDirectionalLight(float3 lightPosition, float3 worldPosition, float3 worldNormal, float4 diffuseColor, float4 specularColor, float nearAt, float farAt)
{
    float3 vertexToLight  = normalize(lightPosition);
    float3 vertexToCamera = normalize(g_ViewInverseMatrix[3].xyz - worldPosition);
	
	float dstToLight = length(worldPosition - lightPosition);
    float attenuation =  1.0 - saturate((dstToLight - nearAt) / (farAt - nearAt));
    
    float outDiffuseColor  = saturate(dot(worldNormal, vertexToLight));
    float outSpecularColor = pow(saturate(dot(worldNormal, normalize(vertexToCamera + vertexToLight))), 2.0);
    
    float4 finalDiffuse  = float4(attenuation * outDiffuseColor * diffuseColor.xyz, diffuseColor.a);
    float4 finalSpecular = float4(attenuation * outSpecularColor * specularColor.xyz, 0.0);
    
    return finalDiffuse + finalSpecular;
}

float4 CalcSpotLight(float3 lightPosition, float3 lightDirection, float angle, float angleFallOff, float3 worldPosition, float3 worldNormal, float4 diffuseColor, float4 specularColor)
{
    float3 vertexToLight  = normalize(lightPosition - worldPosition);
    float3 vertexToCamera = normalize(g_ViewInverseMatrix[3].xyz - lightPosition);
	
	float l_Angle = cos(Deg2Rad(angle) / 2.0);
    float l_AngleFallOff = cos(Deg2Rad(angleFallOff) / 2.0);
	
	float l_SpotDotValue = dot(vertexToLight, lightDirection);
    float attenuation = saturate((l_SpotDotValue - l_AngleFallOff) / (l_Angle - l_AngleFallOff));
    
    float outDiffuseColor  = saturate(dot(worldNormal, vertexToLight));
    float outSpecularColor = pow(saturate(dot(worldNormal, normalize(vertexToCamera + vertexToLight))), 50.0);
	
	float4 finalDiffuse  = float4(attenuation * outDiffuseColor * diffuseColor.xyz, diffuseColor.a);
    float4 finalSpecular = float4(attenuation * outSpecularColor * specularColor.xyz, 0.0);
    
    return finalDiffuse + finalSpecular;
}

VertexToPixel mainVS(ApplitationToVertex IN)
{
    VertexToPixel l_OUT = (VertexToPixel) 0;
    
    l_OUT.vertexPosition = mul(IN.vertexPosition, g_WorldViewProj);
    l_OUT.vertexTexture = IN.vertexTexture;
    
    l_OUT.worldPosition = mul(IN.vertexPosition, g_WorldMatrix);	
    l_OUT.worldNormal = mul(IN.vertexNormal, g_WorldMatrix);
	
    return l_OUT;
}

float4 mainPS(VertexToPixel IN) : COLOR
{
    float3 l_Normal = normalize(IN.worldNormal);
    float3 l_Position = IN.worldPosition;
    
    float4 diffuse    = tex2D(S0LineaWrapSample, IN.vertexTexture);
    float4 specular   = float4(1.0, 1.0, 1.0, 1.0);
    float4 finalColor = float4(0.0, 0.0, 0.0, 1.0);
	
    for(int i = 0; i < MAX_LIGHTS_BY_SHADER; i++)
    {
        if(g_LightsEnabled[i]) 
        {
            if(OMNI_LIGHT == g_LightsType[i])
            {
                finalColor += CalcOmniLight(g_LightsPosition[i], l_Position, l_Normal, diffuse, specular, g_LightsStartRangeAttenuation[i], g_LightsEndRangeAttenuation[i]);
            }
            if(DIR_LIGHT == g_LightsType[i])
            {
                finalColor += CalcDirectionalLight(g_LightsPosition[i], l_Position, l_Normal, diffuse, specular, g_LightsStartRangeAttenuation[i], g_LightsEndRangeAttenuation[i]);
            }
            else if(SPOT_LIGHT == g_LightsType[i])
            {
                finalColor += CalcSpotLight(g_LightsPosition[i], g_LightsDirection[i], g_LightsAngle[i], g_LightsFallOff[i], l_Position, l_Normal, diffuse, specular);
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