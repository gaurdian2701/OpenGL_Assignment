#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

Texture::Texture(const char* textureFilePath, GLenum minFilerConfig, GLenum magFilterConfig, 
	GLint internalImageFormat, GLenum pixelFormat, unsigned int textureUnitID)
{
	glGenTextures(1, &m_textureID);
	m_textureUnitID = GL_TEXTURE0 + textureUnitID;
	m_textureSlotID = textureUnitID;
	glActiveTexture(textureUnitID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilerConfig);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilterConfig);

	int width, height, numberOfChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imageData = stbi_load(textureFilePath, &width, &height, &numberOfChannels, 0);	

	if(imageData == nullptr)
	{
		std::cout << "Failed to load texture at path: " << textureFilePath << std::endl;
		return;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalImageFormat, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imageData);
}

unsigned int Texture::GetTextureID() const
{
	return m_textureID;
}
unsigned int Texture::GetTextureUnitID() const
{
	return m_textureUnitID;
}

unsigned int Texture::GetTextureSlotID() const
{
	return m_textureSlotID;
}

