#include "Texture.h"

Texture::Texture() : id(0), wrapS(GL_REPEAT), wrapT(GL_REPEAT), minFilter(GL_LINEAR), magFilter(GL_LINEAR)
{
	glGenTextures(1, &id);
}

void Texture::generate(GLuint shaderProgram, std::string textureName, GLenum activeTextureNum, const std::string path)
{
	glActiveTexture(activeTextureNum);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	if (wrapS == GL_CLAMP_TO_BORDER || wrapT == GL_CLAMP_TO_BORDER)
	{
		glGetTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor.data());
	}

	int width = 0, height = 0, channel = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channel, 0);

	GLenum format = GL_RGB;

	if (1 == channel)
		format = GL_RED;
	else if (3 == channel)
		format = GL_RGB;
	else if (4 == channel)
		format = GL_RGBA;

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);

		glUseProgram(shaderProgram);
		glUniform1i(glGetUniformLocation(shaderProgram, textureName.c_str()), activeTextureNum - GL_TEXTURE0);
	}
	else
	{
		std::cout << "failed to load texture: " << path << std::endl;
	}
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}
