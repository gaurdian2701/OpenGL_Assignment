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
#include "StaticObject.h"
#include "ShaderHandler.h"
#include <random>

void Framebuffer_Size_Callback(GLFWwindow* window, int width, int height);
void Mouse_Callback(GLFWwindow* window, double xpos, double ypos);	
void ProcessInput(GLFWwindow* window);

void CheckForCursorVisibility(GLFWwindow* window);
void SetupModel(Model& model);
void InitiateShutdown();
void UpdateUI();
void ReceiveInput();

GLFWwindow* window = nullptr;
Camera* camera = nullptr;

float deltaTime = 0.0f;
bool cursorHidden = false;

std::vector<glm::vec3> objectOffsets;
std::chrono::duration<float> elapsedTime;
std::random_device randomDevice;
std::mt19937 randomGenerator = std::mt19937(randomDevice());


glm::vec2 windVector = glm::vec2(0.0f);

void InitializeOpenGLContext()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSwapInterval(0);

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Starter Project", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
}

void InitContextCallbacks()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, Framebuffer_Size_Callback);
	glfwSetCursorPosCallback(window, Mouse_Callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
	}
}

int main()
{
	InitializeOpenGLContext();

	camera = new Camera(CAMERA_STARTING_POSITION,
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		window,
		CAMERA_SPEED);

	InitContextCallbacks();

	FileHandler fileHandler = FileHandler();
	ShaderHandler* shaderHandler = new ShaderHandler(fileHandler);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& imguiIO = ImGui::GetIO();
	imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
	imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	ShaderProgram* lightSourceShaderProgram =
		new ShaderProgram(
			shaderHandler->GetShader(SIMPLE_OBJECT_VERTEX_SHADER_FILEPATH),
			shaderHandler->GetShader(SIMPLE_OBJECT_FRAGMENT_SHADER_FILEPATH));

	ShaderProgram* illuminatedObjectShaderProgram = 
		new ShaderProgram(
			shaderHandler->GetShader(INSTANCED_OBJECT_VERTEX_SHADER_FILEPATH),
			shaderHandler->GetShader(GRASS_RENDER_FRAGMENT_SHADER_FILEPATH));

	glm::mat4 modelTransformationMatrix = glm::mat4(1.0f);
	glm::mat4 viewTransformationMatrix = glm::mat4(1.0f);
	glm::mat4 projectionTransformationMatrix = glm::mat4(1.0f);

	projectionTransformationMatrix = glm::perspective(glm::radians(45.0f),
		static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),
		0.1f, 100.0f);

	Model grassModel(MODEL_PATH.c_str());
	SetupModel(grassModel);

	StaticObject lightSource = StaticObject(LIGHT_SOURCE_COLOR,
		LIGHT_SOURCE_POSITION,
		projectionTransformationMatrix,
		lightSourceShaderProgram);

	illuminatedObjectShaderProgram->Use();
	illuminatedObjectShaderProgram->SetVec3Float("lightColor", LIGHT_SOURCE_COLOR);
	illuminatedObjectShaderProgram->SetVec3Float("lightPosition", LIGHT_SOURCE_POSITION);
	illuminatedObjectShaderProgram->SetFloat("material.shininess", SPECULAR_MATERIAL_SHININESS);

	float lastFrame = 0.0f;
	auto timeAtStart = std::chrono::high_resolution_clock::now();
	auto timeAtCurrentFrame = 0;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		auto timeAtCurrentFrame = std::chrono::high_resolution_clock::now();
		elapsedTime = timeAtCurrentFrame - timeAtStart;

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

		glm::mat4 modelTransformationMatrix = glm::mat4(1.0f);
		modelTransformationMatrix = glm::translate(modelTransformationMatrix, LIGHT_SOURCE_POSITION);

		illuminatedObjectShaderProgram->Use();
		illuminatedObjectShaderProgram->SetMat4("viewTransformationMatrix", glm::value_ptr(viewTransformationMatrix));
		illuminatedObjectShaderProgram->SetMat4("projectionTransformationMatrix", glm::value_ptr(projectionTransformationMatrix));
		illuminatedObjectShaderProgram->SetVec3Float("viewPosition", camera->GetCameraPosition());
		illuminatedObjectShaderProgram->SetFloat("timeStep", elapsedTime.count());
		illuminatedObjectShaderProgram->SetVec2Float("windVector", windVector);

		modelTransformationMatrix = glm::mat4(1.0f);
		illuminatedObjectShaderProgram->SetMat4("modelTransformationMatrix", 
			glm::value_ptr(modelTransformationMatrix));

		grassModel.Draw(illuminatedObjectShaderProgram, DrawMode::INSTANCED);
		lightSource.Draw(lightSourceShaderProgram, viewTransformationMatrix);

		UpdateUI();
		glfwSwapBuffers(window);
	}

	delete lightSourceShaderProgram;
	delete illuminatedObjectShaderProgram;

	InitiateShutdown();
	return 0;
}


void Framebuffer_Size_Callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Mouse_Callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!cursorHidden)
	{
		camera->MouseCallback(xpos, ypos);
	}
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_TAB) == (GLFW_PRESS | GLFW_RELEASE))
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
	std::uniform_real_distribution<float> offsetDistribution(MIN_OBJECT_OFFSET, MAX_OBJECT_OFFSET);

	for (int i = 1; i <= NUMBER_OF_ROWS; i++)
	{
		for (int j = 1; j <= NUMBER_OF_COLUMNS; j++)
		{
			objectOffsets.push_back(glm::vec3(
				offsetDistribution(randomGenerator) * j,
				0.0f,
				offsetDistribution(randomGenerator) * i));
		}
	}

	model.SetupOffsets(&objectOffsets);
	model.SetupInstanceCount(NUMBER_OF_OBJECTS);
	model.SetupMeshes();
}

void InitiateShutdown()
{
	glfwTerminate();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UpdateUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
		1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);

	ImGui::Text("Number of Objects Rendered: %i", NUMBER_OF_OBJECTS);

	ImGui::Text("Time Step: %.4f", elapsedTime.count());
	ImGui::Text("Sine Value: %.4f", sin(elapsedTime.count()));


	ReceiveInput();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ReceiveInput()
{
	ImGui::SliderFloat2("Wind Vector", &windVector.x, -1.0f, 1.0f);
}

