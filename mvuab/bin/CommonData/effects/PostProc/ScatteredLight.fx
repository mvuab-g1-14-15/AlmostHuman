// http://fabiensanglard.net/lightScattering/
// http://http.developer.nvidia.com/GPUGems3/gpugems3_ch13.html
// https://medium.com/community-play-3d/god-rays-whats-that-5a67f26aeac2

#include "../samplers.fxh"
#include "../globals.fxh"

float4 OcclusionMap(float2 UV, float4 l_LightColor, float4 l_LightPos)
{
    float l_Depth = tex2D(S1LinearSampler, UV).x;
    float3 l_VertexPos = GetPositionFromZDepthView(l_Depth, UV, g_ViewInverseMatrix, g_ProjectionInverseMatrix);
    
    float l_DstCameraToLight = length(g_ViewInverseMatrix[3].xyz - l_LightPos.xyz);
    float l_DstCameraToVertex = length(g_ViewInverseMatrix[3].xyz - l_VertexPos.xyz);
    
    if(l_DstCameraToVertex < l_DstCameraToVertex) return float4(0.0, 0.0, 0.0, 1.0);
    else return l_LightColor;
}

float4 mainPS(in float2 UV : TEXCOORD0) : COLOR
{
    float4 l_Color = float4(0.4, 0.4, 0.4, 1.0);
    float4 l_LPos  = float4(112.0, -5.0, -34.68, 1.0);
    
    return OcclusionMap(UV, l_Color, l_LPos);
     
    /*float4 l_LightScreenSpace = mul(l_LPos, g_WorldViewProj);
    float2 l_DeltaTexCoord = UV - l_LightScreenSpace.xy / l_LightScreenSpace.w;
    float2 l_TexCoord = UV;
    
    float l_IlluminationDecay = 1.0;
    l_DeltaTexCoord *= 1.0 / float(g_Samples) * g_SDensity; 
    
    for(int i=0; i < g_Samples ; i++)
    {
        l_TexCoord -= l_DeltaTexCoord;
        
        float4 sample = tex2D(S0LinearSampler, l_TexCoord);
        sample *= l_IlluminationDecay * g_SWeight;
        
        l_Color += sample;
        l_IlluminationDecay *= g_SDecay;
    }
    
    return float4(l_Color * g_SExposure, 1.0);*/
}

technique ScatteredLightTechnique
{
    pass p0
    {
        PixelShader = compile ps_3_0 mainPS();
    }
}
