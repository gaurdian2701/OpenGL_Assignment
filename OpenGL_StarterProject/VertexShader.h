#pragma once
#include "Shader.h"
class VertexShader : public Shader
{
public:
	VertexShader(const char* shaderSource);
	~VertexShader();

protected:
	void InitShader(const char* shaderSource) override;
};

