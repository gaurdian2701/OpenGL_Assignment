#pragma once
#include <glad/glad.h>

class Texture
{
public: 
	Texture(const char* textureFilePath, GLenum minFilerConfig, GLenum magFilterConfig,
		GLint internalImageFormat, GLenum pixelFormat, unsigned int textureUnitID);
	unsigned int GetTextureID() const;
	unsigned int GetTextureUnitID() const { return m_textureUnitID; }

protected:
	unsigned int m_textureID = 0;
	unsigned int m_textureUnitID = 0;	
};

