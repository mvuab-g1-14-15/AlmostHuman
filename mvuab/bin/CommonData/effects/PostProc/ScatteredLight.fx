// http://fabiensanglard.net/lightScattering/
// http://http.developer.nvidia.com/GPUGems3/gpugems3_ch13.html
// https://medium.com/community-play-3d/god-rays-whats-that-5a67f26aeac2

#include "../samplers.fxh"
#include "../globals.fxh"

int        g_Samples = 50;
float      g_SDecay = 1.0;
float      g_SExposure = 0.0034;
float      g_SDensity  = 0.84;
float      g_SWeight = 5.65;

float4 Merge(in float2 UV : TEXCOORD0) : COLOR
{
    float4 s1 = tex2D(S0LinearSampler, UV);
    float4 s2 = tex2D(S1LinearSampler, UV);
    
    return s1 + s2;
}

float4 ScatterLight(in float2 UV : TEXCOORD0) : COLOR
{
    float4 l_Color = float4(0, 0, 0, 0);
    l_Color = tex2D(S0LinearSampler, UV);
        
    for(int i = 0; i < MAX_LIGHTS_BY_SHADER; i++)
    {
        if(g_LightsEnabled[i])
        {
            float4 l = mul(float4(g_LightsPosition[i], 1.0f), g_WorldViewProj);
            l.xy /= l.w;
            
            l.x = 0.5f + l.x / 2.0f;
            l.y = 1.0f - (0.5f + l.y / 2.0f);	
            
            float2 l_DeltaTexCoord = UV - l.xy;
            float2 l_TexCoord = UV;
            
            float l_IlluminationDecay = 1.0;
            l_DeltaTexCoord *= g_SDensity / float(g_Samples); 
            
            for(int j = 0; j < g_Samples; j++)
            {
                l_TexCoord -= l_DeltaTexCoord;
                
                float4 sample = tex2D(S0LinearSampler, l_TexCoord);
                sample *= l_IlluminationDecay * g_SWeight;
                
                l_Color += sample;
                l_IlluminationDecay *= g_SDecay;
            }
        }
    }
    
    return float4(l_Color.xyz * g_SExposure, 1.0);
}

technique MergeTechnique
{
    pass p0
    {
        PixelShader = compile ps_3_0 Merge();
    }
}

technique ScatteredLightTechnique
{
    pass p0
    {
        PixelShader = compile ps_3_0 ScatterLight();
    }
}
