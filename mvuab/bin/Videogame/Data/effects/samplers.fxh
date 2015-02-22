sampler S0PointSampler : register (s0)=sampler_state
{ 
	MipFilter = POINT; 
	MinFilter = POINT;
	MagFilter = POINT;
};

sampler S0LinearClampSampler : register (s0)=sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

sampler S1LinearClampSampler : register (s1)=sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

sampler S1PointSampler : register (s1)=sampler_state
{ 
	MipFilter = POINT; 
	MinFilter = POINT;
	MagFilter = POINT;
};
sampler S2PointSampler : register (s2)=sampler_state
{ 
	MipFilter = POINT; 
	MinFilter = POINT;
	MagFilter = POINT;
};
sampler S3PointSampler : register (s3)=sampler_state
{ 
	MipFilter = POINT; 
	MinFilter = POINT;
	MagFilter = POINT;
};
sampler S4PointSampler : register (s4)=sampler_state
{ 
	MipFilter = POINT; 
	MinFilter = POINT;
	MagFilter = POINT;
};
sampler S5PointSampler : register (s5)=sampler_state
{ 
	MipFilter = POINT; 
	MinFilter = POINT;
	MagFilter = POINT;
};
sampler S6PointSampler : register (s6)=sampler_state
{ 
	MipFilter = POINT; 
	MinFilter = POINT;
	MagFilter = POINT;
};
sampler S7PointSampler : register (s7)=sampler_state
{ 
	MipFilter = POINT; 
	MinFilter = POINT;
	MagFilter = POINT;
};
sampler S0LinearSampler : register (s0)=sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
sampler S1LinearSampler : register (s1)=sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
sampler S2LinearSampler : register (s2)=sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
sampler S3LinearSampler : register (s3)=sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
sampler S4LinearSampler : register (s4)=sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
sampler S5LinearSampler : register (s5)=sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
sampler S6LinearSampler : register (s6)=sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
sampler S7LinearSampler : register (s7)=sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
sampler S0AnisotropicSampler : register (s0)=sampler_state
{ 
	MipFilter = ANISOTROPIC; 
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};
sampler S1AnisotropicSampler : register (s1)=sampler_state
{ 
	MipFilter = ANISOTROPIC; 
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};
sampler S2AnisotropicSampler : register (s2)=sampler_state
{ 
	MipFilter = ANISOTROPIC; 
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};
sampler S3AnisotropicSampler : register (s3)=sampler_state
{ 
	MipFilter = ANISOTROPIC; 
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};
sampler S4AnisotropicSampler : register (s4)=sampler_state
{ 
	MipFilter = ANISOTROPIC; 
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};
sampler S5AnisotropicSampler : register (s5)=sampler_state
{ 
	MipFilter = ANISOTROPIC; 
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};
sampler S6AnisotropicSampler : register (s6)=sampler_state
{ 
	MipFilter = ANISOTROPIC; 
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};
sampler S7AnisotropicSampler : register (s7)=sampler_state
{ 
	MipFilter = ANISOTROPIC; 
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};
samplerCUBE S0CubePointSampler : register (s0)=sampler_state
{ 
	MipFilter = POINT; 
	MinFilter = POINT;
	MagFilter = POINT;
};
samplerCUBE S1CubePointSampler : register (s1)=sampler_state
{ 
	MipFilter = POINT; 
	MinFilter = POINT;
	MagFilter = POINT;
};
samplerCUBE S2CubePointSampler : register (s2)=sampler_state
{ 
	MipFilter = POINT; 
	MinFilter = POINT;
	MagFilter = POINT;
};
samplerCUBE S3CubePointSampler : register (s3)=sampler_state
{ 
	MipFilter = POINT; 
	MinFilter = POINT;
	MagFilter = POINT;
};
samplerCUBE S4CubePointSampler : register (s4)=sampler_state
{ 
	MipFilter = POINT; 
	MinFilter = POINT;
	MagFilter = POINT;
};
samplerCUBE S5CubePointSampler : register (s5)=sampler_state
{ 
	MipFilter = POINT; 
	MinFilter = POINT;
	MagFilter = POINT;
};
samplerCUBE S6CubePointSampler : register (s6)=sampler_state
{ 
	MipFilter = POINT; 
	MinFilter = POINT;
	MagFilter = POINT;
};
samplerCUBE S7CubePointSampler : register (s7)=sampler_state
{ 
	MipFilter = POINT; 
	MinFilter = POINT;
	MagFilter = POINT;
};
samplerCUBE S0CubeLinearSampler : register (s0)=sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
samplerCUBE S1CubeLinearSampler : register (s1)=sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
samplerCUBE S2CubeLinearSampler : register (s2)=sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
samplerCUBE S3CubeLinearSampler : register (s3)=sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
samplerCUBE S4CubeLinearSampler : register (s4)=sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
samplerCUBE S5CubeLinearSampler : register (s5)=sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
samplerCUBE S6CubeLinearSampler : register (s6)=sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
samplerCUBE S7CubeLinearSampler : register (s7)=sampler_state
{ 
	MipFilter = LINEAR; 
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
samplerCUBE S0CubeAnisotropicSampler : register (s0)=sampler_state
{ 
	MipFilter = ANISOTROPIC; 
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};
samplerCUBE S1CubeAnisotropicSampler : register (s1)=sampler_state
{ 
	MipFilter = ANISOTROPIC; 
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};
samplerCUBE S2CubeAnisotropicSampler : register (s2)=sampler_state
{ 
	MipFilter = ANISOTROPIC; 
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};
samplerCUBE S3CubeAnisotropicSampler : register (s3)=sampler_state
{ 
	MipFilter = ANISOTROPIC; 
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};
samplerCUBE S4CubeAnisotropicSampler : register (s4)=sampler_state
{ 
	MipFilter = ANISOTROPIC; 
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};
samplerCUBE S5CubeAnisotropicSampler : register (s5)=sampler_state
{ 
	MipFilter = ANISOTROPIC; 
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};
samplerCUBE S6CubeAnisotropicSampler : register (s6)=sampler_state
{ 
	MipFilter = ANISOTROPIC; 
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};
samplerCUBE S7CubeAnisotropicSampler : register (s7)=sampler_state
{ 
	MipFilter = ANISOTROPIC; 
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};

sampler ShadowMapTextureSampler : register( s5 ) = sampler_state
{
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};
sampler ShadowMapTextureDynamicSampler : register( s6 ) = sampler_state
{
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};