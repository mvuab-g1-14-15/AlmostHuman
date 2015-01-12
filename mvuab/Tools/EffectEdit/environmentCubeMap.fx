texture tex0 < string name = "tiger.bmp"; >;
//texture tex0 < string name = "bihull.bmp"; >;

//define USE_PS_14

float3 lightDir
<
    string UIDirectional = "Light Direction";
> = {0.577, -0.577, 0.577};

//string XFile = "airplane 2.x";
string XFile = "sphere0.x";
//string XFile = "tiger.x";   // Model to load

// COMENTARI: Creem una textura de tipus CUBEMAP
texture EnvironmentMap 
< 
    string type = "CUBE"; 
    string name = "lobbycube.dds"; 
>;

DWORD  BCLR = 0xff202080;  // Background color (if no image)

// TODO: Crear la variable eyePos amb semàntica CAMERAPOSITION, contindrà la posició de la càmara

//Constantes de specular
float3 lightColor={1.0,1.0,1.0};
float Ks=0.8;
float SpecExpon=8;

// TODO: Crear una matriu de tipus float4x4 que té com a semàntica WORLDVIEWPROJECTION
// TODO: Crear una matriu de tipus float4x4 que té com a semàntica WORLDVIEW
// TODO: Crear una matriu de tipus float4x4 que té com a semàntica WORLD

// TODO: Crear un sampler (Environment) que agafi la textura EnvironmentMap i amb filtres LINEAR

// TODO: Crear un sampler (DiffuseTextureSampler) que agafi la textura tex0 i amb filtres LINEAR

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
    float3 Reflection : TEXCOORD4;
};

PNormalVertex RenderNormalsVS(
    VNormalVertex IN,
    uniform float4x4 WorldViewProjMatrix,
    uniform float4x4 WorldViewMatrix,
    uniform float4x4 WorldMatrix)
{
    // TODO: Inicialitzar l'estructura OUT de tipus PNormalVertex que retornarem a 0
    // TODO: Multiplicar la posició d'entrada per la matriu WorldViewProj per tenir el vèrtex transformat
    // TODO: Transformar la normal a normal de World, multiplicant la IN.normal per la matriu de WORLD i normalitzar-la
    // TODO: Normalitzar el vector -lightDir i passar-li a l'atribut lightDir de l'estructura PNormalVertex
    // TODO: Normalitzar la posició de l'ull (eyePos) i passar-la per l'eyeNormal del PNormalVertex
    // TODO: Passar les coordenades de textura d'entrada a la variable de sortida    

    // COMENTARI: Amb el següent codi calculem el vector Reflection, descomentar el codi
    /*
    float3 P = mul(float4(IN.pos.xyz,1),(float4x3) WorldViewMatrix);   // position (view space)
    float3 N = normalize(mul(IN.normal, (float3x3)WorldViewMatrix));     // normal (view space)
    float3 V = -normalize(P);                    // view direction (view space)
    float3 G = normalize(2 * dot(N, V) * N - V);                // glance vector (view space)
    OUT.Reflection = float3(-G.x, G.y, -G.z);*/
    
    // TODO: Retorna OUT
}

float4 RenderSpecularPS20(PNormalVertex IN) : COLOR
{
	// TODO: Fer la normalització del lightDir utilitzant el mètode normalize hlsl i assignar el valor a la variable Ln de tipus float3
	// TODO: Fer la normalització del eyeNormal utilitzant el mètode normalize hlsl i assignar el valor a la variable Vn de tipus float3
	// TODO: Fer la normalització de la normal utilitzant el mètode normalize hlsl i assignar el valor a la variable Nn de tipus float3
	// TODO: Calcular el vector reflexe normalitzant amb el mètode normalize hlsl la suma de Vn i Ln i assignar el valor a la variable Hn de tipus float3
	// TODO: Calculem el vector d'iluminació amb el mètode lit hlsl passant-li com a paràmetres el dot de Ln i Nn, el dot de Hn i Nn, el paràmetre SpecExpon i assignar el valor a la variable litV de tipus float4
	// TODO: Calcular la llum difusa que s'aplica al pixel multiplicant litV.y pel lightColor i assignar el valor a la variable DiffuseContrib de tipus float3
	// TODO: Calcular la llum specular que s'aplica al pixel multiplicant litV.y per litV.z per la constant d'specular Ks pel lightColor i assignar el valor a la variable SpecularContrib de tipus float3
	// TODO: Calcular el difús de la textura i assignar el valor a la variable diffuseColor de tipus float3
	// TODO: Calcular el color del pixel resultant multiplicant el DiffuseColor*DiffuseContrib més el SpecularContrib
	// COMENTARI: Amb el codi de sota calculem el valor del color del cubemap
    //float3 envCubeMap = texCUBE(Environment, IN.Reflection);
	// TODO: Sumem a les components xyz del OUT.xyz l'envCubeMap*un factor de reflexe (0.15 per exemple)*DiffuseContrib
	// TODO: Retornem el color del pixel amb alfa 1.0
}

technique tec0
{
    pass p0
    {
        VertexShader =compile vs_1_1 RenderNormalsVS(g_WorldViewProjMatrix, g_WorldViewMatrix, g_WorldMatrix);
        PixelShader = compile ps_2_0 RenderSpecularPS20();
    }
}