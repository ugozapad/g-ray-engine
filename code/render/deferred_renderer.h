#ifndef DEFERRED_RENDERER_H
#define DEFERRED_RENDERER_H

class Shader;
class IFramebuffer;
class ITexture2D;
class IVertexBuffer;
class Camera;
class Entity;
class Light;

class DeferredRenderer
{
public:
	enum RT
	{
		RT_POS,
		RT_NORMAL,
		RT_COLOR
	};
public:
	void init();
	void shutdown();

	IFramebuffer* getFramebuffer() { return m_framebuffer; }

	ITexture2D* getTexture(int index);

	void drawGeometry(Camera* camera, Entity* entity);
	void drawLight(Camera* camera, Light** lights, size_t size);
	void drawNoLight(Camera* camera);

private:
	ITexture2D* m_textures[3];
	IFramebuffer* m_framebuffer;
	Shader* m_lightPassShader;

};

extern DeferredRenderer g_deferredRenderer;

#endif // !DEFERRED_RENDERER_H
