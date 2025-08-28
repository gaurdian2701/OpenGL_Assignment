#pragma once
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/vector_float3.hpp>

class Camera
{
public:
	Camera(glm::vec3 cameraPosition, glm::vec3 cameraFront, glm::vec3 cameraUp, GLFWwindow* window, float cameraSpeed);
	void ProcessInput(float deltaTime, float yaw, float pitch);
	glm::mat4 GetViewMatrix();

private:
	glm::mat4 m_cameraViewMatrix;
	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraFront;
	GLFWwindow* m_window;
	float m_cameraSpeed = 0.0f;

	void MoveCamera(float deltaTime);
	void RotateCamera(float yaw, float pitch);
};