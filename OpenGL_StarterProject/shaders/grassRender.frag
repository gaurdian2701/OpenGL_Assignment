#version 330 core

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	float shininess;
};

in VS_OUT
{
	vec2 TexCoord;
	vec3 Normal;
	vec3 FragPos;
} fs_in;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform Material material;

float ambientStrength = 1;
float cullingDistance = 1000;

void main()
{
	vec4 diffuseTexture = texture(material.texture_diffuse1, fs_in.TexCoord);
	vec4 specularTexture = texture(material.texture_specular1, fs_in.TexCoord);

	vec3 directionToLight = normalize(lightPosition - fs_in.FragPos);
	vec3 normal = normalize(fs_in.Normal);

	vec3 ambientFactor = lightColor * diffuseTexture.rgb;

	vec3 diffuseFactor = lightColor * (max(dot(normal, directionToLight), 0.0))
						* diffuseTexture.rgb;

	vec3 viewDirection = normalize(viewPosition - fs_in.FragPos);
	vec3 reflectionDirection = reflect(-directionToLight, normal);

	vec3 specularFactor = lightColor * pow(max(dot(reflectionDirection, viewDirection), 0.0f),
							material.shininess) * specularTexture.rgb;

	if(diffuseTexture.a < 0.1 || distance(fs_in.FragPos, viewPosition) >= cullingDistance)
	{
		discard;
	}

	FragColor = vec4((ambientFactor + diffuseFactor + specularFactor), diffuseTexture.a);
}
