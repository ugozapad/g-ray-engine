#include "stdafx.h"
#include "gldevice.h"

#include "gltextures.h"
#include "glbuffers.h"
#include "glframebuffer.h"

#include "../framebuffer.h"
#include "../r_stats.h"

static glRenderDevice s_glRenderDevice;
IRenderDevice* g_renderDevice = static_cast<IRenderDevice*>(&s_glRenderDevice);

glRenderDevice::glRenderDevice()
{

}

glRenderDevice::~glRenderDevice()
{

}

void glRenderDevice::Clear(uint32_t flags)
{
	if (flags > IRenderDevice::CLEAR_STENCIL)
		Error("IRenderDevice::Clear: Unknown flag passed");

	GLenum clearFlags = 0;
	if (flags & IRenderDevice::CLEAR_RT)
		clearFlags |= GL_COLOR_BUFFER_BIT;
	if (flags & IRenderDevice::CLEAR_DEPTH)
		clearFlags |= GL_DEPTH_BUFFER_BIT;
	if (flags & IRenderDevice::CLEAR_STENCIL)
		clearFlags |= GL_STENCIL_BUFFER_BIT;

	glClear(clearFlags);
}

void glRenderDevice::ClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void glRenderDevice::ClearDepth(float value)
{
	glClearDepth(value);
}

void glRenderDevice::DepthMask(bool value)
{
	glDepthMask(value);
}

void glRenderDevice::DepthTest(bool value)
{
	value ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void glRenderDevice::SetViewport(Viewport* viewport)
{
	if (!viewport)
		Error("IRenderDevice::SetViewport: Viewport is null pointer.");

	glViewport(viewport->m_X, viewport->m_Y, viewport->m_Width, viewport->m_Height);
}

void glRenderDevice::SetTexture2D(uint32_t num, ITexture2D* texture)
{
	if (!texture)
		Error("IRenderDevice::SetTexture2D: Texture is null pointer.");

	if (num > GL_TEXTURE31)
		Error("IRenderDevice::SetTexture2D: Reach maxmimum value of slot position. %i (max %i)", num, 31);

	glActiveTexture(GL_TEXTURE0 + num);

	if (texture)
		glBindTexture(GL_TEXTURE_2D, texture->GetHandle());
	else
		glBindTexture(GL_TEXTURE_2D, 0);
}

void glRenderDevice::SetVertexBuffer(IVertexBuffer* buffer)
{
	if (buffer)
		buffer->Bind();
	else
		glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void glRenderDevice::SetIndexBuffer(IIndexBuffer* buffer)
{
	if (buffer)
		buffer->Bind();
	else
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void glRenderDevice::SetFramebuffer(IFramebuffer* buffer)
{
	if (buffer)
		glBindFramebuffer(GL_FRAMEBUFFER, buffer->getHandle());
	else
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void glRenderDevice::DrawArray(PrimMode primMode, size_t startof, size_t count)
{
	g_renderStats.m_drawCalls++;
	//g_renderStats.m_verticesCount += startof - count;

	glDrawArrays(GL_TRIANGLES, startof, count);
}

void glRenderDevice::DrawElement(PrimMode primMode, size_t count)
{
	g_renderStats.m_drawCalls++;

	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

IVertexBuffer* glRenderDevice::CreateVertexBuffer(void* data, size_t size, BufferAccess access)
{
	return New(glVertexBuffer, data, size, access);
}

IIndexBuffer* glRenderDevice::CreateIndexBuffer(unsigned int* data, size_t size, BufferAccess access)
{
	return New(glIndexBuffer, data, size, access);
}

IFramebuffer* glRenderDevice::CreateFramebuffer()
{
	return New(glFramebuffer);
}

ITexture2D* glRenderDevice::CreateTexture2D(const TextureCreationDesc& desc)
{
	return New(glTexture2D, desc);
}
