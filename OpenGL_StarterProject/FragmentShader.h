#pragma once
#include "Shader.h"
class FragmentShader : public Shader
{
public:
	FragmentShader(const char* shaderSource);
	~FragmentShader();

protected:
	void InitShader(const char* shaderSource) override;
};

