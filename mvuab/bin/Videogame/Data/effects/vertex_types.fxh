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