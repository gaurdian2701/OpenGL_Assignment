#include "ShaderProgram.h"
#include "glad/glad.h"
#include "Shader.h"

ShaderProgram::ShaderProgram(Shader* vertexShader, Shader* fragmentShader)
{
	m_shaderProgramID = glCreateProgram();
	glAttachShader(m_shaderProgramID, vertexShader->GetShaderID());
	glAttachShader(m_shaderProgramID, fragmentShader->GetShaderID());
	glLinkProgram(m_shaderProgramID);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_shaderProgramID);
}

void ShaderProgram::Use() const
{
	glUseProgram(m_shaderProgramID);
}

unsigned int ShaderProgram::GetID() const
{
	return m_shaderProgramID;
}

void ShaderProgram::SetBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), (int)value);
}

void ShaderProgram::SetInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), value);
}

void ShaderProgram::SetFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(m_shaderProgramID, name.c_str()), value);
}

void ShaderProgram::SetMat4(const std::string& name, const float* value)
{
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, name.c_str()), 1, GL_FALSE, value);
}

void ShaderProgram::SetVec3Float(const std::string& name, glm::vec3 floatVec)
{
	glUniform3f(glGetUniformLocation(m_shaderProgramID, name.c_str()), floatVec.x, floatVec.y, floatVec.z);	
}
