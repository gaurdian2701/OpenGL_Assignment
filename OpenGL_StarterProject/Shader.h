#pragma once
class Shader
{
public:
	virtual ~Shader() {}

	virtual unsigned int GetShaderID() const { return m_shaderID; }

protected:
	virtual void InitShader(const char* shaderSource) = 0;

protected:
	unsigned int m_shaderID = 0;
};
