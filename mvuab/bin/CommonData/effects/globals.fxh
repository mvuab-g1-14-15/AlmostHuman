#if !defined( GLOBALS_FXH )
#define GLOBALS_FXH

#define MAX_LIGHTS_BY_SHADER 4
#define MAXBONES 29

#define OMNI_LIGHT  0
#define DIR_LIGHT   1
#define SPOT_LIGHT  2

#define SHADOW_EPSILON 0.001f

#define PI          3.14159265359
#define Deg2Rad(x)  (x * PI / 180.0)

//
// Shared Parameters 
//

shared float4x4 g_WorldViewProj                                            : WVPMatrix;
shared float4x4 g_ViewMatrix                                               : ViewMatrix;
shared float4x4 g_ViewInverseMatrix                                        : InverseViewMatrix;
shared float4x4 g_ProjectionInverseMatrix                                  : InverseProjectionMatrix;
shared float4x4 g_ProjectionMatrix                                         : ProjectionMatrix;
shared float4x4 g_WorldMatrix                                              : WorldMatrix;
shared float4x4 g_WorldInverseMatrix                                       : InverseWorldMatrix;
shared float4x4 g_WorldViewMatrix                                          : WVMatrix;
shared float4x4 g_ViewProjMatrix                                           : VPMatrix;
shared float3   g_CameraPosition                                           : CameraPosition;
shared float3   g_AmbientLight 											   : AmbientLight;
shared float    g_DeltaTime 											   : DeltaTime;
shared int      g_FBWidth    											   : FBWidth;
shared int      g_FBHeight       										   : FBHeight;


//
// Debug semantics
//
bool    g_UseDebugColor                                             : UseDebugColor;
float4  g_DebugColor                                                : DebugColor;

//
// Shadow map stuff
//
float4x4    g_ShadowProjectionMatrix                                : ShadowProjection;
float4x4    g_LightViewMatrix                                       : LightView;
float4x4    g_ViewToLightProjMatrix                                 : ViewToLightProjection;
bool 		g_UseShadowMaskTexture 								    : UseShadowMaskTexture;
bool 		g_UseShadowMapStatic 									: UseStaticShadowMap;
bool 		g_UseShadowMapDynamic 									: UseDynamicShadowMap;
int  		g_ShadowMapTextureSize									: ShadowMapTextureSize;

float	    g_Size													: Size;
int	        g_Angle													: Angle;
float	    g_Alpha													: Alpha;
float3	    g_Color													: Color;
float3	    g_Direction											    : Direction;

//
// Lights data
//
int         g_LightsType[MAX_LIGHTS_BY_SHADER]                      : LightsTypes;
bool        g_LightsEnabled[MAX_LIGHTS_BY_SHADER]                   : LightsEnabled;
float       g_LightsAngle[MAX_LIGHTS_BY_SHADER]                     : LightsAngles;
float       g_LightsIntensity[MAX_LIGHTS_BY_SHADER]                 : LightsIntensity;
float       g_LightsFallOff[MAX_LIGHTS_BY_SHADER]                   : LightsFallOffs;
float       g_LightsEndRangeAttenuation[MAX_LIGHTS_BY_SHADER]       : LightsEndAngle;
float       g_LightsStartRangeAttenuation[MAX_LIGHTS_BY_SHADER]     : LightsStartAngle;
float3      g_LightsColor[MAX_LIGHTS_BY_SHADER]                     : LightsColors;
float3      g_LightsPosition[MAX_LIGHTS_BY_SHADER]                  : LightsPositions;
float3      g_LightsDirection[MAX_LIGHTS_BY_SHADER]                 : LightsDirections;
float       g_SpecularExponent 	= 20.0f;
float       g_SpecularFactor 	= 20.0f;
//float4    g_AmbientLight 		= float4(0.35, 0.35, 0.35, 1.0);

float		g_LightProbes[105]										: LightProbes;

//
// Cal3D parameters
//
float3x4    g_Bones[MAXBONES]                                       : Bones;

float       g_Bump = 24.0;

//
// Fog data
//
bool		g_UseFog												: UseFog;
float       g_FogStar                                           	: FogStart;
float       g_FogEnd                                            	: FogEnd;
float       g_FodExp                                            	: FogExp;
int         g_FogFun                                            	: FogFun;

float       g_Weight[5]                                         	: Weight;
float       g_Offset[5]                                         	: Offset;

//
// 2D texture parameters
//
int         g_TextureWidth                                      	: TextureWidth;
int         g_TextureHeight                                     	: TextureHeight;

//
// Window size
//
int         g_WindowWidth                                      		: WindowWidth;
int         g_WindowHeight                                     		: WindowHeight;


float       g_Time													: Time;


//
// Functions
//
float DistanceAttenuation( int i, float3 aDistanceToLight )
{
    return 1.0 - saturate((aDistanceToLight-g_LightsStartRangeAttenuation[i])/(g_LightsEndRangeAttenuation[i]-g_LightsStartRangeAttenuation[i]));
}

float SpotAttenuation( int i, float3 LightToPixelDirection )
{   
    float3 l_LightDirection = normalize(g_LightsDirection[i]);
    return 1.0-saturate((cos(g_LightsAngle[i]/2)-dot(LightToPixelDirection,l_LightDirection))/(cos(g_LightsAngle[i]/2)-cos(g_LightsFallOff[i]/2)));
}

float3 Normal2Texture(float3 Normal)
{
    return Normal * 0.5 + 0.5;
}

