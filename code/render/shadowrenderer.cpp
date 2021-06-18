#include "stdafx.h"
#include "shadowrenderer.h"
#include "device.h"
#include "engine/entity.h"
#include "render.h"

ShadowRenderer g_shadowRenderer;

void ShadowRenderer::Init()
{
	//m_Framebuffer = g_renderDevice->CreateFramebuffer();
	//m_ShadowShader = g_resourceManager->LoadShader("shadow_map");
}

void ShadowRenderer::Shutdown()
{
	//mem_delete(m_Framebuffer);
}

void ShadowRenderer::RenderShadow(Entity* entity)
{
	Viewport vp = {0, 0, 256, 256};
	g_renderDevice->SetViewport(&vp);

	//g_renderDevice->SetFramebuffer(m_Framebuffer);
	g_renderDevice->Clear(IRenderDevice::CLEAR_DEPTH);

	if (entity->m_EntityFlags ^ Entity::EF_NOVISUAL ||
		entity->m_EntityFlags ^ Entity::EF_INVISIBLE)
	{
		float near_plane = 1.0f, far_plane = 7.5f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

		glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		RenderContext ctx = {0};
		ctx.model = entity->GetTranslation();
		ctx.view = lightView;
		ctx.proj = lightProjection;

		g_render->SetRenderContext(ctx);
		entity->Render();
	}
}

