// http://fabiensanglard.net/lightScattering/
// http://http.developer.nvidia.com/GPUGems3/gpugems3_ch13.html
// https://medium.com/community-play-3d/god-rays-whats-that-5a67f26aeac2

#include "../vertex_types.fxh"
#include "../samplers.fxh"
#include "../globals.fxh"

float4 ScatterLight(in float2 UV : TEXCOORD0) : COLOR
{
    float2 l_DeltaTexCoord = UV - float2(g_TextureWidth, g_TextureHeight);
    float4 l_Color = float4(0, 0, 0, 0);
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
    
    return float4(l_Color.xyz * g_SExposure, 1.0);
}

technique ScatteredLightTechnique
{
    pass p0
    {
        PixelShader = compile ps_3_0 ScatterLight();
    }
}
