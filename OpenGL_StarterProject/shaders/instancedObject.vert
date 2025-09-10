#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aOffset;

uniform float timeStep;
uniform vec2 windVector;
uniform mat4 modelTransformationMatrix;
uniform mat4 viewTransformationMatrix;
uniform mat4 projectionTransformationMatrix;

float minSwayingHeight = 0.1f;
float sineValue = sin(timeStep);

out VS_OUT
{
	vec2 TexCoord;
	vec3 Normal;
	vec3 FragPos;
} vs_out;	


void SwayVertices()
{
	float windX = windVector.x;
	float windY = windVector.y;

	vec3 baseDisplacementVector = vs_out.FragPos;
	vec3 swayInWindDirection = vec3(0.0f);

	if(aPos.y > minSwayingHeight)
	{
		baseDisplacementVector = vec3(
		vs_out.FragPos.x + windX * aPos.y,
		vs_out.FragPos.y,
		vs_out.FragPos.z + windY * aPos.y);   
	}

	vs_out.FragPos = baseDisplacementVector;
}

void main()
{
	vs_out.FragPos = vec3(modelTransformationMatrix * vec4(aPos + aOffset, 1.0));
	vs_out.Normal = mat3(transpose(inverse(modelTransformationMatrix))) * aNormal;
	vs_out.TexCoord = aTexCoord;

	SwayVertices();

	gl_Position = projectionTransformationMatrix * viewTransformationMatrix * vec4(vs_out.FragPos, 1.0);
}