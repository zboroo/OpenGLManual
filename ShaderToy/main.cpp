#include <iostream>
#include <fstream>
#include <string>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

GLFWwindow* window;
int width = 800;
int height = 600;
std::string title = "OpenGL";

void framebufferSizeCallback(GLFWwindow*, int width, int height)
{
	::width = width;
	::height = height;
	glViewport(0, 0, width, height);
}

int main(int argc, char** argv)
{
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

	std::string line;
	GLint success;

	std::string vertexShaderContent;
	std::ifstream vertexShaderFile("shader/base.vert");
	while (std::getline(vertexShaderFile, line))
	{
		vertexShaderContent += line + '\n';
	}

	const char* vertexShaderSource = vertexShaderContent.c_str();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar info[512];
		glGetShaderInfoLog(vertexShader, 512, nullptr, info);
		std::cout << "vertex shader compile failed: " << info << std::endl;
		return EXIT_FAILURE;
	}

	std::string fragmentShaderContent;
	std::ifstream fragmentShaderFile("shader/base.frag");
	while (std::getline(fragmentShaderFile, line))
	{
		fragmentShaderContent += line + '\n';
	}

	const char* fragmentShaderSource = fragmentShaderContent.c_str();
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar info[512];
		glGetShaderInfoLog(fragmentShader, 512, nullptr, info);
		std::cout << "fargment shader compile failed: " << info << std::endl;
		return EXIT_FAILURE;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar info[512];
		glGetProgramInfoLog(program, 512, nullptr, info);
		std::cout << "program shader link failed: " << info << std::endl;
		return EXIT_FAILURE;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLfloat positions[] =
	{
		-1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f
	};

	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), nullptr, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);

	GLuint IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLint positionLocation = glGetAttribLocation(program, "vertexPostion");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	GLint uResolutionLocation = glGetUniformLocation(program, "iResolution");
	GLint uTimeLocation = glGetUniformLocation(program, "iTime");

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		
		glUniform2f(uResolutionLocation, static_cast<GLfloat>(::width), static_cast<GLfloat>(::height));
		glUniform1f(uTimeLocation, static_cast<GLfloat>(glfwGetTime()));

		glBindVertexArray(VAO);
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