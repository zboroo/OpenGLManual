#version 430 core
out vec4 fragmentColor;

in data
{
	vec3 position;
	vec3 color;
}fragmentData;

void main()
{
	fragmentColor = vec4(fragmentData.color, 1.0f);
}