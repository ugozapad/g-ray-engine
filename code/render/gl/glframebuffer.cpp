#include "stdafx.h"
#include "glframebuffer.h"
#include "gltextures.h"

#include <glad/glad.h>

#include "engine/application.h"
#include "engine/engine.h"

glFramebuffer::glFramebuffer()
{
	int width, height;
	glfwGetWindowSize(g_engine.m_app->GetWindow(), &width, &height);

	// generate frame buffer
	glGenFramebuffers(1, &m_handle);
	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

	// generate render buffer
	glGenRenderbuffers(1, &m_render_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer);

	// create default depth buffer.
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	// and attach them to fbo
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_render_buffer);

	// unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glFramebuffer::~glFramebuffer()
{
	glDeleteRenderbuffers(1, &m_render_buffer);
	glDeleteFramebuffers(1, &m_handle);
}

unsigned int glFramebuffer::getHandle()
{
	return m_handle;
}

void glFramebuffer::setColorTexture(int index, ITexture2D* colorTexture)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, colorTexture->GetHandle(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void glFramebuffer::setDepthTexture(ITexture2D* depthTexture)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->GetHandle(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
