#ifndef GLDEVICE_H
#define GLDEVICE_H

#include "../device.h"

class glRenderDevice : public IRenderDevice
{
public:
	glRenderDevice();
	~glRenderDevice();

	void Clear(uint32_t flags);
	void ClearColor(float r, float g, float b, float a);
	void ClearDepth(float value);

	void DepthMask(bool value);
	void DepthTest(bool value);

	void SetViewport(Viewport* viewport);
	void SetTexture2D(uint32_t num, ITexture2D* texture);
	void SetVertexBuffer(IVertexBuffer* buffer);
	void SetIndexBuffer(IIndexBuffer* buffer);
	void SetFramebuffer(IFramebuffer* buffer);

	void DrawArray(PrimMode primMode, size_t startof, size_t count);
	void DrawElement(PrimMode primMode, size_t count);

	IVertexBuffer* CreateVertexBuffer(void* data, size_t size, BufferAccess access);
	IIndexBuffer* CreateIndexBuffer(unsigned int* data, size_t size, BufferAccess access);
	IFramebuffer* CreateFramebuffer();
	ITexture2D* CreateTexture2D(const TextureCreationDesc& desc);
};

#endif // !GLDEVICE_H
