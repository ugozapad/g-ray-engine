#ifndef DEVICE_H
#define DEVICE_H

#include "render/texture.h"
#include "render/vertexbuffer.h"
#include "render/indexbuffer.h"
#include "render/framebuffer.h"

struct Viewport
{
	int m_X;
	int m_Y;
	int m_Width;
	int m_Height;
};

enum PrimMode
{
	PM_TRIANGLES,
	PM_TRIANGLE_STRIP,
};

class IRenderDevice
{
public:
	enum ClearFlags
	{
		CLEAR_RT = 1 << 0,
		CLEAR_DEPTH = 1 << 1,
		CLEAR_STENCIL = 1 << 2,
	};

	enum DepthValue
	{
		DEPTH_NEVER,
		DEPTH_ALWAYS,
		DEPTH_LESS,
		DEPTH_LEQUAL,
		DEPTH_GREATER,
		DEPTH_GEQUAL,
		DEPTH_EQUAL,
		DEPTH_NOTEQUAL,
	};
public:
	virtual ~IRenderDevice() {}

	virtual void Clear(uint32_t flags) = 0;
	virtual void ClearColor(float r, float g, float b, float a) = 0;
	virtual void ClearDepth(float value) = 0;

	virtual void DepthMask(bool value) = 0;
	virtual void DepthTest(bool value) = 0;

	virtual void SetViewport(Viewport* viewport) = 0;
	virtual void SetTexture2D(uint32_t num, ITexture2D* texture) = 0;
	virtual void SetVertexBuffer(IVertexBuffer* buffer) = 0;
	virtual void SetIndexBuffer(IIndexBuffer* buffer) = 0;
	virtual void SetFramebuffer(IFramebuffer* buffer) = 0;

	virtual void DrawArray(PrimMode primMode, size_t startof, size_t count) = 0;
	virtual void DrawElement(PrimMode primMode, size_t count) = 0;

	virtual IVertexBuffer* CreateVertexBuffer(void* data, size_t size, BufferAccess access) = 0;
	virtual IIndexBuffer* CreateIndexBuffer(unsigned int* data, size_t size, BufferAccess access) = 0;
	virtual IFramebuffer* CreateFramebuffer() = 0;
	virtual ITexture2D* CreateTexture2D(const TextureCreationDesc& desc) = 0;
};

extern IRenderDevice* g_renderDevice;

#endif // !DEVICE_H
