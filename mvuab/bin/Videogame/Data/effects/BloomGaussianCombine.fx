#include "vertex_types.fxh"
#include "samplers.fxh"
#include "globals.fxh"

//more info --> https://github.com/zayka/LD26_Zayka/blob/master/LD26_Zayka/LD26_ZaykaContent/GlowEffect.fx
//more info --> http://www.gamedev.net/topic/660395-monogamehlsl-seperate-texture-from-sampler/

struct VertexShaderInput
{
    float3 Position : POSITION0;
	float2 TexCoord : TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
	float2 TexCoord : TEXCOORD0;
};

float2		HalfPixel		: HALF_PIXEL;
Texture	SceneTexture	: SCENE_TEXTURE;

sampler SceneTextureSampler = sampler_state 
{
	Texture		= < SceneTexture >;
	MagFilter	= Linear;
	MinFilter	= Linear;
	MipFilter	= Linear;
	AddressU	= Clamp;
	AddressV	= Clamp;
};

//////////////////////
//	Functions		//
//////////////////////
VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output=(VertexShaderOutput) 0;
	
	output.Position = float4(input.Position, 1.0f);
	output.TexCoord = input.TexCoord - HalfPixel;

	return output;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////
//				Extract Bloom				 //
///////////////////////////////////////////////

//////////////////////
//	Variables		//
//////////////////////
float		BloomThreshold	: BLOOM_THRESHOLD;

//////////////////////
//	Functions		//
//////////////////////
float4 FilterPS(float2 texCoord : TEXCOORD0) : COLOR0
{
    // Look up the original image color.
    float4 c = tex2D(SceneTextureSampler, texCoord);

    // Adjust it to keep only values brighter than the specified threshold.
    return saturate((c - BloomThreshold) / (1 - BloomThreshold));
}

//////////////////////////////////END EXTRACT BLOOM////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////
//				Gaussian Blur				 //
///////////////////////////////////////////////

#define SAMPLE_COUNT 15

float2		SampleOffsets[SAMPLE_COUNT]		: SAMPLE_OFFSETS;
float		SampleWeights[SAMPLE_COUNT]		: SAMPLE_WEIGHTS;

texture	GaussianBlurTexture				: GAUSSIAN_TEXTURE;

sampler GaussianBlurTextureSampler = sampler_state 
{
	Texture		= < GaussianBlurTexture >;
	MagFilter	= Linear;
	MinFilter	= Linear;
	MipFilter	= Linear;
	AddressU	= Clamp;
	AddressV	= Clamp;
};

//////////////////////
//	Functions		//
//////////////////////

float4 BlurPS(float2 texCoord : TEXCOORD0) : COLOR0
{
    float4 color = (float4)0;
    
    for (int i = 0; i < SAMPLE_COUNT; ++i)
    // Combine a number of weighted image filter taps.
    {
        color += (tex2D(GaussianBlurTextureSampler, texCoord + SampleOffsets[i]) * SampleWeights[i]);
    }
    
    return color;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////
//				Combine Bloom				 //
///////////////////////////////////////////////

//////////////////////
//	Variables		//
//////////////////////
float		BloomIntensity		: BLOOM_INTENSITY;
float		BaseIntensity		: BASE_INTENSITY;
float		BloomSaturation		: BLOOM_SATURATION;
float		BaseSaturation		: BASE_SATURATION;

texture	PostBloomTexture	: POST_BLOOM_TEXTURE;

sampler PostBloomTextureSampler = sampler_state 
{
	Texture		= < PostBloomTexture >;
	MagFilter	= Linear;
	MinFilter	= Linear;
	MipFilter	= Linear;
	AddressU	= Clamp;
	AddressV	= Clamp;
};

//////////////////////
//	Functions		//
//////////////////////

// Helper for modifying the saturation of a color.
float4 AdjustSaturation(float4 color, float saturation)
{
    // The constants 0.3, 0.59, and 0.11 are chosen because the
    // human eye is more sensitive to green light, and less to blue.
    float grey = dot(color, float3(0.3, 0.59, 0.11));

    return lerp(grey, color, saturation);
}

float4 CombinePS(float2 texCoord : TEXCOORD0) : COLOR0
{
    // Look up the bloom and original base image colors.
    float4 newBloom = tex2D(PostBloomTextureSampler, texCoord);
    float4 newBase = tex2D(SceneTextureSampler, texCoord);
    
    // Adjust color saturation and intensity.
    newBloom = AdjustSaturation(newBloom, BloomSaturation) * BloomIntensity;
    newBase = AdjustSaturation(newBase, BaseSaturation) * BaseIntensity;
    
    // Darken down the base image in areas where there is a lot of bloom,
    // to prevent things looking excessively burned-out.
    newBase *= (1 - saturate(newBloom));
    
    // Combine the two images.
    return newBase + newBloom;
}

///////////////////////////////////////////////
//				Techniques					 //
///////////////////////////////////////////////
technique BloomFilter
{
    pass Pass1
    {
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader = compile ps_3_0 FilterPS();
    }
}

technique GaussianBlur
{
    pass Pass1
    {
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader = compile ps_3_0 BlurPS();
    }
}

technique BloomCombine
{
    pass Pass1
    {
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader = compile ps_3_0 CombinePS();
    }
}