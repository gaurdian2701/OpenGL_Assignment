#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform sampler2D mainTexture;
uniform vec3 lightPosition;

out vec4 FragColor;

void main()
{
	vec3 directionToLight = normalize(lightPosition - FragPos);
	vec3 normal = normalize(Normal);
	vec3 diffuseFactor = max(dot(normal, directionToLight), 0.0) * lightColor;
	vec3 ambientFactor = lightIntensity * lightColor;
	FragColor = vec4((ambientFactor + diffuseFactor) * objectColor, 1.0) * texture(mainTexture, TexCoord);
}
