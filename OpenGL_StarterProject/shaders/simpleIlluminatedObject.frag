#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float lightIntensity;

out vec4 FragColor;

void main()
{
	FragColor = vec4(objectColor * lightColor * lightIntensity, 1.0);
}

