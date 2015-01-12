texture tex0 < string name = "C:\\Documents and Settings\\jordi\\Escritorio\\UAB Videojocs\\Asignatura\\UAB Videojocs Avanzado Marzo 2008\\UAB_Videojocs_Sessio6\\effectedit\\diffuse_earth.png"; >;
texture tex1 < string name = "C:\\Documents and Settings\\jordi\\Escritorio\\UAB Videojocs\\Asignatura\\UAB Videojocs Avanzado Marzo 2008\\UAB_Videojocs_Sessio6\\effectedit\\earthNormalMap.png"; >;

float3 lightDir
<
    string UIDirectional = "Light Direction";
> = {0.577, -0.577, 0.577};

texture EnvironmentMap 
< 
    string type = "CUBE"; 
    string name = "lobby\\lobbycube.dds"; 
>;

string XFile = "misc/sphere.x";   // Model to load

DWORD  BCLR = 0xff202080;  // Background color (if no image)

// TODO: Crear la variable de tipus float3 eyePos amb semàntica CAMERAPOSITION

//Constantes de specular
float3 lightColor={1.0,1.0,1.0};
float Ks=0.8;
float SpecExpon=8;

//Constantes NormalMap
float Bump=2.4;

// TODO: Crear una matriu de tipus float4x4 que té com a semàntica WORLDVIEWPROJECTION
// TODO: Crear una matriu de tipus float4x4 que té com a semàntica WORLDVIEW
// TODO: Crear una matriu de tipus float4x4 que té com a semàntica WORLD

// COMENTARI: Creem el sampler per la textura de Environment
sampler Environment = sampler_state
{ 
    Texture = (EnvironmentMap);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

// COMENTARI: Creem el sampler per la textura de difús
sampler DiffuseTextureSampler = sampler_state
{ 
    Texture = (tex0);
    MipFilter = LINEAR; 
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

// COMENTARI: Creem el sampler per la textura de normals
sampler NormalTextureSampler = sampler_state
{ 
    Texture = (tex1);
    MipFilter = LINEAR; 
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

struct VNormalVertex 
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 Tangent	: TANGENT0;
};

struct PNormalVertex
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
    float3 normal: TEXCOORD1;
    float3 lightDir : TEXCOORD2;
    float3 eyeNormal : TEXCOORD3;
    float3 Reflection : TEXCOORD4;
    float3 WorldTangent	: TEXCOORD5;
    float3 WorldBinormal : TEXCOORD6;
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
	// TODO: Calcular la tangent en tangent de World multiplicant la IN.Tangent per la matriu de WORLD i normalitzar-la
	// TODO: Calcular la WorldBinormal multiplicant (el producte escalar cross del IN.Tangent per la IN.normal) per la WorldMatrix    
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

float4 RenderBumpNormal(PNormalVertex IN) : COLOR
{
	// TODO: Fer la normalització del lightDir utilitzant el mètode normalize hlsl i assignar el valor a la variable Ln de tipus float3
	// TODO: Fer la normalització del eyeNormal utilitzant el mètode normalize hlsl i assignar el valor a la variable Vn de tipus float3
	// TODO: Fer la normalització de la normal utilitzant el mètode normalize hlsl i assignar el valor a la variable Nn de tipus float3
	// TODO: Fer la normalització de la WorldTangent utilitzant el mètode normalize hlsl i assignar el valor a la variable Tn de tipus float3
	// TODO: Calcular el vector reflexe normalitzant amb el mètode normalize hlsl la suma de Vn i Ln i assignar el valor a la variable Hn de tipus float3
	// TODO: Fer la normalització de la WorldBinormal utilitzant el mètode normalize hlsl i assignar el valor a la variable Bn de tipus float3
	// COMENTARI: Calculem el bump mitjançant la textura de normalmap
	float3 bump = Bump * (tex2D(NormalTextureSampler,IN.uv).rgb - float3(0.5,0.5,0.5));
	// COMENTARI: Calculem la nova normal mitjançant el bump i la normalitzem
    Nn = Nn + bump.x*Tn + bump.y*Bn;
    Nn = normalize(Nn);
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
	// TODO: Crear una passada 
    // TODO: Crear un vèrtex shader 1.1 cridant a RenderNormalVS passant-li la matriu de g_WorldViewProjMatrix, g_WorldViewMatrix, g_WorldMatrix
    // TODO: Crear un pixel shader 2.0 cridant a RenderBumpNormal
}
