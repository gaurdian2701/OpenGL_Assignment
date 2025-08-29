#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 modelTransformationMatrix;
uniform mat4 viewTransformationMatrix;
uniform mat4 projectionTransformationMatrix;

void main()
{
	gl_Position = projectionTransformationMatrix * viewTransformationMatrix * modelTransformationMatrix * vec4(aPos, 1.0);
}