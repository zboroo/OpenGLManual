#include <iostream>
#include <fstream>
#include <string>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Texture.h"

GLFWwindow* window;
int width = 800;
int height = 800;
std::string title = "OpenGL";

float prevFrameTime = 0.0f;
float deltaFrameTime = 0.0f;

void framebufferSizeCallback(GLFWwindow*, int width, int height)
{
	::width = width;
	::height = height;
	glViewport(0, 0, width, height);
}

int main(int argc, char** argv)
{
	// GLFW和GLAD初始化
	if (!glfwInit())
	{
		std::cout << "faild to init GLFW..." << std::endl;
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "failed to create GLFW window..." << std::endl;
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to init GLAD..." << std::endl;
	}

	// Shader编译链接
	Shader baseShader("shader/base.vert", "shader/base.frag");

	// 顶点属性
	GLfloat positions[] =
	{
		 0.5f,  0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  0.0f,
		-0.5f,  0.5f,  0.0f,
		 0.5f,  0.5f, -1.0f,
		 0.5f, -0.5f, -1.0f,
		-0.5f, -0.5f, -1.0f,
		-0.5f,  0.5f, -1.0f
	};

	GLfloat colors[] =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f
	};

	GLfloat texcoords[] =
	{
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	// 缓冲区数据绑定
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(colors) + sizeof(texcoords), nullptr, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(colors), colors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(colors), sizeof(texcoords), texcoords);

	GLint positionLocation = glGetAttribLocation(baseShader.id, "vertexPostion");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	GLint colorLocation = glGetAttribLocation(baseShader.id, "vertexColor");
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(sizeof(positions)));

	GLint texcoordLocation = glGetAttribLocation(baseShader.id, "vertexTexcoord");
	glEnableVertexAttribArray(texcoordLocation);
	glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void* )(sizeof(positions) + sizeof(colors)));

	GLuint IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 生成贴图
	Texture firstTexture;
	firstTexture.generate(baseShader.id, "firstTexture", GL_TEXTURE0, "texture/brickwall.jpg");

	Texture secondTexture;
	secondTexture.generate(baseShader.id, "secondTexture", GL_TEXTURE1, "texture/awesomeface.png");


	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window))
	{	
		float currentTime = static_cast<float>(glfwGetTime());
		deltaFrameTime = currentTime - prevFrameTime;
		prevFrameTime = currentTime;

		glClear(GL_COLOR_BUFFER_BIT);

		baseShader.use();
		
		glBindVertexArray(VAO);
		firstTexture.bind();
		secondTexture.bind();
		
		baseShader.setFloat("runtime", static_cast<float>(glfwGetTime()));
		baseShader.setFloat("delta", deltaFrameTime);
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		
		
	}

	glDeleteBuffers(1, &IBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}