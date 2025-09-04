#version 330 core

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	float shininess;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform Material material;

out vec4 FragColor;

float ambientStrength = 1;

void main()
{
	vec4 diffuseTexture = texture(material.texture_diffuse1, TexCoord);
	vec4 specularTexture = texture(material.texture_specular1, TexCoord);

	vec3 directionToLight = normalize(lightPosition - FragPos);
	vec3 normal = normalize(Normal);

	vec3 ambientFactor = lightColor * diffuseTexture.rgb;

	vec3 diffuseFactor = lightColor * (max(dot(normal, directionToLight), 0.0))
						* diffuseTexture.rgb;

	vec3 viewDirection = normalize(viewPosition - FragPos);
	vec3 reflectionDirection = reflect(-directionToLight, normal);

	vec3 specularFactor = lightColor * pow(max(dot(reflectionDirection, viewDirection), 0.0f),
							material.shininess) * specularTexture.rgb;

	if(diffuseTexture.a < 0.1)
	{
		discard;
	}

	FragColor = vec4((ambientFactor + diffuseFactor + specularFactor), diffuseTexture.a);
}
