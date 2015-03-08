texture tex0 < string name = "tiger.bmp"; >;

string XFile = "tiger.x";   // Model to load

DWORD  BCLR = 0xff202080;  // Background color (if no image)

// TODO: Crear una matriu de tipus float4x4 que té com a semàntica WORLDVIEWPROJECTION

// TODO: Crear un sampler que agafi la textura tex0 i amb filtres LINEAR

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
    // TODO: Inicialitzar l'estructura OUT de tipus PNormalVertex que retornarem a 0
    // TODO: Dividir la component x del IN.pos entre un número suficientment gran (per exemple 1000.0f) per xafar l'objecte sobre el pla YZ
    // TODO: Multiplicar la posició d'entrada per la matriu WorldViewProj per tenir el vèrtex transformat
    // TODO: Passar les coordenades de textura d'entrada a la variable de sortida

    // TODO: Retorna OUT
}

float4 RenderNormalsPS(PNormalVertex IN) : COLOR
{
    // TODO: Retornar el difús de la textura utilitzant la funcio HLSL tex2D
}

technique tec0
{
    // TODO: Crear una passada 
    // TODO: Crear un vèrtex shader 1.1 cridant a RenderNormalVS passant-li la matriu de WorldViewProj
    // TODO: Crear un pixel shader 1.1 cridant a RenderNormalPS
}