#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 modelTransformationMatrix;
uniform mat4 viewTransformationMatrix;
uniform mat4 projectionTransformationMatrix;

uniform vec2 offsets[100];

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;	

void main()
{
	FragPos = vec3(modelTransformationMatrix * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(modelTransformationMatrix))) * aNormal;
	gl_Position = projectionTransformationMatrix * viewTransformationMatrix * vec4(FragPos, 1.0);
	TexCoord = aTexCoord;
}