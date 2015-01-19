#define MAX_LIGHTS_BY_SHADER 4

float4x4 g_WorldViewProj 	 		: WorldViewProjection;
float4x4 g_ViewInverseMatrix 		: ViewInverse;

float4x4 g_ProjectionMatrix	 		: Projection;
float4x4 g_WorldMatrix 		 		: World;
float4x4 g_ViewMatrix		 		: View;

float4x4 g_ShadowProjectionMatrix	: ShadowProjection;
float4x4 g_LightViewMatrix 		 	: LightView;

int 		g_LightsType[MAX_LIGHTS_BY_SHADER]						: LightType;
bool		g_LightsEnabled[MAX_LIGHTS_BY_SHADER]					: LightEnabled;

float 		g_LightsAngle[MAX_LIGHTS_BY_SHADER]						: LightsAngle;
float 		g_LightsFallOff[MAX_LIGHTS_BY_SHADER]					: LightsFallOff;
float 		g_LightsEndRangeAttenuation[MAX_LIGHTS_BY_SHADER]		: LightsEndRangeAttenuation;
float 		g_LightsStartRangeAttenuation[MAX_LIGHTS_BY_SHADER]		: LightsStartRangeAttenuation;

float3	 	g_LightsColor[MAX_LIGHTS_BY_SHADER]						: LightsColor;
float3	 	g_LightsPosition[MAX_LIGHTS_BY_SHADER]					: LightsPosition;
float3	 	g_LightsDirection[MAX_LIGHTS_BY_SHADER]					: LightsDirection;
