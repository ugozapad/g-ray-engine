#include "stdafx.h"
#include "deferred_renderer.h"
#include "shadersystem.h"
#include "shader.h"
#include "device.h"
#include "texture.h"
#include "model.h"

#include "engine/application.h"
#include "engine/engine.h"
#include "render/screenquad.h"

DeferredRenderer g_deferredRenderer;

void DeferredRenderer::init()
{
	int width, height;
	glfwGetWindowSize(g_engine.m_app->GetWindow(), &width, &height);

	// create color texture
	TextureCreationDesc colorDesc = {0};
	colorDesc.m_width = width;
	colorDesc.m_height = height;
	colorDesc.m_format = ImageFormat::FMT_RGBA;
	m_textures[0] = g_renderDevice->CreateTexture2D(colorDesc);

	// create normal texture
	TextureCreationDesc normalDesc = {0};
	colorDesc.m_width = width;
	colorDesc.m_height = height;
	colorDesc.m_format = ImageFormat::FMT_RGBA;
	m_textures[1] = g_renderDevice->CreateTexture2D(colorDesc);

	// create specular texture
	TextureCreationDesc specularDesc = { 0 };
	specularDesc.m_width = width;
	specularDesc.m_height = height;
	specularDesc.m_format = ImageFormat::FMT_RGBA;
	m_textures[2] = g_renderDevice->CreateTexture2D(specularDesc);

	// create framebuffer
	m_framebuffer = g_renderDevice->CreateFramebuffer();

	// attach our texture's
	m_framebuffer->setColorTexture(0, m_textures[0]);
	m_framebuffer->setColorTexture(1, m_textures[1]);
	m_framebuffer->setColorTexture(2, m_textures[2]);

	//////////////////////////////////////////////////////////////////////////
	// ugly piece of shit
	g_renderDevice->SetFramebuffer(m_framebuffer);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, DrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Error("DeferredRenderer::init: Framebuffer is not complete.");

	g_renderDevice->SetFramebuffer(0);
	//////////////////////////////////////////////////////////////////////////

	// create light pass shader.
	m_lightPassShader = g_shaderSystem.CreateShader("def_light");
}

void DeferredRenderer::shutdown()
{
	mem_delete(m_framebuffer);
	mem_delete(m_textures[1]);
	mem_delete(m_textures[0]);
}

ITexture2D* DeferredRenderer::getTexture(int index)
{
	return m_textures[index];
}

#include "engine/camera.h"
#include "engine/entity.h"
#include "render/render.h"
#include "game/light.h"

void DeferredRenderer::drawGeometry(Camera* camera, Entity* entity)
{
	ASSERT(camera);
	ASSERT(entity);
	ASSERT(entity->m_Model);

	RenderContext& ctx = g_render->GetRenderContext();
	ctx.view = camera->GetViewMatrix();
	ctx.proj = camera->GetProjectionMatrix();
	ctx.model = entity->GetTranslation();

	g_render->SetRenderContext(ctx);

	// submit entity to render
	entity->Render();
}

void DeferredRenderer::drawLight(Camera* camera, Light* light)
{
	ASSERT(camera);
	ASSERT(light);

	m_lightPassShader->Bind();

	g_renderDevice->SetTexture2D(0, m_textures[RT_POS]);
	m_lightPassShader->SetInteger("u_positionTexture", 0);

	g_renderDevice->SetTexture2D(1, m_textures[RT_NORMAL]);
	m_lightPassShader->SetInteger("u_normalTexture", 1);

	g_renderDevice->SetTexture2D(2, m_textures[RT_COLOR]);
	m_lightPassShader->SetInteger("u_colorTexture", 2);

	m_lightPassShader->SetVec3("u_lightPos", light->m_Position);
	m_lightPassShader->SetVec3("u_viewPos", camera->m_Position);

	ScreenQuad::renderWithoutTextureBinding(m_lightPassShader);
}
