#ifndef GLRENDER_H
#define GLRENDER_H

#include "../render.h"

class glRender : public IRender
{
public:
	glRender();
	~glRender();

	void Init(Application* app);
	void Shutdown();
	void MakeScreenShot();

	void BeginFrame();
	void EndFrame();

	void SetRenderContext(const RenderContext& ctx) { m_Ctx = ctx; }
	RenderContext& GetRenderContext() { return m_Ctx; }

	void RenderSubMesh(const RenderContext& ctx, SubMesh* submesh, Shader* shader, Material* material = NULL);
	void RenderMesh(const RenderContext& ctx, IVertexBuffer* vertexbuffer, IIndexBuffer* indexbuffer, size_t indeciesCount, Shader* shader, Material* material = NULL);
	void RenderMesh(const RenderContext& ctx, IVertexBuffer* vertexbuffer, size_t verticesCount, Shader* shader, Material* material = NULL);

private:
	void SetCtx();

	Application* m_app;
	RenderContext m_Ctx;

public:
	Framebuffer* m_frameBuffer;
};

#endif // !GLRENDER_H
