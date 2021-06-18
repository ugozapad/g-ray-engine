#include "stdafx.h"
#include "glbuffers.h"
#include "render/gl_exts.h"

glVertexBuffer::glVertexBuffer(void* data, size_t size, BufferAccess access)
{
	glGenBuffers(1, &m_glbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_glbuffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, GetBufferAccess(access));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

glVertexBuffer::~glVertexBuffer()
{
	glDeleteBuffers(1, &m_glbuffer);
}

void glVertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_glbuffer);
}

void* glVertexBuffer::Map(BufferMapping mapping)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_glbuffer);
	void* ret = glMapBuffer(GL_ARRAY_BUFFER, GetBufferMapping(mapping));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return ret;
}

void glVertexBuffer::Unmap()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_glbuffer);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

glIndexBuffer::glIndexBuffer(unsigned int* data, size_t size, BufferAccess access)
{
	glGenBuffers(1, &m_glbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GetBufferAccess(access));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

glIndexBuffer::~glIndexBuffer()
{
	glDeleteBuffers(1, &m_glbuffer);
}

void glIndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glbuffer);
}

void* glIndexBuffer::Map(BufferMapping mapping)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glbuffer);
	void* ret = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GetBufferMapping(mapping));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return ret;
}

void glIndexBuffer::Unmap()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glbuffer);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLenum GetBufferAccess(BufferAccess access)
{
	switch (access)
	{
	case BufferAccess::Static: return GL_STATIC_DRAW;
	case BufferAccess::Dynamic: return GL_DYNAMIC_DRAW;
	case BufferAccess::Stream: return GL_STREAM_DRAW;
	}

	return 0;
}

GLenum GetBufferMapping(BufferMapping mapping)
{
	switch (mapping)
	{
	case BufferMapping::READ_ONLY: return GL_READ_ONLY;
	case BufferMapping::WRITE_ONLY: return GL_WRITE_ONLY;
	case BufferMapping::READ_WRITE: return GL_READ_WRITE;
	}

	return 0;
}
