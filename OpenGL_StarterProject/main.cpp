#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "FileHandler.h"
#include "VertexShader.h"
#include "FragmentShader.h"
#include "Texture.h"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;

const std::string vertexShaderFilePath = "shaders/triangle.vert";
const std::string fragmentShaderFilePath = "shaders/triangle.frag";

void Framebuffer_Size_Callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

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

	FileHandler fileHandler = FileHandler();

	float vertices[] = {
		//Positions         //Colors        //Texture Coords
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Bottom Left
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //Top Left
		0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // Top Right
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f // Bottom Right
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));

	std::string vertexShaderString = "";
	std::string fragmentShaderString = "";

	if (!fileHandler.OpenShaderFile(vertexShaderString, vertexShaderFilePath) 
		|| !fileHandler.OpenShaderFile(fragmentShaderString, fragmentShaderFilePath))
	{
		glfwTerminate();
		return -1;
	}

	const char* vertexShaderSource = vertexShaderString.c_str();
	const char* fragmentShaderSource = fragmentShaderString.c_str();

	Shader* vertexShader = new VertexShader(vertexShaderSource);
	Shader* fragmentShader = new FragmentShader(fragmentShaderSource);

	if(vertexShader->IsCompiled() == false || fragmentShader->IsCompiled() == false)
	{
		glfwTerminate();
		return -1;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader->GetShaderID());
	glAttachShader(shaderProgram, fragmentShader->GetShaderID());
	glLinkProgram(shaderProgram);


	Texture skibidiTexture = Texture("textures/skibidi.jpg", GL_LINEAR, GL_LINEAR, GL_RGB, GL_RGB, 0);
	Texture awesomefaceTexture = Texture("textures/awesomeface.png", GL_LINEAR, GL_LINEAR, GL_RGB, GL_RGBA, 1);

	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);

	glm::mat4 modelTransformationMatrix = glm::mat4(1.0f);
	modelTransformationMatrix = glm::rotate(modelTransformationMatrix, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 viewTransformationMatrix = glm::mat4(1.0f);	
	viewTransformationMatrix = glm::translate(viewTransformationMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projectionTransformationMatrix = glm::mat4(1.0f);
	projectionTransformationMatrix = glm::perspective(glm::radians(45.0f),
		static_cast<float>(SCREEN_WIDTH)/static_cast<float>(SCREEN_HEIGHT),
		0.1f, 100.0f);

	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		glClear(GL_COLOR_BUFFER_BIT);
		glActiveTexture(skibidiTexture.GetTextureUnitID());
		glBindTexture(GL_TEXTURE_2D, skibidiTexture.GetTextureID());
		glActiveTexture(awesomefaceTexture.GetTextureUnitID());
		glBindTexture(GL_TEXTURE_2D, awesomefaceTexture.GetTextureID());

		glUseProgram(shaderProgram);

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelTransformationMatrix"),
			1, GL_FALSE, glm::value_ptr(modelTransformationMatrix));

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewTransformationMatrix"),
			1, GL_FALSE, glm::value_ptr(viewTransformationMatrix));

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projectionTransformationMatrix"),
			1, GL_FALSE, glm::value_ptr(projectionTransformationMatrix));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete vertexShader;
	delete fragmentShader;

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