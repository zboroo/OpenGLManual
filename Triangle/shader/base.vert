#version 430 core
layout(location = 0) in vec3 vertexPostion;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexOffset;

out data
{
	vec3 position;
	vec3 color;
}vertexData;

void main()
{
	vertexData.color = vertexColor;
	gl_Position = vec4(vertexPostion, 1.0f);
}