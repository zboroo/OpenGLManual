#version 430 core
out vec4 fragmentColor;

in data
{
	vec3 position;
	vec3 color;
	vec2 texcoord;
}fragmentData;

uniform float runtime;
uniform float delta;

uniform sampler2D firstTexture;
uniform sampler2D secondTexture;

float nTexcoordX = fragmentData.texcoord.x;
float speed = 0.8f;

void main()
{
	nTexcoordX += runtime*speed;
	
	fragmentColor = vec4(texture(firstTexture,fragmentData.texcoord).rgb, 1.0f);
	//fragmentColor = vec4(mix(texture(firstTexture,fragmentData.texcoord),texture(secondTexture,vec2(nTexcoordX,fragmentData.texcoord.y)),0.2f).rgb * fragmentData.color, 1.0f);
}