float3 Texture2Normal(float3 Color)
{
    return (Color - 0.5) * 2.0;
}

float3 GetPositionFromZDepthViewInViewCoordinates(float ZDepthView, float2 UV, float4x4 InverseProjectionMatrix)
{
    float x = UV.x * 2.0 - 1.0;
    float y = (1.0 - UV.y) * 2.0 - 1.0;
    
    float4 l_ProjectedPos = float4(x, y, ZDepthView, 1.0);
    float4 l_PositionVS = mul(l_ProjectedPos, InverseProjectionMatrix);

    return l_PositionVS.xyz / l_PositionVS.w;
}

float3 GetPositionFromZDepthView(float ZDepthView, float2 UV, float4x4 InverseViewMatrix, float4x4 InverseProjectionMatrix)
{
    float3 l_PositionView=GetPositionFromZDepthViewInViewCoordinates(ZDepthView, UV, InverseProjectionMatrix);
    return mul(float4(l_PositionView,1.0), InverseViewMatrix).xyz;
}

float CalcAttenuation(float actual, float start, float end)
{
    return saturate((actual-start)/(end-start));
}

float4 CalcLinearFog(float Depth, float StartFog, float EndFog, float4 FogColor, float4 FragColor)
{
    float l_FogFactor = 1.0 - CalcAttenuation(Depth, StartFog, EndFog);
    return float4(l_FogFactor * FragColor.rgb + (1.0 - l_FogFactor) * FogColor.rgb, FragColor.a);
}

float4 CalcExpFog(float Depth, float dz, float4 FogColor, float4 FragColor)
{
    float l_FogFactor =  1.0 - saturate(1 - exp(-Depth * dz));
    return float4(l_FogFactor * FragColor.rgb + (1.0 - l_FogFactor) * FogColor.rgb, FragColor.a);
}

float4 CalcGroundFog(float4 FogColor, float4 FragColor, float depth, float3 rayOri, float3 rayDir)
{
    float a = 0.1, b = 0.1;
    float fogAmount = a * exp(-rayOri.y * b) * (1.0 - exp(-depth * rayDir.y * b)) / rayDir.y;
    return float4((1- fogAmount) * FragColor.rgb + fogAmount * FogColor.rgb, FragColor.a);
}

float3 CalcNewNormal(float3 Tn, float3 Bn, float4 Color, float3 Normal)
{
    float3 l_NewNormal = Normal;
    float3 l_Bump = g_Bump * (Color.rgb - float3(0.5, 0.5, 0.5));
    
    l_NewNormal = Normal + l_Bump.x * Tn + l_Bump.y * Bn;
    return normalize(l_NewNormal);
}
float3 CalcAnimtedPos(float4 Position, float4 Indices, float4 Weight)
{
	float3 l_Position = 0;
	
	l_Position  = mul(g_Bones[Indices.x], Position) * Weight.x;
	l_Position += mul(g_Bones[Indices.y], Position) * Weight.y;
	l_Position += mul(g_Bones[Indices.z], Position) * Weight.z;
	l_Position += mul(g_Bones[Indices.w], Position) * Weight.w;
	
	return l_Position;
}

void CalcAnimatedNormalTangent(float3 Normal, float3 Tangent, float4 Indices, float4 Weight, out float3 OutNormal, out float3 OutTangent)
{
	OutNormal  = 0;
	OutTangent = 0;
	float3x3 m;
	
	m[0].xyz = g_Bones[Indices.x][0].xyz;
	m[1].xyz = g_Bones[Indices.x][1].xyz;
	m[2].xyz = g_Bones[Indices.x][2].xyz;
	
	OutNormal  += mul(m, Normal.xyz)* Weight.x;
	OutTangent += mul(m, Tangent.xyz)* Weight.x;
	
	m[0].xyz = g_Bones[Indices.y][0].xyz;
	m[1].xyz = g_Bones[Indices.y][1].xyz;
	m[2].xyz = g_Bones[Indices.y][2].xyz;
	
	OutNormal  += mul(m, Normal.xyz)* Weight.y;
	OutTangent += mul(m, Tangent.xyz)* Weight.y;
	
	m[0].xyz = g_Bones[Indices.z][0].xyz;
	m[1].xyz = g_Bones[Indices.z][1].xyz;
	m[2].xyz = g_Bones[Indices.z][2].xyz;
	
	OutNormal += mul(m, Normal.xyz)* Weight.z;
	OutTangent += mul(m, Tangent.xyz)* Weight.z;
	
	m[0].xyz = g_Bones[Indices.w][0].xyz;
	m[1].xyz = g_Bones[Indices.w][1].xyz;
	m[2].xyz = g_Bones[Indices.w][2].xyz;
	
	OutNormal  += mul(m, Normal.xyz)* Weight.w;
	OutTangent += mul(m, Tangent.xyz)* Weight.w;
	
	OutNormal  = normalize(OutNormal);
	OutTangent = normalize(OutTangent);
}

// http://stackoverflow.com/questions/5149544/can-i-generate-a-random-number-inside-a-pixel-shader

float random( float2 p ) // Version 2
{
   // e^pi (Gelfond's constant)
   // 2^sqrt(2) (Gelfond–Schneider constant)
   float2 r = float2( 23.14069263277926, 2.665144142690225 );
 //return frac( cos( mod( 12345678., 256. * dot(p,r) ) ) ); // ver1
   return frac(cos(dot(p,r)) * 123456.); // ver2
}

#endif // !defined( GLOBALS_FXH )