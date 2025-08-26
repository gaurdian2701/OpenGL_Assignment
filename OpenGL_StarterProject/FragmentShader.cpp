#include <glad/glad.h>
#include "FragmentShader.h"

FragmentShader::FragmentShader(const char* shaderSource)
{
	InitShader(shaderSource);
}

FragmentShader::~FragmentShader()
{
	glDeleteShader(m_shaderID);
}

void FragmentShader::InitShader(const char* shaderSource)
{
	m_shaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_shaderID, 1, &shaderSource, nullptr);
	glCompileShader(m_shaderID);
}
