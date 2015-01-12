texture tex0 < string name = "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Samples\\Media\\tiger\\tiger.bmp"; >;

float4 lightDir
<
    string UIDirectional = "Light Direction";
> = {0.577, -0.577, 0.577, 1.0};

string XFile = "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Samples\\Media\\tiger\\tiger.x";   // Model to load load

DWORD  BCLR = 0xff202080;  // Background color (if no image)

float4x4 g_WorldViewProjectionMatrix   : WORLDVIEWPROJECTION;
float4x4 g_WorldMatrix                 : WORLD;
float4x4 g_ViewMatrix                  : INVERSE_VIEW;

float4 g_LightAmbient = float4(0.3, 0.35, 0.4, 1.0);
float4 g_LightColor   = float4(1.0, 0.6,  0.7, 1.0);

sampler DiffuseTextureSampler  = sampler_state
{ 
    Texture = (tex0);
    MipFilter = LINEAR; 
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};


struct VNormalVertex 
{
    float4 pos    : POSITION;
    float3 normal : NORMAL;
    float2 uv     : TEXCOORD0;
};


struct PNormalVertex
{
    float4 pos           : POSITION;
    float2 uv            : TEXCOORD0;
    float3 normal        : TEXCOORD1;
    float3 vertex2light  : TEXCOORD2;
    float3 vertex2camara : TEXCOORD3;
};

PNormalVertex RenderNormalsVS(VNormalVertex IN, uniform float4x4 WorldViewProj, uniform float4x4 worldMatrix)
{
    PNormalVertex OUT = (PNormalVertex )0;
    float3 l_WorldPos = mul(float4(IN.pos.xyz, 1.0), worldMatrix);

    OUT.uv             = IN.uv;
    OUT.pos            = mul(float4(IN.pos.xyz,1), WorldViewProj);
    OUT.normal         = normalize(mul(IN.normal, worldMatrix));

    OUT.vertex2light   = lightDir - l_WorldPos;
    OUT.vertex2camara  = g_ViewMatrix[3].xyz - l_WorldPos;

    return OUT;
}

float4 RenderNormalsPS20(PNormalVertex IN) : COLOR
{
    float3 l_Normal        = normalize(IN.normal);
    float3 l_Vertex2Light  = normalize(IN.vertex2light);
    float3 l_Vertex2Camara = normalize(IN.vertex2camara);

    float4 l_Diffuse  = tex2D(DiffuseTextureSampler, IN.uv) * max(dot(-l_Vertex2Light, l_Normal), 0.0);
    float4 l_Specular = pow(max(dot(l_Vertex2Camara, -reflect(l_Vertex2Light, l_Normal)), 0.0), 10.0);
    
    
    float l_Attenuation = 1.0 / (1.0 + 1.0 * pow(IN.vertex2light, 2.0));
    return l_Attenuation  * (l_Diffuse + l_Specular) ;
}

technique tec0
{
    pass p0
    {
        VertexShader = compile vs_3_0 RenderNormalsVS(g_WorldViewProjectionMatrix, g_WorldMatrix );
        PixelShader  = compile ps_3_0 RenderNormalsPS20();
    }
}