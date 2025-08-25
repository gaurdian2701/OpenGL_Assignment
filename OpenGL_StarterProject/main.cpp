#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const std::string vertexShaderFilePath = "shaders/triangle.vert";
const std::string fragmentShaderFilePath = "shaders/triangle.frag";

void Framebuffer_Size_Callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
bool OpenShaderFiles(std::string& vertexShaderSource, std::string& fragmentShaderSource);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Starter Project", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, Framebuffer_Size_Callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, //bottom left
		-0.5f, 0.5f, 0.0f, //top left
		0.5f, 0.5f, 0.0f, //top right
		0.5f, -0.5f, 0.0f //bottom right
	};

	int indices[] = {
		0, 1, 2, //left triangle
		0, 2, 3 //right triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	std::string vertexShaderString = "";
	std::string fragmentShaderString = "";

	if (!OpenShaderFiles(vertexShaderString, fragmentShaderString))
	{
		glfwTerminate();
		return -1;
	}

	const char* vertexShaderSource = vertexShaderString.c_str();
	const char* fragmentShaderSource = fragmentShaderString.c_str();

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	while (!glfwWindowShouldClose(window))
	{
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		ProcessInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void Framebuffer_Size_Callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

bool OpenShaderFiles(std::string& vertexShaderString, std::string& fragmentShaderString)
{
	std::ifstream ShaderFileStream;

	ShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	std::stringstream vertexShaderStringStream, fragmentShaderStringStream;

	try
	{
		ShaderFileStream.open(vertexShaderFilePath);
		vertexShaderStringStream << ShaderFileStream.rdbuf();
		ShaderFileStream.close();

		ShaderFileStream.open(fragmentShaderFilePath);
		fragmentShaderStringStream << ShaderFileStream.rdbuf();
		ShaderFileStream.close();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "FILE READING ERROR: " << e.what() << std::endl;
		return false;
	}

	vertexShaderString = vertexShaderStringStream.str();
	fragmentShaderString = fragmentShaderStringStream.str();	

	return true;
}