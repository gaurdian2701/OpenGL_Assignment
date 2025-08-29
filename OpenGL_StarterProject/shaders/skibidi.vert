#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 modelTransformationMatrix;
uniform mat4 viewTransformationMatrix;
uniform mat4 projectionTransformationMatrix;

out vec2 TexCoord;

void main()
{
	gl_Position = projectionTransformationMatrix * viewTransformationMatrix * modelTransformationMatrix * vec4(aPos, 1.0);
	TexCoord = aTexCoord;
}