texture tex0 < string name = "tiger.bmp"; >;

string XFile = "tiger.x";   // Model to load

DWORD  BCLR = 0xff202080;  // Background color (if no image)

// Creem una matriu de tipus float4x4 que té com a semàntica WORLDVIEWPROJECTION
float4x4 wvpMatrix : WORLDVIEWPROJECTION;

// TODO: Creem un sampler que agafa la textura tex0 i amb filtres LINEAR
sampler DiffuseTextureSampler  = sampler_state
{ 
    Texture = (tex0);
    MipFilter = LINEAR; 
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};


struct VNormalVertex 
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD0;
};


struct PNormalVertex
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

PNormalVertex RenderNormalsVS(
    VNormalVertex IN,
    uniform float4x4 WorldViewProj)
{
    // Inicialitzem l'estructura OUT de tipus PNormalVertex que retornarem a 0
    PNormalVertex OUT = (PNormalVertex )0;
    // Multipliquem la posició d'entrada per la matriu WorldViewProj per tenir el vèrtex transformat
    OUT.pos=mul(float4(IN.pos,1),WorldViewProj);
    // Passem les coordenades de textura d'entrada a la variable de sortida
    OUT.uv=IN.uv;
    // Retornem OUT
    return OUT;
}

float4 RenderNormalsPS(PNormalVertex IN) : COLOR
{
    // TODO: Retornem el difús de la textura
    return tex2D(DiffuseTextureSampler,IN.uv);
}

technique tec0
{
    // Creem una passada 
    pass p0
    {
        // Creem un vèrtex shader 1.1 cridant a RenderNormalVS passant-li la matriu de WorldViewProj
        VertexShader =compile vs_1_1 RenderNormalsVS(wvpMatrix);
        // Creem un pixel shader 1.1 cridant a RenderNormalPS
        PixelShader = compile ps_1_1 RenderNormalsPS();
    }
}