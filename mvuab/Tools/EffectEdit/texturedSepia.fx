texture tex0 < string name = "tiger.bmp"; >;
texture tex1 < string name = "sepia.tga"; >;

float3 lightDir
<
    string UIDirectional = "Light Direction";
> = {0.577, -0.577, 0.577};

string XFile = "tiger.x";   // Model to load

DWORD  BCLR = 0xff202080;  // Background color (if no image)

// TODO: Crear una matriu de tipus float4x4 que té com a semàntica WORLDVIEWPROJECTION

// TODO: Crear un sampler que agafi la textura tex0 i amb filtres LINEAR
// TODO: Crear un sampler que agafi la textura sepia tex1 i amb filtres LINEAR 

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
    // TODO: Multiplicar la posició d'entrada per la matriu WorldViewProj per tenir el vèrtex transformat
    // TODO: Passar les coordenades de textura d'entrada a la variable de sortida
    // TODO: Retorna OUT
}

float4 RenderNormalsPS(PNormalVertex IN) : COLOR
{
    // TODO: Crear una variable float4 (OUT) on agafem el color difús de la textura mitjançant tex2D
    // TODO: Calculem la mitja aritmètica del color de la textura (OUT.x+OUT.y+OUT.z)/3.0f
    // TODO: Utilitzem el valor calculat anteriorment per agafar de la textura el color mitjançant la funció HLSL tex2D i ho retornem com a color
}

technique tec0
{
    // TODO: Crear una passada 
    // TODO: Crear un vèrtex shader 1.1 cridant a RenderNormalVS passant-li la matriu de WorldViewProj
    // TODO: Crear un pixel shader 1.4 cridant a RenderNormalPS
}