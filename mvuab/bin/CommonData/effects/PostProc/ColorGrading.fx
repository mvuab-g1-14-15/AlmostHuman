// https://docs.unrealengine.com/latest/INT/Engine/Rendering/PostProcessEffects/ColorGrading/index.html
// http://gamedev.stackexchange.com/questions/61738/color-grading-shaders-and-3d-texture
// http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter24.html
// http://kpulv.com/359/Dev_Log__Color_Grading_Shader/
// https://udn.epicgames.com/Three/ColorGrading.html

#include "../globals.fxh"
#include "../samplers.fxh"

float4 sampleAs3DTexture(sampler2D tex, float3 uv, float width) 
{
    float innerWidth = width - 1.0;
    
    float sliceSize = 1.0 / width; // space of 1 slice
    float slicePixelSize = sliceSize / width; // space of 1 pixel
    float sliceInnerSize = slicePixelSize * innerWidth; // space of width pixels
    
    float zSlice0 = min(floor(uv.z * innerWidth), innerWidth);
    float zSlice1 = min(zSlice0 + 1.0, innerWidth);
    
    float xOffset = slicePixelSize * 0.5 + uv.x * sliceInnerSize;
    float s0 = xOffset + (zSlice0 * sliceSize);
    float s1 = xOffset + (zSlice1 * sliceSize);
    
    float yPixelSize = sliceSize;
    float yOffset = yPixelSize * 0.5 + uv.y * (1.0 - yPixelSize);
    
    float4 slice0Color = tex2D(tex, float2(s0, yOffset));
    float4 slice1Color = tex2D(tex, float2(s1, yOffset));
    
    float zOffset = frac(uv.z * innerWidth);
    float4 result = lerp(slice0Color, slice1Color, zOffset);
    
    return result;
} 

float4 ColorGrading(in float2 UV : TEXCOORD0) : COLOR
{
	//return float4(1.0, 0.0, 0.0, 1.0);
	
	return tex2D(S1PointSampler, UV);
	
    //UV.xy = (UV.xy * 15.0f/16.0f) + (0.5f/16.0f);
    
    float4 l_Color = tex2D(S0PointSampler, UV);
    float4 l_GradeColor = sampleAs3DTexture(S1PointSampler, l_Color.rgb, 16);
    
    l_GradeColor.a = l_Color.a;
    return l_GradeColor;
}

technique ColorGradingTechnique
{
	pass p0
	{		
		PixelShader = compile ps_3_0 ColorGrading();
	}
}
