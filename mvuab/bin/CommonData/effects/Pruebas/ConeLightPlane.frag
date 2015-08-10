#version 330

uniform int TIME_FROM_INIT;
uniform int DELTA_FRAME_TIME;

uniform sampler2D TextureUnit0;
uniform sampler2D TextureUnit1;

float rand2(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898, 78.233))) * 43758.5453);
}

float rand3(vec3 co)
{
  	return fract(sin(dot(co ,vec3(12.9898, 78.233, 37.241))) * 43758.5453);
}

float hash(float n)
{
	return fract(sin(n) * 1e4);
}

float hash(vec2 p)
{
	return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) * (0.1 + abs(sin(p.y * 13.0 + p.x))));
}

float noise(vec2 x) 
{
    vec2 i = floor(x);
    vec2 f = fract(x);

	float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);
	return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

bool isEdge(sampler2D l_Texture, vec2 l_TexCoord)
{
	bool l_IsEdge = false;
	
	for(int i = 0; i < 5 && !l_IsEdge; i++)
	{
		if(texture2D(l_Texture, l_TexCoord.xy + vec2( i * 0.001, 0)).a < 0.5) l_IsEdge = true;
		if(texture2D(l_Texture, l_TexCoord.xy + vec2(-i * 0.001, 0)).a < 0.5) l_IsEdge = true;
	}
	
	return l_IsEdge;
}

void main(void)
{
	vec2 l_Movement = vec2(3 + TIME_FROM_INIT * 0.001, 2 + TIME_FROM_INIT * 0.001);
	float l_RandColor = noise(gl_FragCoord.xy + l_Movement * DELTA_FRAME_TIME * 0.001);
	
	vec4 l_Color = vec4(l_RandColor);
	vec4 l_Cone = texture2D(TextureUnit1, gl_TexCoord[1].xy);
	vec4 l_Texture = texture2D(TextureUnit0, gl_TexCoord[0].xy);
	
	if(l_Cone.a > 0.5)
	{
		l_Texture =  mix(l_Texture, l_Color * 2 * ( gl_TexCoord[1].y + 0.2), 0.5);
		if(isEdge(TextureUnit1,  gl_TexCoord[1].xy)) l_Texture = vec4(1.0, 1.0, 1.0, 1.0);
	}
	
	gl_FragColor = l_Texture;
}
