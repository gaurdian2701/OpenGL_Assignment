#include "VertexShader.h"
#include <glad/glad.h>

VertexShader::VertexShader(const char* shaderSource)
{
	InitShader(shaderSource);
}

VertexShader::~VertexShader()
{
	glDeleteShader(m_shaderID);
}

void VertexShader::InitShader(const char* shaderSource)
{
	m_shaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_shaderID, 1, &shaderSource, nullptr);
	glCompileShader(m_shaderID);
}
