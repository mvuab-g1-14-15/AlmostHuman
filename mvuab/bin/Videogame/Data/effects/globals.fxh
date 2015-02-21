#define MAX_LIGHTS_BY_SHADER 4
#define MAXBONES 29

#define OMNI_LIGHT  0
#define DIR_LIGHT   1
#define SPOT_LIGHT  2

#define SHADOW_EPSILON 0.00005f

#define PI			3.14159265359
#define Deg2Rad(x)	(x * PI / 180.0)

float4x4 g_WorldViewProj 	 										: WorldViewProjection;
float4x4 g_ViewMatrix		 										: View;
float4x4 g_ViewInverseMatrix 										: ViewInverse;
float4x4 g_ProjectionInverseMatrix 									: ProjectionInverse;
float4x4 g_ProjectionMatrix	 										: Projection;
float4x4 g_WorldMatrix 		 										: World;
float4x4 g_WorldInverseMatrix 		 								: WorldInverse;
float4x4 g_WorldViewMatrix 		 								    : WorldView;
float4x4 g_ViewProjMatrix 		 								    : ViewProjection;

float4 g_DebugColor													: DebugColor;


float4x4 g_ShadowProjectionMatrix									: ShadowProjection;
float4x4 g_LightViewMatrix 		 									: LightView;
float4x4 g_ViewToLightProjMatrix 		 						    : ViewToLightProjection;

int 		g_LightsType[MAX_LIGHTS_BY_SHADER]						: LightsTypes;
bool		g_LightsEnabled[MAX_LIGHTS_BY_SHADER]					: LightsEnabled;

float 		g_LightsAngle[MAX_LIGHTS_BY_SHADER]						: LightsAngles;
float 		g_LightsFallOff[MAX_LIGHTS_BY_SHADER]					: LightsFallOffs;
float 		g_LightsEndRangeAttenuation[MAX_LIGHTS_BY_SHADER]		: LightsEndAngle;
float 		g_LightsStartRangeAttenuation[MAX_LIGHTS_BY_SHADER]		: LightsStartAngle;

float3	 	g_LightsColor[MAX_LIGHTS_BY_SHADER]						: LightsColors;
float3	 	g_LightsPosition[MAX_LIGHTS_BY_SHADER]					: LightsPositions;
float3	 	g_LightsDirection[MAX_LIGHTS_BY_SHADER]					: LightsDirections;

float3x4 	g_Bones[MAXBONES]										: Bones;
float3	    g_CameraPosition										: CameraPosition;
float 		g_DeltaTime												: DeltaTime;

float4	 	g_AmbientLight = float4(0.35, 0.35, 0.35, 1.0);
float 		g_SpecularExponent=100;

float 		g_Bump = 10.0;


// Functions

float DistanceAttenuation( int i, float3 LightToPixelDirection )
{
	float l_DistanceToLight = length(LightToPixelDirection);
	return 1.0 - saturate((l_DistanceToLight-g_LightsStartRangeAttenuation[i])/(g_LightsEndRangeAttenuation[i]-g_LightsStartRangeAttenuation[i]));
}

float SpotAttenuation( int i, float3 LightToPixelDirection )
{	
	float3 l_LightDirection = normalize(g_LightsDirection[i]);
    return 1.0-saturate((cos(g_LightsAngle[i])-dot(-LightToPixelDirection,l_LightDirection))/(cos(g_LightsAngle[i]/2)-cos(g_LightsFallOff[i]/2)));
}

float3 Normal2Texture(float3 Normal)
{
	return Normal*0.5+0.5;
}

float3 Texture2Normal(float3 Color)
{
	return (Color-0.5)*2;
}

float3 GetPositionFromZDepthViewInViewCoordinates(float ZDepthView, float2 UV )
{
	// Get the depth value for this pixel
	// Get x/w and y/w from the viewport position
	float x = UV.x * 2 - 1;
	float y = (1 - UV.y) * 2 - 1;
	float4 l_ProjectedPos = float4(x, y, ZDepthView, 1.0);
	// Transform by the inverse projection matrix
	float4 l_PositionVS = mul(l_ProjectedPos, g_ProjectionInverseMatrix);
	// Divide by w to get the view-space position
	return l_PositionVS.xyz / l_PositionVS.w;
}

float3 GetPositionFromZDepthView(float ZDepthView, float2 UV )
{
	float3 l_PositionView=GetPositionFromZDepthViewInViewCoordinates(ZDepthView, UV);
	return mul(float4(l_PositionView,1.0), g_ViewInverseMatrix).xyz;
}

float CalcAttenuation(float actual, float start, float end)
{
	return saturate((actual-start)/(end-start));
}

float4 CalcLinearFog(float Depth, float StartFog, float EndFog, float4 FogColor)
{
	return float4(FogColor.xyz, FogColor.a*(1.0-CalcAttenuation(Depth, StartFog, EndFog)));
}