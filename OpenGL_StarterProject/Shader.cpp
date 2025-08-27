#include "Shader.h"	
#include <iostream>
#include <glad/glad.h>


Shader::~Shader()
{
}

unsigned int Shader::GetShaderID() const
{
	return m_shaderID;
}

bool Shader::IsCompiled() const
{
	return m_isCompiled;
}

void Shader::TryShaderCompilation()
{
	glCompileShader(m_shaderID);
	GLint shaderDidCompile = 0;
	glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &shaderDidCompile);
	if (shaderDidCompile == GL_FALSE)
	{
		GLint infoLogLength = 0;
		glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char errorMessage[512];
		glGetShaderInfoLog(m_shaderID, infoLogLength, &infoLogLength, &errorMessage[0]);
		std::cout << "ERROR: Shader compilation failed:\n" << errorMessage << std::endl;
	}

	m_isCompiled = shaderDidCompile;
}