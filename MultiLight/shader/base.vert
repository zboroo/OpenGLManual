#version 430 core
layout(location = 0) in vec3 vertexPostion;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexTexcoord;

out data
{
	vec3 position;
	vec3 color;
	vec2 texcoord;
}vertexData;

void main()
{
	vertexData.texcoord = vertexTexcoord;
	vertexData.color = vertexColor;
	gl_Position = vec4(vertexPostion, 1.0f);
}