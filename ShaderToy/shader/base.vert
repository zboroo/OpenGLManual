#version 430 core
layout(location = 0) in vec3 vertexPostion;

void main()
{
	gl_Position = vec4(vertexPostion, 1.0f);
}