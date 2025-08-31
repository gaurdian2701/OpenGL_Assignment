#version 330 core

in vec2 TexCoord;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform sampler2D mainTexture;

out vec4 FragColor;

void main()
{
	FragColor = vec4(objectColor * lightColor * lightIntensity, 1.0) * texture(mainTexture, TexCoord);
}

