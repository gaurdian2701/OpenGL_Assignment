#pragma once

#include "glad/glad.h"
#include "Camera.h"
#include "FileHandler.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexAttributePointerData.h"
#include "ObjectDrawData.h"
#include "Config.h"
#include <vector>

const std::string simpleObjectVertexShaderFilePath = "shaders/simpleObject.vert";
const std::string lightSourceFragmentShaderFilePath = "shaders/lightSource.frag";
const std::string illuminatedObjectFragmentShaderFilePath = "shaders/simpleIlluminatedObject.frag";

void Framebuffer_Size_Callback(GLFWwindow* window, int width, int height);
void Mouse_Callback(GLFWwindow* window, double xpos, double ypos);	
void ProcessInput(GLFWwindow* window);
void CreateShaderProgram(const std::string& vertexShaderFilePath,
	const std::string& fragmentShaderFilePath,
	Shader** vertexShader,
	Shader** fragmentShader,
	ShaderProgram** shaderProgram);

Camera* camera = nullptr;
FileHandler fileHandler = FileHandler();
float deltaTime = 0.0f;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Starter Project", NULL, NULL);

	camera = new Camera(CAMERA_STARTING_POSITION, 
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		window,
		CAMERA_SPEED);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, Framebuffer_Size_Callback);
	glfwSetCursorPosCallback(window, Mouse_Callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	glm::vec3 cubePositions[] = {
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	std::vector<float> cubeVertexData = {
		//Positions         //Texture Coords	
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	VertexAttributePointerData positionVertexAttributePointerData = 
		VertexAttributePointerData(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	VertexAttributePointerData textureCoordinateVertexAttributePointerData =
		VertexAttributePointerData(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 3 * sizeof(float));

	std::vector<VertexAttributePointerData*> cubeVertexAttributePointerData = { 
		&positionVertexAttributePointerData, 
		&textureCoordinateVertexAttributePointerData};

	std::vector<VertexAttributePointerData*> lightSourceVertexAttributePointerData = {
		&positionVertexAttributePointerData };

	std::vector<unsigned int> cubeIndices = {};

	ObjectDrawData* cubeDrawData = new ObjectDrawData(cubeVertexData,
		cubeVertexAttributePointerData,
		cubeIndices);

	ObjectDrawData* lightSourceDrawData = new ObjectDrawData(cubeDrawData->GetVBO(),
		lightSourceVertexAttributePointerData,
		cubeIndices);

	Shader* simpleObjectVertexShader = nullptr;
	Shader* lightSourceFragmentShader = nullptr;
	ShaderProgram* lightSourceShaderProgram = nullptr;

	CreateShaderProgram(simpleObjectVertexShaderFilePath,
		lightSourceFragmentShaderFilePath,
		&simpleObjectVertexShader,
		&lightSourceFragmentShader,
		&lightSourceShaderProgram);

	Shader* illuminatedObjectFragmentShader = nullptr;
	ShaderProgram* illuminatedObjectShaderProgram = nullptr;

	CreateShaderProgram(simpleObjectVertexShaderFilePath,
		illuminatedObjectFragmentShaderFilePath,
		&simpleObjectVertexShader,
		&illuminatedObjectFragmentShader,
		&illuminatedObjectShaderProgram);

	glm::mat4 modelTransformationMatrix = glm::mat4(1.0f);
	glm::mat4 viewTransformationMatrix = glm::mat4(1.0f);
	glm::mat4 projectionTransformationMatrix = glm::mat4(1.0f);

	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		viewTransformationMatrix = camera->GetViewMatrix();

		projectionTransformationMatrix = glm::perspective(glm::radians(45.0f),
			static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),
			0.1f, 100.0f);

		glm::mat4 modelTransformationMatrix = glm::mat4(1.0f);
		modelTransformationMatrix = glm::translate(modelTransformationMatrix, glm::vec3(0.0f, 0.0f, -10.0f));

		lightSourceShaderProgram->Use();
		glBindVertexArray(lightSourceDrawData->GetVAO());

		lightSourceShaderProgram->SetMat4("viewTransformationMatrix", glm::value_ptr(viewTransformationMatrix));
		lightSourceShaderProgram->SetMat4("projectionTransformationMatrix", glm::value_ptr(projectionTransformationMatrix));
		lightSourceShaderProgram->SetMat4("modelTransformationMatrix", glm::value_ptr(modelTransformationMatrix));
		lightSourceShaderProgram->SetVec3Float("lightColor", LIGHT_SOURCE_COLOR);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		illuminatedObjectShaderProgram->Use();
		glBindVertexArray(cubeDrawData->GetVAO());
		illuminatedObjectShaderProgram->SetMat4("viewTransformationMatrix", glm::value_ptr(viewTransformationMatrix));
		illuminatedObjectShaderProgram->SetMat4("projectionTransformationMatrix", glm::value_ptr(projectionTransformationMatrix));

		for(int i = 0; i < 9; i++)
		{
			modelTransformationMatrix = glm::mat4(1.0f);
			modelTransformationMatrix = glm::translate(modelTransformationMatrix, cubePositions[i]);
			modelTransformationMatrix = glm::rotate(modelTransformationMatrix,
				static_cast<float>(glfwGetTime()), glm::vec3(1.0f, 0.0f, 0.0f));

			illuminatedObjectShaderProgram->SetMat4("modelTransformationMatrix", glm::value_ptr(modelTransformationMatrix));	
			illuminatedObjectShaderProgram->SetVec3Float("objectColor", OBJECT_COLOR);
			illuminatedObjectShaderProgram->SetVec3Float("lightColor", LIGHT_SOURCE_COLOR);
			illuminatedObjectShaderProgram->SetFloat("lightIntensity", LIGHT_INTENSITY);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete cubeDrawData;
	delete lightSourceDrawData;
	delete simpleObjectVertexShader;
	delete lightSourceFragmentShader;
	delete lightSourceShaderProgram;
	delete illuminatedObjectFragmentShader;
	delete illuminatedObjectShaderProgram;
	

	glfwTerminate();
	return 0;
}


void CreateShaderProgram(const std::string& vertexShaderFilePath,
	const std::string& fragmentShaderFilePath,
	Shader** vertexShader,
	Shader** fragmentShader,
	ShaderProgram** shaderProgram)
{
	std::string vertexShaderString = "";
	std::string fragmentShaderString = "";

	if (!fileHandler.OpenShaderFile(vertexShaderString, vertexShaderFilePath)
		|| !fileHandler.OpenShaderFile(fragmentShaderString, fragmentShaderFilePath))
	{
		glfwTerminate();
	}

	const char* vertexShaderSource = vertexShaderString.c_str();
	const char* fragmentShaderSource = fragmentShaderString.c_str();

	if(*vertexShader == nullptr)
	{
		*vertexShader = new Shader(vertexShaderSource, SHADER_TYPE::VERTEX);
	}

	if (*fragmentShader == nullptr)
	{
		*fragmentShader = new Shader(fragmentShaderSource, SHADER_TYPE::FRAGMENT);
	}

	*shaderProgram = new ShaderProgram(*vertexShader, *fragmentShader);
}

void Framebuffer_Size_Callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Mouse_Callback(GLFWwindow* window, double xpos, double ypos)
{
	camera->MouseCallback(xpos, ypos);
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	camera->ProcessInput(deltaTime);
}

