// http://es.wikipedia.org/wiki/Operador_Sobel

#include "globals.fxh"
#include "samplers.fxh"

float3x3 g_SobelX = float3x3
(
	float3(-1.0, -2.0, -1.0),
	float3( 0.0,  0.0,  0.0),
	float3( 1.0,  2.0,  1.0)
);

float3x3 g_SobelY = float3x3
(
	float3(1.0, 0.0, -1.0),
	float3(2.0, 0.0, -2.0),
	float3(1.0, 0.0, -1.0)
);

/*float3x3 g_SobelX = float3x3
(
	float3(-1.0, 0.0, 1.0),
	float3(-2.0, 0.0, 2.0),
	float3(-1.0, 0.0, 1.0)
);

float3x3 g_SobelY = float3x3
(
	float3( 1.0,  2.0,  1.0),
	float3( 0.0,  0.0,  0.0),
	float3(-1.0, -2.0, -1.0)
);*/

float getGray( float4 c )
{
	//return length(c);
    return dot( c.rgb, ( (0.33333).xxx) );
}

float4 SobelPS(in float2 UV : TEXCOORD0) : COLOR
{

	float2 xOffset	= float2( 1.0 / 800.0, 0.0f );
    float2 yOffset	= float2( 0.0f, 1.0 / 600.0 );
	
    float2 pp		= UV - yOffset;
    
	float4 cc;
	
	cc = tex2D( S0LinearSampler, pp - xOffset );	float g00 = getGray( cc );
	cc = tex2D( S0LinearSampler, pp );				float g01 = getGray( cc );
	cc = tex2D( S0LinearSampler, pp + xOffset );	float g02 = getGray( cc );
    
	pp = UV;
    
	cc = tex2D( S0LinearSampler, pp - xOffset );	float g10 = getGray( cc );
    cc = tex2D( S0LinearSampler, pp );				float g11 = getGray( cc );
    cc = tex2D( S0LinearSampler, pp + xOffset );	float g12 = getGray( cc );
    
	pp = UV + yOffset;

    cc = tex2D( S0LinearSampler, pp - xOffset );	float g20 = getGray( cc );
	cc = tex2D( S0LinearSampler, pp );				float g21 = getGray( cc );
	cc = tex2D( S0LinearSampler, pp + xOffset );	float g22 = getGray( cc );
    
	// Sobel filter kernel
	float K00	= -1;
    float K01	= -2;
    float K02	= -1;
    float K10	= 0;
    float K11	= 0;
    float K12	= 0;
    float K20	= 1;
    float K21	= 2;
    float K22	= 1;

    float Gx	= 0;
    float Gy	= 0;
    
	// Gx
	Gx += g00 * K00;
    Gx += g01 * K01;
    Gx += g02 * K02;
    Gx += g10 * K10;
    Gx += g11 * K11;
    Gx += g12 * K12;
    Gx += g20 * K20;
    Gx += g21 * K21;
    Gx += g22 * K22;
	 
	// Gy
    Gy += g00 * K00;
    Gy += g01 * K10;
    Gy += g02 * K20;
    Gy += g10 * K01;
    Gy += g11 * K11;
    Gy += g12 * K21;
    Gy += g20 * K02;
    Gy += g21 * K12;
    Gy += g22 * K22; 
    
	float norm = sqrt( Gx * Gx + Gy * Gy );
	
	/*if (norm > 0.7) return float4(0.0, 0.0, 0.0, 1.0);
	else return float4(1.0, 1.0, 1.0, 1.0);*/
    
	if (norm > 0.7) return GaussianBlur(S0LinearSampler, UV);
	else return tex2D(S0LinearSampler, UV);
	
	/*float4 lum = float4(0.30, 0.59, 0.11, 1);
 
	  // TOP ROW
	  float s11 = dot(tex2D(S0LinearSampler, UV + float2(-1.0f / 800.0f, -1.0f / 600.0f)), lum);   // LEFT
	  float s12 = dot(tex2D(S0LinearSampler, UV + float2(0, -1.0f / 600.0f)), lum);             // MIDDLE
	  float s13 = dot(tex2D(S0LinearSampler, UV + float2(1.0f / 800.0f, -1.0f / 600.0f)), lum);    // RIGHT
	 
	  // MIDDLE ROW
	  float s21 = dot(tex2D(S0LinearSampler, UV + float2(-1.0f / 800.0f, 0)), lum);                // LEFT
	  // Omit center
	  float s23 = dot(tex2D(S0LinearSampler, UV + float2(-1.0f / 800.0f, 0)), lum);                // RIGHT
	 
	  // LAST ROW
	  float s31 = dot(tex2D(S0LinearSampler, UV + float2(-1.0f / 800.0f, 1.0f / 600.0f)), lum);    // LEFT
	  float s32 = dot(tex2D(S0LinearSampler, UV + float2(0, 1.0f / 600.0f)), lum);              // MIDDLE
	  float s33 = dot(tex2D(S0LinearSampler, UV + float2(1.0f / 800.0f, 1.0f / 600.0f)), lum); // RIGHT
	 
	  // Filter ... thanks internet <span class="wp-smiley wp-emoji wp-emoji-smile" title=":)">:)</span>
	  float t1 = s13 + s33 + (2 * s23) - s11 - (2 * s21) - s31;
	  float t2 = s31 + (2 * s32) + s33 - s11 - (2 * s12) - s13;
	 
	  float4 col;
 
	  if (((t1 * t1) + (t2 * t2)) > 0.05) { col = float4(0,0,0,1); } 
	  else { col = float4(1,1,1,1); }
	 
	  return col;
	  */
	/*
	float3x3 l_I;
	float2 l_UVx = float2(1 / 800, 0);
	float2 l_UVy = float2(0, 1 / 600);
	
	//	A B C
	//	D E F
	//  G H I
	
	float3 A = tex2D(S0LinearSampler, UV - l_UVx + l_UVy).xyz;
	float3 B = tex2D(S0LinearSampler, UV + l_UVy).xyz;
	float3 C = tex2D(S0LinearSampler, UV + l_UVx + l_UVy).xyz;

	
	float3 D = tex2D(S0LinearSampler, UV - l_UVx).xyz;	
	float3 E = tex2D(S0LinearSampler, UV).xyz;
	float3 F = tex2D(S0LinearSampler, UV + l_UVx).xyz;
	
	float3 G = tex2D(S0LinearSampler, UV - l_UVx - l_UVy).xyz;	
	float3 H = tex2D(S0LinearSampler, UV - l_UVy).xyz;
	float3 I = tex2D(S0LinearSampler, UV + l_UVx - l_UVy).xyz;
	
	for (int i = 0; i < 3; i++)	for(int j = 0; j < 3; j++)
	{
		l_I[i][j] = length(tex2D(S0LinearSampler, UV + float2(i * 1/800  - 1/800, j * 1 / 600 - 1 / 600)).xyz);
	}
	
	float l_SobelX = dot(g_SobelX[0], l_I[0]) + dot(g_SobelX[1], l_I[1]) + dot(g_SobelX[2], l_I[2]);
	float l_SobelY = dot(g_SobelY[0], l_I[0]) + dot(g_SobelY[1], l_I[1]) + dot(g_SobelY[2], l_I[2]);
	
	float s = sqrt(l_SobelX * l_SobelX + l_SobelY * l_SobelY);
	float a = (s > 0.0001)? atan2(l_SobelY, l_SobelX) : 0.0;
	
	return float4(a, s, 0.0, 0.0);
	*/
}


technique SobelTechnique
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		
		PixelShader = compile ps_3_0 SobelPS();
	}
}