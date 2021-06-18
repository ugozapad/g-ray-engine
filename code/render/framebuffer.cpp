#include "stdafx.h"
#include "framebuffer.h"

Framebuffer::Framebuffer(uint32_t width, uint32_t height, const std::string& name, bool srgb /*= false*/) :
	m_Width(width),
	m_Height(height),
	m_Name(name)
{
	if (srgb)
		glEnable(GL_FRAMEBUFFER_SRGB);

	glGenFramebuffers(1, &m_Framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

	GLint internalFormat = 0;

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, srgb ? GL_SRGB : GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

	glGenRenderbuffers(1, &m_RenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Error("Framebuffer::Framebuffer: Framebuffer is not complete.");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Msg("created framebuffer %s [%ix%i]", m_Name.c_str(), m_Width, m_Height);
}

Framebuffer::~Framebuffer()
{
	glDeleteRenderbuffers(1, &m_RenderBuffer);
	glDeleteTextures(1, &m_Texture);
	glDeleteFramebuffers(1, &m_Framebuffer);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
}

void Framebuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
