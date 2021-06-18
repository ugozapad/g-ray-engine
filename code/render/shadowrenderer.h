#ifndef SHADOWRENDERER_H
#define SHADOWRENDERER_H

#include "render/shader.h"

class Framebuffer;
class Entity;

class ShadowRenderer
{
public:
	void Init();
	void Shutdown();

	void RenderShadow(Entity* entity);

private:
	Framebuffer* m_Framebuffer;
	std::shared_ptr<Shader> m_ShadowShader;
};

extern ShadowRenderer g_shadowRenderer;

#endif
