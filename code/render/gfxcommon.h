#ifndef GFXCOMMON_H
#define GFXCOMMON_H

#include <glad/glad.h>

enum class BufferAccess
{
	Static,
	Dynamic,
	Stream
};

enum class ImageFormat
{
	FMT_RGB,
	FMT_RGBA,
	FMT_DEPTH24_STENCIL8,
	FMT_DEPTH32_STENCIL_8_F,
	FMT_SRGB,

	FMT_RGB32F,
	FMT_RGBA32F,
};

struct RenderContext
{
	int width;
	int height;
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 model;
};

enum class BufferMapping
{
	READ_ONLY,
	WRITE_ONLY,
	READ_WRITE
};

struct IGPUBuffer
{
	virtual ~IGPUBuffer() {}

	virtual void* Map(BufferMapping mapping) = 0;
	virtual void Unmap() = 0;
};

#endif // GFXCOMMON_H
