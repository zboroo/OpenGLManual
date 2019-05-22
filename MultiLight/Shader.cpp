#include "Shader.h"

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	compile(vertexShaderPath, vertexShaderID, ShaderType::VERTEX);

	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	compile(fragmentShaderPath, fragmentShaderID, ShaderType::FRAGMENT);

	id = glCreateProgram();
	glAttachShader(id, vertexShaderID);
	glAttachShader(id, fragmentShaderID);
	glLinkProgram(id);
	check(id, ShaderType::PROGRAM);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

void Shader::compile(const std::string& path, GLuint shaderID, ShaderType type)
{
	std::string shaderSource;

	std::ifstream file(path);
	std::string line;

	while (std::getline(file, line))
	{
		shaderSource += line + '\n';
	}
	
	file.close();

	const char* shaderSourceCStr = shaderSource.c_str();
	glShaderSource(shaderID, 1, &shaderSourceCStr, nullptr);
	glCompileShader(shaderID);
	check(shaderID, type);
}

void Shader::check(GLuint id, ShaderType type)
{
	int success;
	char infoLog[512];

	switch (type)
	{
	case ShaderType::VERTEX:
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 512, nullptr, infoLog);
			std::cout << "failed to compile vertex shader" << infoLog << std::endl;
		}
		break;
	case ShaderType::FRAGMENT:
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 512, nullptr, infoLog);
			std::cout << "failed to compile fragment shader" << infoLog << std::endl;
		}
		break;
	case ShaderType::PROGRAM:
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(id, 512, NULL, infoLog);
			std::cout << "failed to linked shader program" << infoLog << std::endl;
		}
		break;
	default:
		break;
	}
}

void Shader::use()
{
	glUseProgram(id);
}

void Shader::setInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}
