#pragma once

#include "glad/glad.h"
#include "Camera.h"
#include "FileHandler.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Config.h"
#include <vector>
#include "Model.h"

void Framebuffer_Size_Callback(GLFWwindow* window, int width, int height);
void Mouse_Callback(GLFWwindow* window, double xpos, double ypos);	
void ProcessInput(GLFWwindow* window);
void CreateShaderProgram(const std::string& vertexShaderFilePath,
	const std::string& fragmentShaderFilePath,
	Shader** vertexShader,
	Shader** fragmentShader,
	ShaderProgram** shaderProgram);
void CheckForCursorVisibility(GLFWwindow* window);
void SetupModel(Model& model);

Camera* camera = nullptr;
FileHandler fileHandler = FileHandler();
float deltaTime = 0.0f;
bool cursorHidden = false;
std::vector<glm::vec3> objectOffsets;

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

	Shader* simpleObjectVertexShader = nullptr;
	Shader* lightSourceFragmentShader = nullptr;
	ShaderProgram* lightSourceShaderProgram = nullptr;

	CreateShaderProgram(SIMPLE_OBJECT_VERTEX_SHADER_FILEPATH,
		LIGHT_SOURCE_FRAGMENT_SHADER_FILEPATH,
		&simpleObjectVertexShader,
		&lightSourceFragmentShader,
		&lightSourceShaderProgram);

	Shader* instancedObjectVertexShader = nullptr;
	Shader* illuminatedObjectFragmentShader = nullptr;
	ShaderProgram* illuminatedObjectShaderProgram = nullptr;

	CreateShaderProgram(INSTANCED_OBJECT_VERTEX_SHADER_FILEPATH,
		GRASS_FRAGMENT_SHADER_FILEPATH,
		&instancedObjectVertexShader,
		&illuminatedObjectFragmentShader,
		&illuminatedObjectShaderProgram);

	Model grassModel(MODEL_PATH.c_str());
	SetupModel(grassModel);

	illuminatedObjectShaderProgram->Use();
	illuminatedObjectShaderProgram->SetVec3Float("lightColor", LIGHT_SOURCE_COLOR);
	illuminatedObjectShaderProgram->SetVec3Float("lightPosition", LIGHT_SOURCE_POSITION);
	illuminatedObjectShaderProgram->SetFloat("material.shininess", SPECULAR_MATERIAL_SHININESS);

	glm::mat4 modelTransformationMatrix = glm::mat4(1.0f);
	glm::mat4 viewTransformationMatrix = glm::mat4(1.0f);
	glm::mat4 projectionTransformationMatrix = glm::mat4(1.0f);

	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		ProcessInput(window);
		CheckForCursorVisibility(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(BACKGROUND_COLOUR.x, BACKGROUND_COLOUR.y, BACKGROUND_COLOUR.z, BACKGROUND_COLOUR.w);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);

		viewTransformationMatrix = camera->GetViewMatrix();

		projectionTransformationMatrix = glm::perspective(glm::radians(45.0f),
			static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),
			0.1f, 100.0f);

		glm::mat4 modelTransformationMatrix = glm::mat4(1.0f);
		modelTransformationMatrix = glm::translate(modelTransformationMatrix, LIGHT_SOURCE_POSITION);

		lightSourceShaderProgram->Use();

		lightSourceShaderProgram->SetMat4("viewTransformationMatrix", glm::value_ptr(viewTransformationMatrix));
		lightSourceShaderProgram->SetMat4("projectionTransformationMatrix", glm::value_ptr(projectionTransformationMatrix));
		lightSourceShaderProgram->SetMat4("modelTransformationMatrix", glm::value_ptr(modelTransformationMatrix));
		lightSourceShaderProgram->SetVec3Float("lightColor", LIGHT_SOURCE_COLOR);
		glDrawElements(GL_TRIANGLES, 0, 36, 0);

		illuminatedObjectShaderProgram->Use();
		illuminatedObjectShaderProgram->SetMat4("viewTransformationMatrix", glm::value_ptr(viewTransformationMatrix));
		illuminatedObjectShaderProgram->SetMat4("projectionTransformationMatrix", glm::value_ptr(projectionTransformationMatrix));
		illuminatedObjectShaderProgram->SetVec3Float("viewPosition", camera->GetCameraPosition());

		modelTransformationMatrix = glm::mat4(1.0f);
		illuminatedObjectShaderProgram->SetMat4("modelTransformationMatrix", glm::value_ptr(modelTransformationMatrix));
		grassModel.Draw(illuminatedObjectShaderProgram, DrawMode::INSTANCED);

		glfwSwapBuffers(window);
	}

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
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		cursorHidden = !cursorHidden;
	}

	if (!cursorHidden)
	{
		camera->ProcessInput(deltaTime);
	}
}

void CheckForCursorVisibility(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, cursorHidden == false ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void SetupModel(Model& model)
{
	for (int i = 1; i <= NUMBER_OF_GRASS_OBJECTS; i++)
	{
		for (int j = 1; j <= NUMBER_OF_GRASS_OBJECTS; j++)
		{
			objectOffsets.push_back(glm::vec3(1.0f * j, 0.0f, -1.0f * i));
		}
	}

	model.SetupOffsets(&objectOffsets);
	model.SetupInstanceCount(NUMBER_OF_GRASS_OBJECTS * NUMBER_OF_GRASS_OBJECTS);
	model.SetupMeshes();
}

