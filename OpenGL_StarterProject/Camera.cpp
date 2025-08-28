#include "Camera.h"
#include <GLFW/glfw3.h>

Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraFront, glm::vec3 cameraUp, GLFWwindow* window, float cameraSpeed)
{
	m_window = window;
	m_cameraPosition = cameraPosition;
	m_cameraFront = cameraFront;
	m_cameraSpeed = cameraSpeed;
	m_cameraViewMatrix = glm::lookAt(cameraPosition, cameraFront, cameraUp);
	RotateCamera(-90.0f, 0.0f);
}

glm::mat4 Camera::GetViewMatrix()
{
	m_cameraViewMatrix = glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));
	return m_cameraViewMatrix;
}

void Camera::ProcessInput(float deltaTime, float yaw, float pitch)
{
	MoveCamera(deltaTime);
	RotateCamera(yaw, pitch);
}

void Camera::MoveCamera(float deltaTime)
{
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_cameraPosition += m_cameraSpeed * m_cameraFront * deltaTime;
	}

	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_cameraPosition -= m_cameraSpeed * m_cameraFront * deltaTime;
	}

	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_cameraPosition -= glm::normalize(glm::cross(m_cameraFront, glm::vec3(0.0f, 1.0f, 0.0f))) * m_cameraSpeed * deltaTime;
	}

	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_cameraPosition += glm::normalize(glm::cross(m_cameraFront, glm::vec3(0.0f, 1.0f, 0.0f))) * m_cameraSpeed * deltaTime;
	}
}

void Camera::RotateCamera(float yaw, float pitch)
{
	m_cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));	
	m_cameraFront.y = sin(glm::radians(pitch));
	m_cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	m_cameraFront = glm::normalize(m_cameraFront);
}
