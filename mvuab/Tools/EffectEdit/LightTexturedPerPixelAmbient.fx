texture tex0 < string name = "tiger.bmp"; >;

float3 lightDir
<
    string UIDirectional = "Light Direction";
> = {0.577, -0.577, 0.577};

string XFile = "tiger.x";   // Model to load

DWORD  BCLR = 0xff202080;  // Background color (if no image)

// TODO: Crear una matriu de tipus float4x4 que té com a semàntica WORLDVIEWPROJECTION
// TODO: Crear una matriu de tipus float4x4 que té com a semàntica WORLD

// TODO: Crear una variable de tipus float4 amb nom LightAmbient que serà el valor contant de llum ambient possant-li com a valors inicials {0.25,0.25,0.25,1}

// TODO: Crear un sampler que agafi la textura tex0 i amb filtres LINEAR

struct VNormalVertex 
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

struct PNormalVertex
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 lightDir : TEXCOORD2;
};

PNormalVertex RenderNormalsVS(
    VNormalVertex IN,
    uniform float4x4 worldViewProjMatrix,
    uniform float4x4 worldMatrix)
{
    // TODO: Inicialitzar l'estructura OUT de tipus PNormalVertex que retornarem a 0
    // TODO: Multiplicar la posició d'entrada per la matriu WorldViewProj per tenir el vèrtex transformat
    // TODO: Passar les coordenades de textura d'entrada a la variable de sortida    
    // TODO: Transformar la normal a normal de World, multiplicant la IN.normal per la matriu de WORLD
    // TODO: Normalitzar el vector -lightDir i passar-li a l'atribut lightDir de l'estructura PNormalVertex
    // TODO: Retorna OUT
}

float4 RenderNormalsPS20(PNormalVertex IN) : COLOR
{
    // TODO: Crear una variable lightEffect de tipus float4 que tindrà el valor de la iluminació 
	// TODO: Fer la normalització de la normal utilitzant el mètode normalize hlsl
    // TODO: Fer el dot de la normal amb lightDir a nivell de pixel
    // hacemos un max con 0 para los valores negativos del acos del dot
    // Aquesta línia de sota s'ha de descomentar
    //lightEffect=max(0,lightEffect);
    // TODO: Retornar el difús de la textura per (valor de la iluminació més lightAmbient)
}

technique tec0
{
    // TODO: Crear una passada 
    // TODO: Crear un vèrtex shader 1.1 cridant a RenderNormalVS passant-li la matriu de WorldViewProj i la matriu de WorldMatrix
    // TODO: Crear un pixel shader 2.0 cridant a RenderNormalPS20
}