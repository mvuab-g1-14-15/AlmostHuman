#define MAX_LIGHTS_BY_SHADER 4
#define MAXBONES 29

float4x4 g_WorldViewProj 	 										: WorldViewProjection;
float4x4 g_ViewMatrix		 										: View;
float4x4 g_ViewInverseMatrix 										: ViewInverse;
float4x4 g_ProjectionInverseMatrix 									: ProjectionInverse;
float4x4 g_ProjectionMatrix	 										: Projection;
float4x4 g_WorldMatrix 		 										: World;
float4x4 g_WorldInverseMatrix 		 								: WorldInverse;

float4x4 g_ShadowProjectionMatrix									: ShadowProjection;
float4x4 g_LightViewMatrix 		 									: LightView;

int 		g_LightsType[MAX_LIGHTS_BY_SHADER]						: LightsTypes;
bool		g_LightsEnabled[MAX_LIGHTS_BY_SHADER]					: LightsEnabled;

float 		g_LightsAngle[MAX_LIGHTS_BY_SHADER]						: LightsAngles;
float 		g_LightsFallOff[MAX_LIGHTS_BY_SHADER]					: LightsFallOffs;
float 		g_LightsEndRangeAttenuation[MAX_LIGHTS_BY_SHADER]		: LightsEndRangeAttenuations;
float 		g_LightsStartRangeAttenuation[MAX_LIGHTS_BY_SHADER]		: LightsStartRangeAttenuations;

float3	 	g_LightsColor[MAX_LIGHTS_BY_SHADER]						: LightsColors;
float3	 	g_LightsPosition[MAX_LIGHTS_BY_SHADER]					: LightsPositions;
float3	 	g_LightsDirection[MAX_LIGHTS_BY_SHADER]					: LightsDirections;

float3x4 	g_Bones[MAXBONES]										: Bones;
float3	    g_CameraPosition										: CameraPosition;
float 		g_DeltaTime												: DeltaTime;
