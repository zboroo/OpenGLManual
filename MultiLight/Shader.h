#ifndef _SHADER_H_
#define _SHADER_H_

#include <iostream>
#include <string>
#include <fstream>
#include "glad/glad.h"

enum class ShaderType
{
	VERTEX, FRAGMENT, PROGRAM
};

class Shader
{
public:
	GLuint id;

public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

public:
	void use();

	void setInt(const std::string& name, int value);

	void setBool(const std::string& name, bool value);

	void setFloat(const std::string& name, float value);

	void setVec2f();
	void setVec3f();
	void setVec4f();

	void setMat4();

private:
	void compile(const std::string& path, GLuint shaderID, ShaderType type);

	void check(GLuint id, ShaderType type);
};

#endif



