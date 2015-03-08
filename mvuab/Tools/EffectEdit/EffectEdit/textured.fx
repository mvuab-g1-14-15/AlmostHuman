texture tex0 < string name = "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Samples\\Media\\tiger\\tiger.bmp"; >;

float3 lightDir
<
    string UIDirectional = "Light Direction";
> = {0.577, -0.577, 0.577};

string XFile = "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Samples\\Media\\tiger\\tiger.x";   // Model to load

DWORD  BCLR = 0xff202080;  // Background color (if no image)

float4x4 g_WorldViewProjectionMatrix : WORLDVIEWPROJECTION;
float4x4 g_WorldMatrix : WORLD;

sampler DiffuseTextureSampler  = sampler_state
{ 
    Texture = (tex0);
    MipFilter = LINEAR; 
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};


struct VNormalVertex 
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};


struct PNormalVertex
{
    float4 pos : POSITION;
    float3 normal : TEXCOORD0;
    float2 uv : TEXCOORD1;
};

PNormalVertex RenderNormalsVS(
    VNormalVertex IN,
    uniform float4x4 WorldViewProj,
    uniform float4x4 worldMatrix)
{
    PNormalVertex OUT = (PNormalVertex )0;
    OUT.pos=mul(float4(IN.pos.xyz,1),WorldViewProj);
    OUT.uv=IN.uv;
    OUT.normal=mul(IN.normal,worldMatrix);


    return OUT;
}

float4 RenderNormalsPS20(PNormalVertex IN) : COLOR
{
    float4 OUT;

    return tex2D(DiffuseTextureSampler , IN.uv);
}

technique tec0
{
    pass p0
    {
        VertexShader =compile vs_1_1 RenderNormalsVS(g_WorldViewProjectionMatrix, g_WorldMatrix );
        PixelShader = compile ps_2_0 RenderNormalsPS20();
    }
}