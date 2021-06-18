#ifndef RENDER_H
#define RENDER_H

#include "render/gfxcommon.h"

class Application;
class ModelBase;
class SubMesh;
class Material;
class Shader;
class IVertexBuffer;
class IIndexBuffer;

class IRender
{
public:
	virtual ~IRender() {}

	virtual void Init(Application* app) = 0;
	virtual void Shutdown() = 0;

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

	virtual void MakeScreenShot() = 0;

	virtual void SetRenderContext(const RenderContext& ctx) = 0;
	virtual RenderContext& GetRenderContext() = 0;

	virtual void RenderSubMesh(const RenderContext& ctx, SubMesh* submesh, Shader* shader, Material* material = NULL) = 0;
	virtual void RenderMesh(const RenderContext& ctx, IVertexBuffer* vertexbuffer, IIndexBuffer* indexbuffer, size_t indeciesCount, Shader* shader, Material* material = NULL) = 0;
	virtual void RenderMesh(const RenderContext& ctx, IVertexBuffer* vertexbuffer, size_t verticesCount, Shader* shader, Material* material = NULL) = 0;
};

extern IRender* g_render;

class Framebuffer;

Framebuffer* GetFramebuffer();

#endif // !RENDER_H
