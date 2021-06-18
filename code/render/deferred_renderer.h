#ifndef DEFERRED_RENDERER_H
#define DEFERRED_RENDERER_H

class Shader;
class IFramebuffer;
class ITexture2D;
class IVertexBuffer;

class ScreenQuad
{
private:
	static IVertexBuffer* ms_vertexBuffer;
	static Shader* ms_screenQuadShader;

public:
	static void init();
	static void shutdown();

	static void render(ITexture2D* texture);
};

class Camera;
class Entity;

class DeferredRenderer
{
public:
	void init();
	void shutdown();

	IFramebuffer* getFramebuffer() { return m_framebuffer; }

	ITexture2D* getTexture(int index);

	void drawGeometry(Camera* camera, Entity* entity);

private:
	ITexture2D* m_textures[3];
	IFramebuffer* m_framebuffer;
	Shader* m_lightPassShader;

};

extern DeferredRenderer g_deferredRenderer;

#endif // !DEFERRED_RENDERER_H
