#ifndef RENDER_GL_BUFFERS_H
#define RENDER_GL_BUFFERS_H

#include "render/vertexbuffer.h"
#include "render/indexbuffer.h"

GLenum GetBufferAccess(BufferAccess access);
GLenum GetBufferMapping(BufferMapping mapping);

class glVertexBuffer : public IVertexBuffer
{
public:
	glVertexBuffer(void* data, size_t size, BufferAccess access);
	~glVertexBuffer();

	void Bind();

	void* Map(BufferMapping mapping);
	void Unmap();

private:
	GLuint m_glbuffer;
};

class glIndexBuffer : public IIndexBuffer
{
public:
	glIndexBuffer(unsigned int* data, size_t size, BufferAccess access);
	~glIndexBuffer();

	void Bind();

	void* Map(BufferMapping mapping);
	void Unmap();

private:
	GLuint m_glbuffer;
};

#endif // !RENDER_GL_BUFFERS_H
