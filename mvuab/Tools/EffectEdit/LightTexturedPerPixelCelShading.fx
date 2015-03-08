texture tex0 < string name = "tiger.bmp"; >;
texture tex1 < string name = "celshade.tga"; >;

float3 lightDir
<
    string UIDirectional = "Light Direction";
> = {0.577, -0.577, 0.577};

string XFile = "tiger.x";   // Model to load

DWORD  BCLR = 0xff202080;  // Background color (if no image)

// TODO: Crear una matriu de tipus float4x4 que t� com a sem�ntica WORLDVIEWPROJECTION
// TODO: Crear una matriu de tipus float4x4 que t� com a sem�ntica WORLD

// TODO: Crear un sampler que agafi la textura tex0 i amb filtres LINEAR

// Creem el sampler de la textura de cellshading, podem utilitzar tant filtres LINEAR com filtres de POINT depenent de l'aparen�a que volguem donar-li
// Tamb� li fem clamp a la textura perque no filtri amb els cantons
sampler CelShadingTextureSampler  = sampler_state
{ 
    Texture = (tex1);
//Si ponemos filtros POINT veremos el corte m�x brusco, m�s de dibujo
    MipFilter = POINT;     
    MinFilter = POINT;
    MagFilter = POINT;
/*    MagFilter = LINEAR;
    MipFilter = LINEAR;     
    MinFilter = LINEAR;*/
    AddressU = Clamp;
    AddressV = Clamp;
};


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
    uniform float4x4 WorldViewProjMatrix,
    uniform float4x4 WorldMatrix)
{
    // TODO: Inicialitzar l'estructura OUT de tipus PNormalVertex que retornarem a 0
    // TODO: Multiplicar la posici� d'entrada per la matriu WorldViewProj per tenir el v�rtex transformat
    // TODO: Passar les coordenades de textura d'entrada a la variable de sortida
    // TODO: Transformar la normal a normal de World, multiplicant la IN.normal per la matriu de WORLD i normalitzar
    // TODO: Normalitzar el vector -lightDir i passar-li a l'atribut lightDir de l'estructura PNormalVertex
    // TODO: Retorna OUT
}

float4 RenderNormalsPS20(PNormalVertex IN) : COLOR
{
	// TODO: Crear una variable lightEffect de tipus float4 que tindr� el valor de la iluminaci� 
    // TODO: Fer la normalitzaci� utilitzant el m�tode normalize hlsl
    // TODO: Fer el dot de la normal normalitzada amb lightDir a nivell de pixel
    // Fem max amb 0.0 del dot a la l�nia de sota que ha d'estar descomentada
    //lightEffect=max(lightEffect,0);
    // Treiem la coordenada de la llum de la textura de cellshading segons la iluminaci� 
    //float4 cellShadingColor=tex1D(CelShadingTextureSampler,lightEffect);
    // TODO: Retornar el dif�s de la textura pel valor de la iluminaci� del cellShadingColor
}

technique tec0
{
    // TODO: Crear una passada 
    // TODO: Crear un v�rtex shader 1.1 cridant a RenderNormalVS passant-li la matriu de WorldViewProj i la matriu de WorldMatrix
    // TODO: Crear un pixel shader 2.0 cridant a RenderNormalPS20
}