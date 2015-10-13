// http://fabiensanglard.net/lightScattering/
// http://http.developer.nvidia.com/GPUGems3/gpugems3_ch13.html
// https://medium.com/community-play-3d/god-rays-whats-that-5a67f26aeac2

#include "../samplers.fxh"
#include "../globals.fxh"

float4 mainPS(in float2 UV : TEXCOORD0) : COLOR
{
    float4 l_Color = float4(0.4, 0.4, 0.4, 1.0);
    float4 l_LPos  = float4(112.0, -5.0, -34.68, 1.0);
    
    return tex2D(S0LinearSampler, UV);
     
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

float4 DrawLight(in float2 UV : TEXCOORD0) : COLOR0
{
	float aspect = (float)g_TextureWidth / (float)g_TextureHeight;

	float distanceX = abs(UV.x - g_LightPos.x) * aspect;
	float distanceY = abs(UV.y - g_LightPos.y);

	float distance = length(float2(distanceX,distanceY));
	float4 Color = float4(0,0,0,0);
	float SunRadius = 0.017f;

	if((distance) <= SunRadius)
	{
		if(distance <= SunRadius * 0.8)
		{
			Color = float4(1, 1, 1, 1) * 0.68f;
		}
		else
		{
			float t = (SunRadius - distance) / (SunRadius * 0.2f);
			Color = float4(1, 1, 1, 1) * t * 0.68f;
		}
	}
	else
	{
		discard;
	}

	return Color;
}

technique ScatteredLightTechnique
{
    pass p0
    {
        //PixelShader = compile ps_3_0 mainPS();
        PixelShader = compile ps_3_0 DrawLight();
    }
}
