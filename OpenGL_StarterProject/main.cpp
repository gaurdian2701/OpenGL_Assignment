#pragma once
#include "glad/glad.h"

#include "Camera.h"
#include "FileHandler.h"
#include "Shader.h"
#include "Texture.h"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;

const std::string vertexShaderFilePath = "shaders/triangle.vert";
const std::string fragmentShaderFilePath = "shaders/triangle.frag";

void Framebuffer_Size_Callback(GLFWwindow* window, int width, int height);
void Mouse_Callback(GLFWwindow* window, double xpos, double ypos);	
void ProcessInput(GLFWwindow* window);

Camera* camera = nullptr;
float deltaTime = 0.0f;
const float mouseSensitivity = 0.1f;	
bool firstMouse = true;
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
float yaw = -90.0f;
float pitch = 0.0f;


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Starter Project", NULL, NULL);
	camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), 
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		window,
		5.0f);

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

	FileHandler fileHandler = FileHandler();

	float vertices[] = {
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

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

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

	Shader* vertexShader = new Shader(vertexShaderSource, SHADER_TYPE::VERTEX);
	Shader* fragmentShader = new Shader(fragmentShaderSource, SHADER_TYPE::FRAGMENT);

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
	glm::mat4 viewTransformationMatrix = glm::mat4(1.0f);
	glm::mat4 projectionTransformationMatrix = glm::mat4(1.0f);

	const float circlingRadius = 10.0f;
	float lastFrame = 0.0f;


	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glActiveTexture(skibidiTexture.GetTextureUnitID());
		glBindTexture(GL_TEXTURE_2D, skibidiTexture.GetTextureID());
		glActiveTexture(awesomefaceTexture.GetTextureUnitID());
		glBindTexture(GL_TEXTURE_2D, awesomefaceTexture.GetTextureID());

		float camX = static_cast<float>(sin(glfwGetTime()) * circlingRadius);
		float camZ = static_cast<float>(cos(glfwGetTime()) * circlingRadius);

		viewTransformationMatrix = camera->GetViewMatrix();

		projectionTransformationMatrix = glm::perspective(glm::radians(45.0f),
			static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),
			0.1f, 100.0f);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewTransformationMatrix"),
			1, GL_FALSE, glm::value_ptr(viewTransformationMatrix));

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projectionTransformationMatrix"),
			1, GL_FALSE, glm::value_ptr(projectionTransformationMatrix));  

		for(int i = 0; i < 9; i++)
		{
			modelTransformationMatrix = glm::mat4(1.0f);
			modelTransformationMatrix = glm::translate(modelTransformationMatrix, cubePositions[i]);
			modelTransformationMatrix = glm::rotate(modelTransformationMatrix,
				static_cast<float>(glfwGetTime()), glm::vec3(1.0f, 0.0f, 0.0f));

			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelTransformationMatrix"),
				1, GL_FALSE, glm::value_ptr(modelTransformationMatrix));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

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

void Mouse_Callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	camera->ProcessInput(deltaTime, yaw, pitch);
}