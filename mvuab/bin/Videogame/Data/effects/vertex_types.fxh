#if !defined( VERTEX_TYPES_FXH )
#define VERTEX_TYPES_FXH 

struct TNORMAL_COLOR {
    float3 Position     : POSITION;
    float3 Normal       : NORMAL;
    float4 Color        : COLOR;
};

struct TNORMAL_TAN_BI_T2_DIFF_VERTEX {
	float3 	Position	: POSITION;
    float3 	Normal 		: NORMAL;
    float3 	Tangent 	: TANGENT0;
    float3 	Binormal 	: BINORMAL0;
    float4 	Color 		: COLOR0;
	float2 	UV 			: TEXCOORD0;
    float2 	UV2 		: TEXCOORD1;
};

struct TNORMAL_TAN_BI_T1_DIFF_VERTEX {
	float3 	Position	: POSITION;
    float3 	Normal 		: NORMAL;
    float3 	Tangent 	: TANGENT0;
    float3 	Binormal 	: BINORMAL0;
    float4 	Color 		: COLOR0;
	float2 	UV 			: TEXCOORD0;
};    

struct TNORMAL_TAN_BI_T2_VERTEX {
	float3 	Position	: POSITION;
    float3 	Normal 		: NORMAL;
    float3 	Tangent 	: TANGENT0;
    float3 	Binormal 	: BINORMAL0;
	float2 	UV 			: TEXCOORD0;
    float2 	UV2 		: TEXCOORD1;
};

struct TNORMAL_TAN_BI_T1_VERTEX {
	float3 	Position	: POSITION;
    float3 	Normal 		: NORMAL;
    float3 	Tangent 	: TANGENT0;
    float3 	Binormal 	: BINORMAL0;
	float2 	UV 			: TEXCOORD0;
};

struct TNORMAL_T2_DIFF_VERTEX {
	float3 	Position	: POSITION;
    float3 	Normal 		: NORMAL;
    float4 	Color 		: COLOR0;
	float2 	UV 			: TEXCOORD0;
    float2 	UV2 		: TEXCOORD1;
};

struct TNORMAL_T1_DIFF_VERTEX {
	float3 	Position	: POSITION;
    float3 	Normal 		: NORMAL;
    float4 	Color 		: COLOR0;
	float2 	UV 			: TEXCOORD0;
};

struct TNORMAL_DIFF_VERTEX {
	float3 	Position	: POSITION;
    float3 	Normal 		: NORMAL;
    float4 	Color 		: COLOR0;
};

struct TNORMAL_T2_VERTEX {
	float3 	Position	: POSITION;
    float3 	Normal 		: NORMAL;
	float2 	UV 			: TEXCOORD0;
    float2 	UV2 		: TEXCOORD1;
};

struct TNORMAL_T1_VERTEX {
	float3 	Position	: POSITION;
    float3 	Normal 		: NORMAL;
	float2 	UV 			: TEXCOORD0;
};

struct TT2_DIFF_VERTEX {
	float3 	Position	: POSITION;
    float4 	Color 		: COLOR0;
	float2 	UV 			: TEXCOORD0;
    float2 	UV2 		: TEXCOORD1;
};

struct TT1_DIFF_VERTEX {
	float3 	Position	: POSITION;
    float4 	Color 		: COLOR0;
	float2 	UV 			: TEXCOORD0;
};

struct TDIFF_VERTEX {
	float3 	Position	: POSITION;
    float4 	Color 		: COLOR0;
};

struct TT2_VERTEX {
	float3 	Position	: POSITION;
	float2 	UV 			: TEXCOORD0;
    float2 	UV2 		: TEXCOORD1;
};

struct TT1_VERTEX {
	float3 	Position	: POSITION;
	float2 	UV 			: TEXCOORD0;
};

struct CAL3D_HW_VERTEX {
	float3 	Position	: POSITION;
	float4 	Weights 	: WEIGHTS;
	float4 	Indices		: INDICES;
	//float4 	Normal 		: NORMAL;
    //float4 	Tangent 	: TANGENT0;
    //float4 	Binormal 	: BINORMAL0;
    float3 	Normal 		: NORMAL;
	float2 	UV 			: TEXCOORD0;
};

struct TMultiRenderTargetPixel
{
	float4 Albedo 	: COLOR0; //Albedo (float3) + (float) SpecularFactor
	float4 Ambient  : COLOR1; //AmbientLight (float3) + (float) SpecularPow
	float4 Normal   : COLOR2; //Normal (float3) + (float) Not used
	float4 Depth    : COLOR3; //Depth (float4)
};

struct TNORMAL_TAN_BI_T1_VERTEX_PS
{
    float4 HPosition 		: POSITION;
	float3 Normal 			: NORMAL;
    float2 UV 				: TEXCOORD0;
	float4 WorldPosition 	: TEXCOORD1;
	float4 WorldTangent		: TEXCOORD2;
    float4 WorldBinormal 	: TEXCOORD3;
};

struct TNORMAL_T1_VERTEX_PS
{
    float4 HPosition 		: POSITION;
	float3 Normal 			: NORMAL;
	float2 UV 				: TEXCOORD0;
	float4 WorldPosition 	: TEXCOORD1;
	float3 WorldNormal 		: TEXCOORD2;
};

struct TNORMAL_T2_VERTEX_PS {
    float4 HPosition 		: POSITION;
	float3 Normal 			: NORMAL;
    float2 UV 				: TEXCOORD0;
	float2 UV2 				: TEXCOORD1;
	float3 WorldPosition 	: TEXCOORD2;
};

struct UBER_VERTEX_VS {
	float3 	Position	: POSITION;
    float3 	Normal 		: NORMAL;
#if defined( USE_DIFFUSE_COLOR )
	float4 Color		: COLOR;
#endif
	float2 	UV 			: TEXCOORD0;
#if defined( USE_NORMAL )
	float3 	Tangent 	: TANGENT0;
    float3 	Binormal 	: BINORMAL0;
#endif
#if defined( USE_SELF_ILUM )
	float2 UV2 		    : TEXCOORD1;
#endif
};

struct UBER_VERTEX_PS
{
    float4 HPosition 		: POSITION;
	float3 Normal 			: NORMAL;
	float2 UV 				: TEXCOORD0;
	float4 WorldPosition 	: TEXCOORD1;
	float3 WorldNormal 		: TEXCOORD2;
	
#if defined( USE_NORMAL )
	float4 WorldTangent		: TEXCOORD3;
    float4 WorldBinormal 	: TEXCOORD4;
	#if defined( USE_SELF_ILUM )
		float2 UV2 			: TEXCOORD5;
	#endif
#elif defined( USE_SELF_ILUM )
	float2 UV2 				: TEXCOORD3;
#elif defined( USE_DIFFUSE_COLOR )
	float4 Color			: TEXCOORD3;
#endif
};

#endif // !defined( VERTEX_TYPES_FXH )