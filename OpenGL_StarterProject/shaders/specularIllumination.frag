#version 330 core

struct Material
{
	vec3 ambient;
	sampler2D diffuseMap;
	sampler2D specularMap;
	float shininess;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform Material objectMaterial;

out vec4 FragColor;

float specularStrength = 0.8;
float ambientStrength = 0.2;
float lightIntensity = 0.5f;

void main()
{
	vec3 directionToLight = normalize(lightPosition - FragPos);
	vec3 normal = normalize(Normal);

	vec3 ambientFactor = lightColor * objectMaterial.ambient * texture(objectMaterial.diffuseMap, TexCoord).rgb;

	vec3 diffuseFactor = lightColor * (max(dot(normal, directionToLight), 0.0))
						* texture(objectMaterial.diffuseMap, TexCoord).rgb;

	vec3 viewDirection = normalize(viewPosition - FragPos);
	vec3 reflectionDirection = reflect(-directionToLight, normal);

	vec3 specularFactor = lightColor * pow(max(dot(reflectionDirection, viewDirection), 0.0f),
							objectMaterial.shininess) * texture(objectMaterial.specularMap, TexCoord).rgb;

	FragColor = vec4((ambientFactor + diffuseFactor + specularFactor), 1.0);
}
