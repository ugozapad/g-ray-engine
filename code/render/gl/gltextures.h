#ifndef RENDER_GL_TEXTURES_H
#define RENDER_GL_TEXTURES_H

#include "render/texture.h"

class glTexture2D : public ITexture2D
{
public:
	glTexture2D(const TextureCreationDesc& desc);
	~glTexture2D();

	void Release() override;
	uint32_t GetHandle() override { return m_handle; }
	GLenum GetType() { return m_textureType; }
private:
	TextureCreationDesc m_desc;
	GLuint m_handle;
	GLenum m_textureType;
};

#endif // !RENDER_GL_TEXTURES_H
