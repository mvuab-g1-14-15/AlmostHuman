texture tex0 < string name = "tiger.bmp"; >;

float3 lightDir
<
    string UIDirectional = "Light Direction";
> = {0.577, -0.577, 0.577};

string XFile = "tiger.x";   // Model to load

DWORD  BCLR = 0xff202080;  // Background color (if no image)

float4 TintColor={1.0f,0.0f,0.0f,1.0f};

// TODO: Crear una matriu de tipus float4x4 que t� com a sem�ntica WORLDVIEWPROJECTION

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
    // TODO: Multiplicar la posici� d'entrada per la matriu WorldViewProj per tenir el v�rtex transformat
    // TODO: Passar les coordenades de textura d'entrada a la variable de sortida
    // TODO: Retorna OUT
}

float4 RenderNormalsPS(PNormalVertex IN) : COLOR
{
    // TODO: Retornar el color dif�s de la textura segons el m�tode tex2D i les coordenades de textura multiplicat pel color de tintat (TintColor)
}

technique tec0
{
    // TODO: Crear una passada 
    // TODO: Crear un v�rtex shader 1.1 cridant a RenderNormalVS passant-li la matriu de WorldViewProj
    // TODO: Crear un pixel shader 1.1 cridant a RenderNormalPS
}