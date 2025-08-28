#pragma once

enum class SHADER_TYPE
{
	VERTEX,
	FRAGMENT
};

class Shader
{
public:
	Shader(const char* shaderSource, SHADER_TYPE shaderType);
	~Shader();
	unsigned int GetShaderID() const;

protected:
	virtual void TryShaderCompilation();

protected:
	unsigned int m_shaderID = 0;
};
