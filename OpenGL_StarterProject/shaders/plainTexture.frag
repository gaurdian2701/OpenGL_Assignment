#version 330 core

struct Material
{
	vec3 ambient;
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	float shininess;
};

in vec2 TexCoord;

out vec4 FragColor;

uniform Material material;

void main()
{
	FragColor = texture(material.texture_diffuse1, TexCoord);
}