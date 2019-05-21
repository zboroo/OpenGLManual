#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <iostream>
#include <string>
#include <array>
#include "glad/glad.h"
#include "stb_image.h"

class Texture
{
public:
	GLuint id;

public:
	GLint wrapS;
	GLint wrapT;
	GLint minFilter;
	GLint magFilter;
	std::array<float, 4> borderColor;

public:
	Texture();
	void generate(GLuint shaderProgram, std::string textureName, GLenum activeTextureNum, const std::string path);
	void bind();
};

#endif