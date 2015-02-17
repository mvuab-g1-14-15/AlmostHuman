float g_ZBlurFocalStart = 50;
float g_ZBlurFocalEnd = 65;
float g_ZBlurEnd = 300;

float g_ConstantBlur = 0.4;

float3 GetPositionFromZDepthViewInViewCoordinates(float ZDepthView, float2 UV, float4x4 InverseProjectionMatrix)
{
	// Get the depth value for this pixel
	// Get x/w and y/w from the viewport position
	float x = UV.x * 2 - 1;
	float y = (1 - UV.y) * 2 - 1;
	float4 l_ProjectedPos = float4(x, y, ZDepthView, 1.0);

	// Transform by the inverse projection matrix
	float4 l_PositionVS = mul(l_ProjectedPos, InverseProjection atrix);

	// Divide by w to get the view-space position
	return l_PositionVS.xyz / l_PositionVS.w;
}

float3 GetPositionFromZDepthView(float ZDepthView, float2 UV, float4x4 InverseViewMatrix, float4x4 InverseProjectionMatrix)
{
	float3 l_PositionView = GetPositionFromZDepthViewInViewCoordinates(ZDepthView, UV, InverseProjectionMatrix);
	return mul(float4(l_PositionView, 1.0), InverseViewMatrix).xyz;
}

float4 ZBlurPS(in float2 UV : TEXCOORD0) : COLOR
{
	float4 l_Color = float4(0.0, 0.0, 0.0, 0.0);
	float4 l_DepthMap = tex2D(GUIZMapTextureSampler, UV);
	
	float3 l_CameraPosition = g_InverseViewMatrix[3].xyz;
	float3 l_WorldPosition = GetPositionFromZDepthView(l_DepthMap, float2(0.0, 0.0), g_InverseViewMatrix, g_InverseProjectionMatrix);
	
	float l_Distance = length(l_WorldPosition - l_CameraPosition);
	float l_Blur = 1.0;

	if(l_Distance < g_ZBlurFocalStart) l_Blur = max(l_Distance / g_ZBlurFocalStart, g_ConstantBlur);
	else if(l_Distance > g_ZBlurFocalEnd) l_Blur = max(1.0 - (l_Distance - g_ZBlurFocalEnd) / g_ZBlurEnd, g_ConstantBlur);
	
	const float2 delta[8] =
	{
		float2(-1,1),float2(1,-1),float2(-1,1),float2(1,1),
		float2(-1,0),float2(1,0),float2(0,-1),float2(0,1)
	};
	
	float2 l_PixelInc=4*1/g_RenderTargetSize; //4 pixeles a la redonda
	float4 l_AlbedoColor=tex2D(GUIDiffuseMapTextureSampler,UV);
	
	for( int i=0;i<8;i++ ) l_Color += tex2D(GUIDiffuseMapTextureSampler,UV + delta[i]*l_PixelInc)*(1-l_Blur)+l_Blur*l_AlbedoColor;
	l_Color = l_Color*(1.0/8.0);
	
	return l_Color;
}

technique ZBlurTechnique
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		
		PixelShader = compile ps_3_0 ZBlurPS();
	}
}