#pragma once
class Shader
{
public:
	virtual ~Shader();
	virtual unsigned int GetShaderID() const;
	virtual bool IsCompiled() const;

protected:
	virtual void InitShader(const char* shaderSource) = 0;
	virtual void TryShaderCompilation();

protected:
	unsigned int m_shaderID = 0;
	bool m_isCompiled = false;
};
