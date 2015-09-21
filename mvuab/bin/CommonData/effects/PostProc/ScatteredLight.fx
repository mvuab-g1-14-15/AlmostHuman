// http://fabiensanglard.net/lightScattering/
// http://http.developer.nvidia.com/GPUGems3/gpugems3_ch13.html
// https://medium.com/community-play-3d/god-rays-whats-that-5a67f26aeac2

#include "../samplers.fxh"
#include "../globals.fxh"

#define NUM_SAMPLES 100

float g_SDecay = 0.96815;
float g_SExposure = 0.2;

float g_SDensity = 0.926;
float g_SWeight = 0.98767;

float4 mainPS(in float2 UV : TEXCOORD0) : COLOR
{
    float3 l_Color = float3(0.0, 0.0, 0.0);
    float4 l_LPos  = float4(66.7, -9.68, -70.95, 1.0);
     
    //for(int i = 0; i < MAX_LIGHTS_BY_SHADER; i++)
    {
		//if(g_LightsEnabled[i] == 1)
        {
            float4 l_LightScreenSpace = mul(l_LPos, g_WorldViewProj);
            float2 l_DeltaTexCoord = UV - l_LightScreenSpace.xy / l_LightScreenSpace.w;
            float2 l_TexCoord = UV;
            
            float l_IlluminationDecay = 1.0;
            l_DeltaTexCoord *= 1.0 / float(NUM_SAMPLES) * g_SDensity; 

            for(int i=0; i < NUM_SAMPLES ; i++)
            {
                l_TexCoord -= l_DeltaTexCoord;
                
                float4 sample = tex2D(S0LinearSampler, l_TexCoord );
                sample *= l_IlluminationDecay * g_SWeight;

                l_Color += sample;
                l_IlluminationDecay *= g_SDecay;
            }
        }
    }
    
    return float4(l_Color * g_SExposure, 1.0);
}

technique ScatteredLightTechnique
{
    pass p0
    {
        PixelShader = compile ps_3_0 mainPS();
    }
}
