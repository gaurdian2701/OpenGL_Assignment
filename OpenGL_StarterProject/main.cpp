#pragma once

#include "glad/glad.h"
#include "Camera.h"
#include "FileHandler.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Config.h"
#include <vector>
#include "Model.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void Framebuffer_Size_Callback(GLFWwindow* window, int width, int height);
void Mouse_Callback(GLFWwindow* window, double xpos, double ypos);	
void ProcessInput(GLFWwindow* window);
void CreateShaderProgram(const std::string& vertexShaderFilePath,
	const std::string& fragmentShaderFilePath,
	Shader** vertexShader,
	Shader** fragmentShader,
	ShaderProgram** shaderProgram);
void StartImGuiFrame();
void EndImGuiFrame();
void ShutdownImgui();
void CheckForCursorVisibility(GLFWwindow* window);

Camera* camera = nullptr;
FileHandler fileHandler = FileHandler();
float deltaTime = 0.0f;
bool cursorHidden = true;

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

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& imguiIO = ImGui::GetIO();
	imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	Shader* simpleObjectVertexShader = nullptr;
	Shader* lightSourceFragmentShader = nullptr;
	ShaderProgram* lightSourceShaderProgram = nullptr;

	CreateShaderProgram(SIMPLE_OBJECT_VERTEX_SHADER_FILEPATH,
		LIGHT_SOURCE_FRAGMENT_SHADER_FILEPATH,
		&simpleObjectVertexShader,
		&lightSourceFragmentShader,
		&lightSourceShaderProgram);

	Shader* illuminatedObjectFragmentShader = nullptr;
	ShaderProgram* illuminatedObjectShaderProgram = nullptr;

	CreateShaderProgram(SIMPLE_OBJECT_VERTEX_SHADER_FILEPATH,
		GRASS_FRAGMENT_SHADER_FILEPATH,
		&simpleObjectVertexShader,
		&illuminatedObjectFragmentShader,
		&illuminatedObjectShaderProgram);

	Model grassModel(MODEL_PATH.c_str());

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
		StartImGuiFrame();
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
		glDrawArraysInstanced(GL_TRIANGLES, 0, 36, NUMBER_OF_GRASS_OBJECTS);

		illuminatedObjectShaderProgram->Use();
		illuminatedObjectShaderProgram->SetMat4("viewTransformationMatrix", glm::value_ptr(viewTransformationMatrix));
		illuminatedObjectShaderProgram->SetMat4("projectionTransformationMatrix", glm::value_ptr(projectionTransformationMatrix));
		illuminatedObjectShaderProgram->SetVec3Float("viewPosition", camera->GetCameraPosition());

		for (unsigned int i = 0; i < NUMBER_OF_GRASS_OBJECTS; i++)
		{
			modelTransformationMatrix = glm::mat4(1.0f);
			modelTransformationMatrix = glm::translate(modelTransformationMatrix, glm::vec3(0.0f, 0.0f, -1.0f * i));
			illuminatedObjectShaderProgram->SetMat4("modelTransformationMatrix", glm::value_ptr(modelTransformationMatrix));
			grassModel.Draw(illuminatedObjectShaderProgram);
		}

		EndImGuiFrame();
		glfwSwapBuffers(window);
	}

	delete simpleObjectVertexShader;
	delete lightSourceFragmentShader;
	delete lightSourceShaderProgram;
	delete illuminatedObjectFragmentShader;
	delete illuminatedObjectShaderProgram;
	
	ShutdownImgui();
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

void StartImGuiFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
}

void EndImGuiFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ShutdownImgui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
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

