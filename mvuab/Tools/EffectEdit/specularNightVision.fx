texture tex0 < string name = "tiger.bmp"; >;

float3 lightDir
<
    string UIDirectional = "Light Direction";
> = {0.577, -0.577, 0.577};

//string XFile = "teapot.x";

string XFile = "tiger.x";   // Model to load

DWORD  BCLR = 0xff202080;  // Background color (if no image)

// TODO: Crear la variable eyePos amb sem�ntica CAMERAPOSITION, contindr� la posici� de la c�mara

//Constantes de specular
float3 lightColor={1.0,1.0,1.0};
float Ks=0.8;
float SpecExpon=8;

// TODO: Crear una matriu de tipus float4x4 que t� com a sem�ntica WORLDVIEWPROJECTION
// TODO: Crear una matriu de tipus float4x4 que t� com a sem�ntica WORLD

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
    float3 normal: TEXCOORD1;
    float3 lightDir : TEXCOORD2;
    float3 eyeNormal : TEXCOORD3;
};

PNormalVertex RenderNormalsVS(
    VNormalVertex IN,
    uniform float4x4 WorldViewProj,
    uniform float4x4 WorldMatrix)
{
    // TODO: Inicialitzar l'estructura OUT de tipus PNormalVertex que retornarem a 0
    // TODO: Multiplicar la posici� d'entrada per la matriu WorldViewProj per tenir el v�rtex transformat
    // TODO: Transformar la normal a normal de World, multiplicant la IN.normal per la matriu de WORLD i normalitzar-la
    // TODO: Normalitzar el vector -lightDir i passar-li a l'atribut lightDir de l'estructura PNormalVertex
    // TODO: Normalitzar la posici� de l'ull (eyePos) i passar-la per l'eyeNormal del PNormalVertex
    // TODO: Passar les coordenades de textura d'entrada a la variable de sortida    
    // TODO: Retorna OUT
}

float4 RenderSpecularPS20(PNormalVertex IN) : COLOR
{
	// TODO: Fer la normalitzaci� del lightDir utilitzant el m�tode normalize hlsl i assignar el valor a la variable Ln de tipus float3
	// TODO: Fer la normalitzaci� del eyeNormal utilitzant el m�tode normalize hlsl i assignar el valor a la variable Vn de tipus float3
	// TODO: Fer la normalitzaci� de la normal utilitzant el m�tode normalize hlsl i assignar el valor a la variable Nn de tipus float3
	// TODO: Calcular el vector reflexe normalitzant amb el m�tode normalize hlsl la suma de Vn i Ln i assignar el valor a la variable Hn de tipus float3
	// TODO: Calculem el vector d'iluminaci� amb el m�tode lit hlsl passant-li com a par�metres el dot de Ln i Nn, el dot de Hn i Nn, el par�metre SpecExpon i assignar el valor a la variable litV de tipus float4
	// TODO: Calcular la llum difusa que s'aplica al pixel multiplicant litV.y pel lightColor i assignar el valor a la variable DiffuseContrib de tipus float3
	// TODO: Calcular la llum specular que s'aplica al pixel multiplicant litV.y per litV.z per la constant d'specular Ks pel lightColor i assignar el valor a la variable SpecularContrib de tipus float3
	// TODO: Calcular el dif�s de la textura i assignar el valor a la variable diffuseColor de tipus float3
	// TODO: Calcular el color del pixel resultant multiplicant el DiffuseColor*DiffuseContrib m�s el SpecularContrib i assignar-lo a la variable result de tipus float3
	// TODO: Retornem el color del pixel amb component red igual a 0, component green a la mitja aritm�tica de les components result.red, result.green, result.blue, component blue igual a 0 i alfa 1.0
}

technique tec0
{
	// TODO: Crear una passada
    // TODO: Crear un v�rtex shader 1.1 cridant a RenderNormalsVS passant-li la matriu de WorldViewProj i la matriu de WorldMatrix
    // TODO: Crear un pixel shader 2.0 cridant a RenderSpecularPS20
}